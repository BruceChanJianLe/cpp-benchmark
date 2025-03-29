# prm performance benchmark
add_executable(pmr_performance ./pmr_performance.cpp)
target_include_directories(pmr_performance PRIVATE ../include)
target_link_libraries(pmr_performance benchmark::benchmark)

# vector benchmark
add_executable(vector_benchmark ./vector_benchmark.cpp)
target_include_directories(vector_benchmark PRIVATE ../include)
target_link_libraries(vector_benchmark benchmark::benchmark)

# game entity benchmark
add_executable(game_entity_benchmark ./game_entity_benchmark.cpp)
target_include_directories(game_entity_benchmark PRIVATE ../include)
target_link_libraries(game_entity_benchmark benchmark::benchmark game_entity)
