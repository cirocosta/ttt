include_directories(${ttt_INCLUDE_DIRS})

add_executable(ttt-server "./ttt-server.cc")
add_executable(ttt-client "./ttt-client.cc")

target_link_libraries(ttt-server ${ttt_LIBRARIES})
target_link_libraries(ttt-client ${ttt_LIBRARIES})

