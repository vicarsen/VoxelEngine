add_executable(typetest typetest.cpp)

target_link_libraries(typetest PRIVATE utils Catch2::Catch2)

add_test(NAME testtypetest COMMAND typetest)

