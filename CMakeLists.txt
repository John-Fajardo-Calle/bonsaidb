cmake_minimum_required(VERSION 3.20)
project(bonsaidb VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-march=x86-64)

set(BONSAIDB_SOURCES
        src/Record.cpp
        src/Page.cpp
        src/FileManager.cpp
        src/BPlusTree.cpp
        src/DatabaseEngine.cpp
)

add_library(bonsaidb_lib ${BONSAIDB_SOURCES})

target_include_directories(bonsaidb_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(bonsaidb src/main.cpp)
target_link_libraries(bonsaidb PRIVATE bonsaidb_lib)

add_subdirectory(benchmarks)
add_subdirectory(test)
