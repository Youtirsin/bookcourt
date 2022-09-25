project(post_request_sender)

aux_source_directory(. SOURCE)

add_library(${PROJECT_NAME}
  ${SOURCE}
)

target_link_libraries(${PROJECT_NAME}
  curl
)
