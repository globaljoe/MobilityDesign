import os

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration, PythonExpression
from launch_ros.actions import Node


def generate_launch_description():
    sim_node_name = DeclareLaunchArgument(
        "sim_node_name",
        default_value="furosim_node",
        description="Name of the running furosim node (topic prefix root)")

    vehicle_name = DeclareLaunchArgument(
        "vehicle_name",
        default_value="PhysXCar",
        description="Vehicle name as configured in AirSim settings.json")

    speed = DeclareLaunchArgument(
        "speed",
        default_value="0.3",
        description="Target linear.x magnitude (m/s before cmd_vel_linear_scale is applied on the sim side)")

    turn = DeclareLaunchArgument(
        "turn",
        default_value="1.0",
        description="angular.z magnitude per key press (mapped directly to steering, [-1, 1])")

    cmd_vel_topic = PythonExpression([
        "'/' + '", LaunchConfiguration("sim_node_name"),
        "' + '/' + '", LaunchConfiguration("vehicle_name"),
        "' + '/cmd_vel'"
    ])

    # Forward display-related env vars explicitly so xterm can find WSLg/Xorg.
    # ros2 launch sometimes drops these when spawning child processes.
    display_env = {
        "DISPLAY": os.environ.get("DISPLAY", ":0"),
        "WAYLAND_DISPLAY": os.environ.get("WAYLAND_DISPLAY", "wayland-0"),
        "XDG_RUNTIME_DIR": os.environ.get("XDG_RUNTIME_DIR", "/run/user/1000"),
        "XAUTHORITY": os.environ.get("XAUTHORITY", ""),
    }

    teleop = Node(
        package="teleop_twist_keyboard",
        executable="teleop_twist_keyboard",
        name="car_teleop",
        output="screen",
        prefix="xterm -fa Monospace -fs 11 -hold -e",
        additional_env=display_env,
        parameters=[{
            "speed": LaunchConfiguration("speed"),
            "turn": LaunchConfiguration("turn"),
        }],
        remappings=[("/cmd_vel", cmd_vel_topic)],
    )

    ld = LaunchDescription()
    ld.add_action(sim_node_name)
    ld.add_action(vehicle_name)
    ld.add_action(speed)
    ld.add_action(turn)
    ld.add_action(teleop)
    return ld
