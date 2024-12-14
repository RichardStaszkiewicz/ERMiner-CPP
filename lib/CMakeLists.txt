add_library(erminer STATIC ${SRC})
target_include_directories(erminer PUBLIC BEFORE ${CMAKE_SOURCE_DIR}/include)
