#include <catch2/catch_test_macros.hpp>
#include <utils/type.hpp>

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

    inline ref_counter& operator=(ref_counter&& other) noexcept
    {
        return *this;
    }

    inline ref_counter& operator=(const ref_counter& other) noexcept
    {
        return *this;
    }

    static inline int get() noexcept { return count; }

private:
    static inline int count = 0;
};

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

TEST_CASE("allocator check", "[type]")
{
    int *v;

    v = utils::basic_allocator<int>::allocate();
    REQUIRE(v != nullptr);
    utils::basic_allocator<int>::deallocate(v);

    v = utils::basic_allocator<int>::allocate(1'000'000);
    REQUIRE(v != nullptr);

    v = utils::basic_allocator<int>::reallocate(v, 10'000'000);
    REQUIRE(v != nullptr);

    utils::basic_allocator<int>::deallocate(v);

    v = utils::basic_allocator<int>::allocate();

    utils::basic_allocator<int>::construct_at(v, 10);
    REQUIRE(*v == 10);

    utils::basic_allocator<int>::deallocate(v);

    ref_counter *counter;
    counter = utils::basic_allocator<ref_counter>::allocate();
    REQUIRE(ref_counter::get() == 0);

    utils::basic_allocator<ref_counter>::construct_at(counter);
    REQUIRE(ref_counter::get() == 1);

    utils::basic_allocator<ref_counter>::destruct_at(counter);
    REQUIRE(ref_counter::get() == 0);

    utils::basic_allocator<ref_counter>::deallocate(counter);
    REQUIRE(ref_counter::get() == 0);

    counter = utils::basic_allocator<ref_counter>::allocate();
    utils::basic_allocator<ref_counter>::construct_at(counter);
    utils::basic_allocator<ref_counter>::deallocate(counter);
    REQUIRE(ref_counter::get() == 1);
}

