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

    static inline int get() noexcept { return count; }

private:
    static inline int count = 0;
};

TEST_CASE("basic buffer check", "[buffer]")
{
    utils::buffer<int> b1;
    b1.resize(10);

    REQUIRE(b1.size() == 10);
    REQUIRE(b1.data() != nullptr);

    *(b1.data() + 5) = 20;
    REQUIRE(b1[5] == 20);

    utils::buffer<int> b2(b1);

    REQUIRE(b1.size() == b2.size());
    REQUIRE(b1.data() != nullptr);
    REQUIRE(b1.data() != nullptr);
    REQUIRE(b1.data() != b2.data());
    REQUIRE(b1[5] == b2[5]);

    b2 = std::move(utils::buffer<int>());
    REQUIRE(b2.data() == nullptr);
    REQUIRE(b2.size() == 0);

    utils::buffer<ref_counter> rbuff;
    REQUIRE(ref_counter::get() == 0);

    rbuff.resize(50);
    REQUIRE(ref_counter::get() == 0);
}

