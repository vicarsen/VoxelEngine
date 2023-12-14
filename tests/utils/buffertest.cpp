#include <catch2/catch_test_macros.hpp>
#include <utils/buffer.hpp>

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

TEST_CASE("basic buffer check", "[buffer]")
{
    utils::buffer<int> buff(10);
    REQUIRE(buff.data() != nullptr);
    REQUIRE(buff.size() == 10);
    buff[7] = 5;
    
    SECTION("constructor")
    {
        SECTION("default")
        {
            utils::buffer<int> buff;
            REQUIRE(buff.size() == 0);
        }

        SECTION("size")
        {
            utils::buffer<int> buff(10);
            REQUIRE(buff.data() != nullptr);
            REQUIRE(buff.size() == 10);

            buff[7] = 5;
            REQUIRE(buff[7] == 5);
        }

        SECTION("copy")
        {
            utils::buffer<int> buff2(buff);
            REQUIRE(buff2.data() != nullptr);
            REQUIRE(buff2.size() == 10);
            REQUIRE(buff2[7] == 5);
        }

        SECTION("move")
        {
            utils::buffer<int> buff2(::std::move(buff));
            REQUIRE(buff2.data() != nullptr);
            REQUIRE(buff2.size() == 10);
            REQUIRE(buff2[7] == 5);
        }
    }

    SECTION("assignment")
    {
        SECTION("move")
        {
            utils::buffer<int> buff2(5);
            REQUIRE(buff2.data() != nullptr);
            REQUIRE(buff2.size() == 5);

            buff2 = std::move(buff);
            REQUIRE(buff2.data() != nullptr);
            REQUIRE(buff2.size() == 10);
            REQUIRE(buff2[7] == 5);
        }

        SECTION("copy")
        {
            utils::buffer<int> buff2(5);
            REQUIRE(buff2.data() != nullptr);
            REQUIRE(buff2.size() == 5);

            buff2 = buff;
            REQUIRE(buff2.data() != nullptr);
            REQUIRE(buff2.size() == 10);
            REQUIRE(buff2[7] == 5);
        }
    }

    SECTION("resize")
    {
        REQUIRE(buff.size() == 10);
        buff.resize(20);
        REQUIRE(buff.size() == 20);
        REQUIRE(buff[7] == 5);
    }
}

