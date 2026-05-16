import os

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription, LogInfo
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from launch.launch_description_sources import PythonLaunchDescriptionSource

from ament_index_python.packages import get_package_share_directory


def _detect_default_host():
    """Return the simulator host IP.

    Under WSL2 the Unreal simulator runs on the Windows host, which is reachable
    from the WSL2 guest at the default route gateway (the vEthernet adapter IP
    on the Windows side). /etc/resolv.conf's nameserver may point to WSL2's
    internal DNS resolver (10.255.255.254) instead, so we use /proc/net/route.
    Outside WSL we fall back to localhost.
    """
    try:
        with open("/proc/version", "r") as f:
            if "microsoft" not in f.read().lower():
                return "localhost"
    except OSError:
        return "localhost"
    try:
        with open("/proc/net/route", "r") as f:
            next(f)  # header
            for line in f:
                fields = line.strip().split()
                if len(fields) >= 3 and fields[1] == "00000000":
                    gw_hex = fields[2]
                    return ".".join(
                        str(int(gw_hex[i:i + 2], 16)) for i in (6, 4, 2, 0)
                    )
    except (OSError, StopIteration, ValueError):
        pass
    return "localhost"


def generate_launch_description():

    output = DeclareLaunchArgument(
        "output",
        default_value='log')

    publish_clock = DeclareLaunchArgument(
        "publish_clock",
        default_value='False')

    is_vulkan = DeclareLaunchArgument(
        "is_vulkan",
        default_value='True')

    default_host = _detect_default_host()
    host = DeclareLaunchArgument(
        "host",
        default_value=default_host,
        description="Simulator host IP. Auto-detected on WSL2 (Windows host via /etc/resolv.conf); localhost otherwise.")
  
    cmd_vel_linear_scale = DeclareLaunchArgument(
        "cmd_vel_linear_scale",
        default_value='10.0',
        description='Scale factor applied to cmd_vel linear.x. Defaults to 10.0 to match the current 10x map scale; set to 1.0 if returning to default-scale map')

    cmd_vel_angular_scale = DeclareLaunchArgument(
        "cmd_vel_angular_scale",
        default_value='1.0',
        description='Scale factor applied to cmd_vel angular.z. Defaults to 1.0 because angular.z maps to wheel steering angle (-1..1), which is independent of map scale')

    furosim_node = Node(
            package='furosim_ros_pkgs',
            executable='furosim_node',
            name='furosim_node',
            output='screen',
            parameters=[{
                'is_vulkan': False,
                'update_airsim_img_response_every_n_sec': 0.05,
                'update_airsim_control_every_n_sec': 0.01,
                'update_lidar_every_n_sec': 0.01,
                'publish_clock': LaunchConfiguration('publish_clock'),
                'host_ip': LaunchConfiguration('host'),
                'cmd_vel_linear_scale': LaunchConfiguration('cmd_vel_linear_scale'),
                'cmd_vel_angular_scale': LaunchConfiguration('cmd_vel_angular_scale')
            }])

    static_transforms = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(get_package_share_directory('furosim_ros_pkgs'), 'launch/static_transforms.launch.py')
        )
    )

    # Create the launch description and populate
    ld = LaunchDescription()

    # Declare the launch options
    ld.add_action(output)
    ld.add_action(publish_clock)
    ld.add_action(is_vulkan)
    ld.add_action(host)
    ld.add_action(cmd_vel_linear_scale)
    ld.add_action(cmd_vel_angular_scale)

    ld.add_action(LogInfo(msg=f"furosim_node: using simulator host '{default_host}'"))
    ld.add_action(static_transforms)
    ld.add_action(furosim_node)

    return ld
