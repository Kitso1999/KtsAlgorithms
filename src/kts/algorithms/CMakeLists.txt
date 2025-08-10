add_library(KtsAlgorithms INTERFACE)

target_compile_features(KtsAlgorithms INTERFACE cxx_std_20)
target_include_directories(KtsAlgorithms INTERFACE
    $<BUILD_INTERFACE:${KtsAlgorithms_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
)
target_sources(KtsAlgorithms PUBLIC FILE_SET HEADERS FILES sort.hpp )

add_library(Kts::Algorithms ALIAS KtsAlgorithms)
