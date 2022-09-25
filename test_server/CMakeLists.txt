project(bookcourt_test_server VERSION 0.1.0)

add_executable(bookcourt_test_server test_server.cpp)

target_link_libraries(bookcourt_test_server
  pthread
)