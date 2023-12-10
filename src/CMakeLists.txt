add_library(utils INTERFACE)

target_include_directories(utils INTERFACE ../include)

target_compile_features(utils INTERFACE cxx_std_20)

