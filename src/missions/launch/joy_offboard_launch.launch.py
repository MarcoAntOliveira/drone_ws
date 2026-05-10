from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
from launch.actions import ExecuteProcess
import os

def generate_launch_description():
    ld = LaunchDescription()

    # param_config = os.path.join(get_package_share_directory("px4_ros_com"), "config", "camera_bridge.yaml")


    micro_ros_agent = ExecuteProcess(
        cmd=[[
            'micro-ros-agent udp4 --port 8888 -v '
        ]],
        shell=True
    )

    offboard_node = Node(
        package='missions',
        executable='joy_offboard',
        output='screen',
        shell=True,
    )


    # bridge_node = Node(
    #     package='ros_gz_bridge',
    #     executable='parameter_bridge',
    #     name='camera_bridge',
    #     parameters=[{'config_file': param_config}], # Note a chave 'config_file'
    #     output='screen',
    # )

    # ld.add_action(micro_ros_agent)
    ld.add_action(offboard_node)
    
    # ld.add_action(bridge_node)

    return ld
