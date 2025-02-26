add_library(sorting_algorithms)

target_compile_features(sorting_algorithms PUBLIC cxx_std_20)
target_sources(sorting_algorithms
        PUBLIC FILE_SET CXX_MODULES FILES sorting_algorithms.cppm
)
