include_directories(${INCLUDE_DIRS})

file(GLOB srcs "*.cc")
add_library(ttt ${srcs})

set(LIBRARIES ${LIBRARIES} "ttt" PARENT_SCOPE)


