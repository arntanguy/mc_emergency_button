cmake_minimum_required(VERSION 3.1)

add_library(emergency_button_usbio SHARED src/usbio.c src/usbio.h src/usbio_private.h)
target_link_libraries(emergency_button_usbio PUBLIC usb)

set(emergency_button_SRC
  src/WiredEmergencyButton.cpp
  src/WirelessEmergencyButton.cpp
  )

set(emergency_button_HDR
  include/emergency_button/EmergencyButton.h
  include/emergency_button/WiredEmergencyButton.h
  include/emergency_button/WirelessEmergencyButton.h
  )

add_library(emergency_button SHARED "${emergency_button_SRC}" "${emergency_button_HDR}")
target_include_directories(emergency_button PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>)
target_link_libraries(emergency_button PUBLIC emergency_button_usbio)
target_link_libraries(emergency_button PUBLIC Threads::Threads)

add_subdirectory(src)
