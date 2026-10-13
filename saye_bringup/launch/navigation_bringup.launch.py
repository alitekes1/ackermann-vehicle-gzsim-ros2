import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    pkg_nav2_dir = get_package_share_directory('nav2_bringup')
    pkg_saye_bringup = get_package_share_directory('saye_bringup')

    use_sim_time = LaunchConfiguration('use_sim_time')
    autostart = LaunchConfiguration('autostart')

    # bringup_launch.py already starts map_server + amcl (and the lifecycle manager),
    # and AMCL publishes map->odom. Do not start them (or a static map->odom) again.
    nav2_launch_cmd = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(pkg_nav2_dir, 'launch', 'bringup_launch.py')
        ),
        launch_arguments={
            'use_sim_time': use_sim_time,
            'autostart': autostart,
            'map': os.path.join(pkg_saye_bringup, 'maps', 'map.yaml'),
            'params_file': os.path.join(pkg_saye_bringup, 'config', 'nav2_params.yaml'),
        }.items()
    )

    rviz_launch_cmd = Node(
        package='rviz2',
        executable='rviz2',
        name='rviz2',
        arguments=['-d', os.path.join(pkg_saye_bringup, 'rviz', 'navigation.rviz')],
        parameters=[{'use_sim_time': use_sim_time}],
    )

    return LaunchDescription([
        DeclareLaunchArgument('use_sim_time', default_value='true',
                              description='Use simulation (Gazebo) clock'),
        DeclareLaunchArgument('autostart', default_value='true',
                              description='Automatically start the Nav2 stack'),
        nav2_launch_cmd,
        rviz_launch_cmd,
    ])
