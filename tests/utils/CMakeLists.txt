add_executable(typetest typetest.cpp)
target_link_libraries(typetest PRIVATE utils Catch2::Catch2WithMain)
add_test(NAME testtypetest COMMAND typetest)

add_executable(buffertest buffertest.cpp)
target_link_libraries(buffertest PRIVATE utils Catch2::Catch2WithMain)
add_test(NAME testbuffertest COMMAND buffertest)

add_executable(arraytest arraytest.cpp)
target_link_libraries(arraytest PRIVATE utils Catch2::Catch2WithMain)
add_test(NAME testarraytest COMMAND arraytest)

