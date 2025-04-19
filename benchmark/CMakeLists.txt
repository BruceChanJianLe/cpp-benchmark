# vector benchmark
add_executable(vector_benchmark ./vector_benchmark.cpp)
target_include_directories(vector_benchmark PRIVATE ../include)
target_link_libraries(vector_benchmark benchmark::benchmark)

# game entity benchmark
add_executable(game_entity_benchmark ./game_entity_benchmark.cpp)
target_include_directories(game_entity_benchmark PRIVATE ../include)
target_link_libraries(game_entity_benchmark benchmark::benchmark game_entity)

# standard parallelism
find_package(TBB REQUIRED)

add_executable(std_par_benchmark ./std_par_benchmark.cpp)
target_include_directories(std_par_benchmark PRIVATE ../include)
target_link_libraries(std_par_benchmark benchmark::benchmark TBB::tbb)
