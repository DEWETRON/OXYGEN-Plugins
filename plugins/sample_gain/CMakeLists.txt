set(PLUGIN_NAME sample_gain)

# qml.rcc is generated from qml.qrc and compiled into the plugin
qt5_add_binary_resources(${PLUGIN_NAME}_qml qml.qrc DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/qml.rcc")

add_library(${PLUGIN_NAME} SHARED
  src/gain.h
  src/sample_gain_plugin.cpp
)
target_link_libraries(${PLUGIN_NAME} PRIVATE odk_framework)

set(RESOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/resources")
target_include_directories(${PLUGIN_NAME} PRIVATE "${RESOURCE_DIR}")
AddResourceFile(${PLUGIN_NAME} "${CMAKE_CURRENT_BINARY_DIR}/qml.rcc" "${RESOURCE_DIR}")
add_dependencies(${PLUGIN_NAME} ${PLUGIN_NAME}_qml)

SetPluginOutputOptions(${PLUGIN_NAME})

if(BUILD_TESTING)
  add_plugin_test(${PLUGIN_NAME} tests/gain_test.cpp)
endif()
