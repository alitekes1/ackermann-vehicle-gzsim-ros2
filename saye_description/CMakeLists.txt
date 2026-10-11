cmake_minimum_required(VERSION 3.8)
project(saye_description)

find_package(ament_cmake REQUIRED)

install(DIRECTORY
  models
  worlds
  DESTINATION share/${PROJECT_NAME}/
)

# Adds <install>/share and <install>/share/saye_description/models to GZ_SIM_RESOURCE_PATH
# when the workspace is sourced, so model:// and package:// URIs resolve without manual exports.
ament_environment_hooks("${CMAKE_CURRENT_SOURCE_DIR}/hooks/${PROJECT_NAME}.dsv.in")
ament_environment_hooks("${CMAKE_CURRENT_SOURCE_DIR}/hooks/${PROJECT_NAME}.sh.in")

ament_package()
