import os

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from launch.launch_description_sources import PythonLaunchDescriptionSource

from ament_index_python.packages import get_package_share_directory


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

    host = DeclareLaunchArgument(
        "host",
        default_value='localhost')
  
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

    ld.add_action(static_transforms)
    ld.add_action(furosim_node)

    return ld
