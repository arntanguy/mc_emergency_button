set(AUTOLOAD_EmergencyButtonPlugin_PLUGIN false CACHE BOOL "When true the plugin will always be loaded")

# Find mc_rtc if we are not building inside mc_rtc itself
if(NOT TARGET mc_rtc::mc_control)
  find_package(mc_rtc REQUIRED)
endif()

# Configure and install the plugin configuration
configure_file(etc/EmergencyButtonPlugin.in.yaml "${CMAKE_CURRENT_BINARY_DIR}/etc/EmergencyButtonPlugin.yaml")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/etc/EmergencyButtonPlugin.yaml" DESTINATION "${MC_PLUGINS_RUNTIME_INSTALL_PREFIX}/etc")

set(plugin_SRC
  src/EmergencyButtonPlugin.cpp
  )

set(plugin_HDR
  src/EmergencyButtonPlugin.h
)

add_executable(EmergencyButtonServer src/EmergencyButtonServer.cpp)
target_link_libraries(EmergencyButtonServer PUBLIC emergency_button mc_rtc::mc_control rt)

# The add_plugin macro is provided by mc_rtc
add_plugin(EmergencyButtonPlugin "${plugin_SRC}" "${plugin_HDR}")
target_link_libraries(EmergencyButtonPlugin PUBLIC rt)

add_subdirectory(tests)
