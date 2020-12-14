cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME "sensors_plus_linux")
project(${PROJECT_NAME} LANGUAGES CXX)

set(PLUGIN_NAME "sensors_plus_linux_plugin")
set(RTIMULIB_DIR "../third_party/RTIMULib/RTIMULib")

add_library(${PLUGIN_NAME} SHARED
  "sensors_plus_linux_plugin.cc"
  "sensor_reader.cc"
)
apply_standard_settings(${PLUGIN_NAME})
set_target_properties(${PLUGIN_NAME} PROPERTIES
  CXX_VISIBILITY_PRESET hidden)
target_compile_definitions(${PLUGIN_NAME} PRIVATE FLUTTER_PLUGIN_IMPL)
target_include_directories(${PLUGIN_NAME} INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/${RTIMULIB_DIR}")
target_link_libraries(${PLUGIN_NAME} PRIVATE flutter)
target_link_libraries(${PLUGIN_NAME} PRIVATE RTIMULib)

set(sensors_plus_linux_bundled_libraries
  "$<TARGET_FILE:RTIMULib>"
  PARENT_SCOPE
)

add_subdirectory(${RTIMULIB_DIR} "${CMAKE_CURRENT_BINARY_DIR}/RTIMULib")
