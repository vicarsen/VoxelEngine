#include <catch2/catch_test_macros.hpp>
#include <utils/type.hpp>

TEST_CASE("basic type check", "[type]")
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

    REQUIRE(utils::relocatable<u8> == true);
    REQUIRE(utils::relocatable<f32> == true);

    REQUIRE(utils::trivially_copyable<i8> == true);
    REQUIRE(utils::trivially_copyable<i32> == true);

    REQUIRE(utils::trivially_destructible<f64> == true);
    REQUIRE(utils::trivially_destructible<u64> == true);
}

class ref_counter
{
public:
    inline ref_counter() noexcept
    {
        count++;
    }

    inline ref_counter(const ref_counter& other) noexcept
    {
        count++;
    }

    inline ref_counter(ref_counter&& other) noexcept
    {
        count++;
    }

    inline ~ref_counter() noexcept
    {
        count--;
    }

    static inline int get() noexcept { return count; }

private:
    static inline int count = 0;
};

TEST_CASE("allocator check", "[type]")
{
    int *v;

    v = utils::allocator<int>::allocate();
    REQUIRE(v != nullptr);
    utils::allocator<int>::deallocate(v);

    v = utils::allocator<int>::allocate(1'000'000);
    REQUIRE(v != nullptr);
    
    v = utils::allocator<int>::reallocate(v, 10'000'000);
    REQUIRE(v != nullptr);

    utils::allocator<int>::deallocate(v);

    v = utils::allocator<int>::allocate();

    utils::allocator<int>::construct_at(v, 10);
    REQUIRE(*v == 10);

    utils::allocator<int>::deallocate(v);

    ref_counter *counter;
    counter = utils::allocator<ref_counter>::allocate();
    REQUIRE(ref_counter::get() == 0);

    utils::allocator<ref_counter>::construct_at(counter);
    REQUIRE(ref_counter::get() == 1);

    utils::allocator<ref_counter>::destruct_at(counter);
    REQUIRE(ref_counter::get() == 0);

    utils::allocator<ref_counter>::deallocate(counter);
    REQUIRE(ref_counter::get() == 0);

    counter = utils::allocator<ref_counter>::allocate();
    utils::allocator<ref_counter>::construct_at(counter);
    utils::allocator<ref_counter>::deallocate(counter);
    REQUIRE(ref_counter::get() == 1);
}

