include_directories(${ttt_INCLUDE_DIRS})

add_executable(ttt-server "./ttt-server.cc")
add_executable(ttt-client "./ttt-client.cc")

target_link_libraries(ttt-server ${ttt_LIBRARIES})
target_link_libraries(ttt-client ${ttt_LIBRARIES})


# copy certificates directory to current location

add_custom_command(
  TARGET ttt-server POST_BUILD
  COMMAND ${CMAKE_COMMAND}
  -E copy_directory
  ${PROJECT_SOURCE_DIR}/certs ${PROJECT_BINARY_DIR}/certs
)

# create softlinks to binary files

add_custom_command(
    TARGET ttt-server POST_BUILD
    COMMAND ln -s ${CMAKE_CURRENT_BINARY_DIR}/ttt-server 
                  ${PROJECT_BINARY_DIR}/ttt-server
)

add_custom_command(
    TARGET ttt-client POST_BUILD
    COMMAND ln -s ${CMAKE_CURRENT_BINARY_DIR}/ttt-client 
                  ${PROJECT_BINARY_DIR}/ttt-client
)
