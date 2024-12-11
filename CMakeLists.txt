cmake_minimum_required(VERSION 3.16)


set(FLAGS -std=c++17 -O3 -Wall -g -I /usr/local/opt/llvm/include)
set(CMAKE_CXX_COMPILER clang++)
add_compile_options(${FLAGS})
project(ERMINER VERSION 0.1 LANGUAGES CXX)

file(GLOB_RECURSE SRC ${CMAKE_SOURCE_DIR}/source/*.cpp)

find_package(GTest REQUIRED)

add_subdirectory(${CMAKE_SOURCE_DIR}/lib)


# tutaj się pisze który program będzie skompilowany do pliku wykonywalnego
# bez przecinka można dodawać kolejne pliki
# JAK NIE UŻYWAMY BIBLIOTEKI
# należy dodać za main.cpp wszystkie pliki źródłowe zapisane w zmiennej SRC:
# add_executable(program main.cpp, ${SRC})
add_executable(erminer main.cpp)
add_executable(runItemsetTests tests/test_itemset.cpp)
add_executable(runRulesTests tests/test_rules.cpp)
add_executable(runERMinerTests tests/test_erminer.cpp)


target_include_directories(erminer PUBLIC BEFORE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(runItemsetTests PUBLIC BEFORE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(runRulesTests PUBLIC BEFORE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(runERMinerTests PUBLIC BEFORE ${CMAKE_SOURCE_DIR}/include)

# załączenie biblioteki do projektu (tak by potem inni też mogli korzystać)
# najpierw GTest, potem biblioteki stworzone w lib (w tym wypadku proiekt_lib)
target_link_libraries(erminer PUBLIC erminer_lib)
target_link_libraries(runItemsetTests PUBLIC GTest::GTest erminer_lib)
target_link_libraries(runRulesTests PUBLIC GTest::GTest erminer_lib)
target_link_libraries(runERMinerTests PUBLIC GTest::GTest erminer_lib)


# Aby uruchomić, mając zainstalowanego cmake-a należy wykonać komendy (w katalogu z głównym CMakeListem):
# cmake CMakeLists.txt
# make

# utworzony zostaje wówczas plik wykonywalny "program". Aby go uruchomić wystarczy:
# ./program