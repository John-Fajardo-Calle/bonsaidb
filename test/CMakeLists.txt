# test/CMakeLists.txt
add_executable(test_insert test_insert.cpp)
add_executable(test_query test_query.cpp)
add_executable(test_concurrency test_concurrency.cpp)

target_link_libraries(test_insert PRIVATE bonsaidb_lib)
target_link_libraries(test_query PRIVATE bonsaidb_lib)
target_link_libraries(test_concurrency PRIVATE bonsaidb_lib)