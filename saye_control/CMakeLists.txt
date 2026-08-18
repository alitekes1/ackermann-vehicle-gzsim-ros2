cmake_minimum_required(VERSION 3.8)
project(saye_control)

# Set C++ standard to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclpy REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(saye_msgs REQUIRED)
find_package(nav2_msgs REQUIRED)
include_directories(include)

install(
  DIRECTORY include
  DESTINATION include
)

add_executable(main src/main.cpp)
ament_target_dependencies(main rclcpp saye_msgs nav_msgs sensor_msgs)

add_executable(client src/client.cpp)
ament_target_dependencies(client rclcpp saye_msgs nav_msgs)

install(
  TARGETS 
  main 
  client
  DESTINATION lib/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
