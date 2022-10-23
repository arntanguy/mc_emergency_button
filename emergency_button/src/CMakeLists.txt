add_executable(test_emergency_button emergency_button.cpp)
target_link_libraries(test_emergency_button PUBLIC emergency_button)

install(TARGETS test_emergency_button
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin)
