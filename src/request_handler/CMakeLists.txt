project(request_handler)

aux_source_directory(. SOURCE)

add_library(${PROJECT_NAME}
  ${SOURCE}
)

target_link_libraries(${PROJECT_NAME}
  boost_json
  my_config
  bookcourt_util
  post_request_sender
  task_timer
)
