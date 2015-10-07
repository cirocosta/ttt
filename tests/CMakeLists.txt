include_directories(${ttt_INCLUDE_DIRS})

file(GLOB_RECURSE tests "*.cc")
foreach(file ${tests})
  get_filename_component(testname ${file} NAME_WE)

  add_executable(${testname} ${file})
  target_link_libraries(${testname} gtest gmock gmock_main ${ttt_LIBRARIES})
  add_test(NAME ${testname} COMMAND ${testname})

endforeach()

