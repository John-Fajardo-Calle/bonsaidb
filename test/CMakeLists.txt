add_executable(test_insert test_insert.cpp)
add_executable(test_query test_query.cpp)
add_executable(test_concurrency test_concurrency.cpp
        test_delete.cpp
        test_read_write.cpp)
add_executable(test_delete test_delete.cpp)
add_executable(test_read_write test_read_write.cpp)


target_link_libraries(test_insert PRIVATE bonsaidb_lib)
target_link_libraries(test_query PRIVATE bonsaidb_lib)
target_link_libraries(test_concurrency PRIVATE bonsaidb_lib)
target_link_libraries(test_delete PRIVATE bonsaidb_lib)
target_link_libraries(test_read_write PRIVATE bonsaidb_lib)
