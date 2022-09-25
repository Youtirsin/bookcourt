set(Boost_INCLUDE_DIR /home/youtirsin/boost_1_80_0)
set(Boost_LIBRARY_DIR /home/youtirsin/boost_1_80_0/stage/lib)

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

include_directories(.)

add_subdirectory(config)
add_subdirectory(util)
add_subdirectory(task_timer)
add_subdirectory(post_request_sender)
add_subdirectory(request_handler)

add_executable(bookcourt 
  server.cpp
)
add_executable(bookcourt_test 
  test.cpp
)

target_link_libraries(bookcourt
  pthread
  my_config
  request_handler
)

target_link_libraries(bookcourt_test
  task_timer
)