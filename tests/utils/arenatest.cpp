#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <utils/arena.hpp>

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

    inline ref_counter& operator=(const ref_counter& other) noexcept
    {
        return *this;
    }

    inline ref_counter& operator=(ref_counter&& other) noexcept
    {
        return *this;
    }

    static inline int get() noexcept { return count; }

private:
    static inline int count = 0;
};

TEST_CASE("basic block arena check", "[arena][block_arena]")
{
    utils::block_arena<int> arena;
    usize i1 = arena.create(2);
    usize i2 = arena.create(7);
    usize i3 = arena.create(5);
    REQUIRE((arena.size() == 3 && "Hello"));
    REQUIRE(arena[i1] == 2);
    REQUIRE(arena[i2] == 7);
    REQUIRE(arena[i3] == 5);

    SECTION("constructor")
    {
        SECTION("default")
        {
            utils::block_arena<int> arena;
            REQUIRE(arena.capacity() == 0);
            REQUIRE(arena.size() == 0);
        }

        SECTION("capacity")
        {
            utils::block_arena<int> arena(20);
            REQUIRE(arena.capacity() >= 20);
            REQUIRE(arena.size() == 0);
        }

        SECTION("move")
        {
            utils::block_arena<int> arena2(::std::move(arena));
            REQUIRE(arena2.size() == 3);
            REQUIRE(arena2[i1] == 2);
            REQUIRE(arena2[i2] == 7);
            REQUIRE(arena2[i3] == 5);
        }

        SECTION("copy")
        {
            utils::block_arena<int> arena2(arena);
            REQUIRE(arena2.size() == 3);
            REQUIRE(arena2[i1] == 2);
            REQUIRE(arena2[i2] == 7);
            REQUIRE(arena2[i3] == 5);
        }
    }

    SECTION("assignment")
    {
        utils::block_arena<int> arena2;
        usize i4 = arena2.create(9);
        usize i5 = arena2.create(3);
        REQUIRE(arena2.size() == 2);
        REQUIRE(arena2[i4] == 9);
        REQUIRE(arena2[i5] == 3);

        SECTION("move")
        {
            arena2 = ::std::move(arena);
            REQUIRE(arena2.size() == 3);
            REQUIRE(arena2[i1] == 2);
            REQUIRE(arena2[i2] == 7);
            REQUIRE(arena2[i3] == 5);
            REQUIRE((!arena2.has(i4) || arena2[i4] != 9));
            REQUIRE((!arena2.has(i5) || arena2[i5] != 3));
        }

        SECTION("copy")
        {
            arena2 = arena;
            REQUIRE(arena2.size() == 3);
            REQUIRE(arena2[i1] == 2);
            REQUIRE(arena2[i2] == 7);
            REQUIRE(arena2[i3] == 5);
            REQUIRE((!arena2.has(i4) || arena2[i4] != 9));
            REQUIRE((!arena2.has(i5) || arena2[i5] != 3));
        }
    }

    SECTION("reserve")
    {
        arena.reserve(50);
        REQUIRE(arena.size() == 3);
        REQUIRE(arena.capacity() >= 50);
    }

    SECTION("modify")
    {
        SECTION("create")
        {
            usize i4 = arena.create(10);
            REQUIRE((arena.has(i4) && arena[i4] == 10));
        }

        SECTION("create unchecked")
        {
            arena.reserve(50);
            usize i4 = arena.create_unchecked(10);
            REQUIRE((arena.has(i4) && arena[i4] == 10));
        }

        SECTION("destroy")
        {
            REQUIRE(arena.has(i3));
            arena.destroy(i3);
            REQUIRE(!arena.has(i3));
            
            arena.destroy(2434);
            REQUIRE(!arena.has(2434));
        }

        SECTION("destroy unchecked")
        {
            REQUIRE(arena.has(i3));
            arena.destroy(i3);
            REQUIRE(!arena.has(i3));
        }

        SECTION("clear")
        {
            arena.clear();
            REQUIRE(!arena.has(i1));
            REQUIRE(!arena.has(i2));
            REQUIRE(!arena.has(i3));
        }
    }

    SECTION("lifetime")
    {
        utils::block_arena<ref_counter> arena;
        REQUIRE(ref_counter::get() == 0);

        usize i1 = arena.create();
        usize i2 = arena.create();
        usize i3 = arena.create();
        REQUIRE(ref_counter::get() == 3);

        arena.destroy(i3);
        REQUIRE(ref_counter::get() == 2);

        arena.destroy(i2);
        REQUIRE(ref_counter::get() == 1);

        i2 = arena.create_unchecked();
        REQUIRE(ref_counter::get() == 2);

        arena.destroy_unchecked(i1);
        REQUIRE(ref_counter::get() == 1);

        arena.reserve(100);
        REQUIRE(ref_counter::get() == 1);

        arena = utils::block_arena<ref_counter>();
        REQUIRE(ref_counter::get() == 0);

        i1 = arena.create();
        i2 = arena.create();
        i3 = arena.create();

        utils::block_arena<ref_counter> copy(arena);
        REQUIRE(ref_counter::get() == 6);

        copy.clear();
        REQUIRE(ref_counter::get() == 3);
    }
}

