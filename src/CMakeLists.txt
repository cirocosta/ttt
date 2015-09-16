include_directories(${INCLUDE_DIRS})

add_executable(ttt-server "./ttt-server.cc")
add_executable(ttt-client "./ttt-client.cc")

target_link_libraries(ttt-server ${LIBRARIES})
target_link_libraries(ttt-client ${LIBRARIES})

