#include <catch2/catch_test_macros.hpp>
#include <utils/type.hpp>

TEST_CASE( "type.hpp check", "[type]" )
{
    REQUIRE(sizeof(byte) == 1);

    REQUIRE(sizeof(u8) == 1);
    REQUIRE(sizeof(u16) == 2);
    REQUIRE(sizeof(u32) == 4);
    REQUIRE(sizeof(u64) == 8);
    
    REQUIRE(sizeof(i8) == 1);
    REQUIRE(sizeof(i16) == 2);
    REQUIRE(sizeof(i32) == 4);
    REQUIRE(sizeof(i64) == 8);

    REQUIRE(sizeof(f32) == 4);
    REQUIRE(sizeof(f64) == 8);
}

