cmake_minimum_required(VERSION 3.21)
project(OxygenPlugins LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)
include(cmake/odk.cmake)
if(BUILD_TESTING)
  include(cmake/unit_tests.cmake)
endif()

# Every folder in plugins/ with a CMakeLists.txt is a plugin.
file(GLOB plugin_lists CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/plugins/*/CMakeLists.txt")
foreach(plugin_list IN LISTS plugin_lists)
  get_filename_component(plugin_dir "${plugin_list}" DIRECTORY)
  add_subdirectory("${plugin_dir}")
endforeach()
