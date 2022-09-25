project(my_config)

aux_source_directory(. SOURCE)

add_library(${PROJECT_NAME}
  ${SOURCE}
)

target_link_libraries(${PROJECT_NAME}
  boost_filesystem
)
