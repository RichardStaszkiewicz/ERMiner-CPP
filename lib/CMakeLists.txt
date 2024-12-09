add_library(erminer_lib STATIC ${SRC})
target_include_directories(erminer_lib PUBLIC BEFORE ${CMAKE_SOURCE_DIR}/include)
