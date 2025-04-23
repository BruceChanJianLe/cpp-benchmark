# vector benchmark
add_executable(vector_benchmark ./vector_benchmark.cpp)
target_include_directories(vector_benchmark PRIVATE ../include)
target_link_libraries(vector_benchmark benchmark::benchmark)

# game entity benchmark
add_executable(game_entity_benchmark ./game_entity_benchmark.cpp)
target_include_directories(game_entity_benchmark PRIVATE ../include)
target_link_libraries(game_entity_benchmark benchmark::benchmark game_entity)

# nested loop
add_executable(nested_loop ./nested_loop.cpp)
target_include_directories(nested_loop PRIVATE ../include)
target_link_libraries(nested_loop benchmark::benchmark TBB::tbb std::mdspan)
