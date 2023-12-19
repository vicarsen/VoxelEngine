#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <utils/array.hpp>

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

TEMPLATE_TEST_CASE("basic span check", "[array][span]", utils::span<int>, utils::const_span<int>)
{
    int buff[] = { 3, 6, 10, 2, 7, 19, 23, 47, 98, 1 };
    const int buff_size = sizeof(buff) / sizeof(buff[0]);

    SECTION("constructors")
    {
        SECTION("default")
        {
            TestType span;
            REQUIRE(span.size() == 0);
            REQUIRE(span.empty());
            REQUIRE(span.data() == nullptr);
        }

        SECTION("cstyle range")
        {
            TestType span(buff + 3, buff + 7);
            REQUIRE(span.size() == 4);
            REQUIRE(!span.empty());
            REQUIRE(span.data() == buff + 3);
            REQUIRE(span[0] == buff[3]);
            REQUIRE(span[1] == buff[4]);
            REQUIRE(span[2] == buff[5]);
            REQUIRE(span[3] == buff[6]);
        }

        SECTION("cstyle array")
        {
            TestType span(buff + 3, 4);
            REQUIRE(span.size() == 4);
            REQUIRE(!span.empty());
            REQUIRE(span.data() == buff + 3);
            REQUIRE(span[0] == buff[3]);
            REQUIRE(span[1] == buff[4]);
            REQUIRE(span[2] == buff[5]);
            REQUIRE(span[3] == buff[6]);
        }

        SECTION("move")
        {
            TestType span(buff + 3, buff + 7);
            TestType span2(::std::move(span));
            REQUIRE(span2.size() == 4);
            REQUIRE(!span2.empty());
            REQUIRE(span2.data() == buff + 3);
            REQUIRE(span2[0] == buff[3]);
            REQUIRE(span2[1] == buff[4]);
            REQUIRE(span2[2] == buff[5]);
            REQUIRE(span2[3] == buff[6]);
        }

        SECTION("copy")
        {
            TestType span(buff + 3, buff + 7);
            TestType span2(span);
            REQUIRE(span2.size() == 4);
            REQUIRE(!span2.empty());
            REQUIRE(span2.data() == buff + 3);
            REQUIRE(span2[0] == buff[3]);
            REQUIRE(span2[1] == buff[4]);
            REQUIRE(span2[2] == buff[5]);
            REQUIRE(span2[3] == buff[6]);
        }
    }

    SECTION("assignment")
    {
        TestType source(buff + 2, buff + 6);

        SECTION("move")
        {
            TestType span(buff + 1, buff + 3);
            REQUIRE(span.size() == 2);
            REQUIRE(span[1] == buff[2]);

            span = ::std::move(source);
            REQUIRE(span.size() == 4);
            REQUIRE(span[1] == buff[3]);
            REQUIRE(span[3] == buff[5]);
        }

        SECTION("copy")
        {
            TestType span(buff + 1, buff + 3);
            REQUIRE(span.size() == 2);
            REQUIRE(span[1] == buff[2]);

            span = source;
            REQUIRE(span.size() == 4);
            REQUIRE(span[1] == buff[3]);
            REQUIRE(span[3] == buff[5]);
        }
    }

    SECTION("modify")
    {
        TestType span(buff, buff_size);
        REQUIRE(span.size() == buff_size);
        REQUIRE(span.data() == buff);

        REQUIRE(span.pop_front() == buff[0]);
        REQUIRE(span.size() == buff_size - 1);
        REQUIRE(span.front() == buff[1]);

        REQUIRE(span.pop_back() == buff[buff_size - 1]);
        REQUIRE(span.size() == buff_size - 2);
        REQUIRE(span.back() == buff[buff_size - 2]);

        span.pop_front(2);
        REQUIRE(span.front() == buff[3]);

        span.pop_back(3);
        REQUIRE(span.back() == buff[buff_size - 5]);
    }

    SECTION("subspans")
    {
        TestType span(buff, buff_size);
        REQUIRE(span.size() == buff_size);
        REQUIRE(span.data() == buff);

        SECTION("subspan")
        {
            auto subspan = span.subspan(3, 2);
            REQUIRE(subspan.size() == 2);
            REQUIRE(subspan.data() == buff + 3);
            REQUIRE(subspan[0] == buff[3]);
            REQUIRE(subspan[1] == buff[4]);
        }

        SECTION("prefix")
        {
            auto prefix = span.prefix(3);
            REQUIRE(prefix.size() == 3);
            REQUIRE(prefix.data() == buff);
            REQUIRE(prefix[2] == buff[2]);
        }

        SECTION("suffix")
        {
            auto suffix = span.suffix(4);
            REQUIRE(suffix.size() == 4);
            REQUIRE(suffix.data() == buff + buff_size - 4);
            REQUIRE(suffix[1] == buff[buff_size - 4 + 1]);
        }
    }
}

TEST_CASE("basic array check", "[array]")
{
    SECTION("constructors")
    {
        SECTION("default")
        {
            utils::array<int> arr;
            REQUIRE(arr.size() == 0);
            REQUIRE(arr.empty());
        }

        SECTION("move")
        {
            utils::array<int> arr;
            arr.push_many(5, 10);

            utils::array<int> arr2(::std::move(arr));
            REQUIRE(arr2.size() == 10);
            REQUIRE(arr2.capacity() >= 10);
            REQUIRE(arr2[4] == 5);
        }

        SECTION("copy")
        {
            utils::array<int> arr;
            arr.push_many(5, 10);

            utils::array<int> arr2(arr);
            REQUIRE(arr.size() == arr2.size());
            REQUIRE(arr[3] == arr2[3]);
        }

        SECTION("initializer list")
        {
            utils::array<int> arr({ 2, 3, 4, 1 });
            REQUIRE(arr.size() == 4);
            REQUIRE(arr.capacity() >= 4);
            REQUIRE(arr[0] == 2);
            REQUIRE(arr[1] == 3);
            REQUIRE(arr[2] == 4);
            REQUIRE(arr[3] == 1);
        }

        SECTION("reserve")
        {
            utils::array<int> arr(20);
            REQUIRE(arr.size() == 0);
            REQUIRE(arr.capacity() >= 20);
        }

        SECTION("cstyle buffer")
        {
            const int buff[] = { 1, 6, 4, 22 };
            const int buff_size = sizeof(buff) / sizeof(buff[0]);
            
            utils::array<int> arr(buff, buff + buff_size);
            REQUIRE(arr.size() == buff_size);
            REQUIRE(arr.capacity() >= buff_size);
            REQUIRE(arr[0] == buff[0]);
            REQUIRE(arr[1] == buff[1]);
            REQUIRE(arr[2] == buff[2]);
            REQUIRE(arr[3] == buff[3]);
        }

        SECTION("cstyle buffer with length")
        {
            const int buff[] = { 1, 6, 4, 22 };
            const int buff_size = sizeof(buff) / sizeof(buff[0]);

            utils::array<int> arr(buff, buff_size);
            REQUIRE(arr.size() == buff_size);
            REQUIRE(arr.capacity() >= buff_size);
            REQUIRE(arr[0] == buff[0]);
            REQUIRE(arr[1] == buff[1]);
            REQUIRE(arr[2] == buff[2]);
            REQUIRE(arr[3] == buff[3]);
        }
    }

    SECTION("assignment")
    {
        SECTION("move")
        {
            utils::array<int> arr({ 1, 2, 3, 4, 5 });
            utils::array<int> arr2({ 4, 3, 2, 1 });

            arr2 = ::std::move(arr);
            REQUIRE(arr2.size() == 5);
            REQUIRE(arr2[2] == 3);
        }

        SECTION("copy")
        {
            utils::array<int> arr({ 1, 2, 3, 4, 5 });
            utils::array<int> arr2({ 4, 3, 2, 1 });

            arr2 = arr;
            REQUIRE(arr2.size() == 5);
            REQUIRE(arr2[2] == 3);
        }

        SECTION("initializer list")
        {
            utils::array<int> arr({ 1, 2, 3, 4, 5 });
            arr = { 4, 3, 2, 1 };

            REQUIRE(arr.size() == 4);
            REQUIRE(arr[2] == 2);
        }
    }

    SECTION("reserve")
    {
        utils::array<int> arr;

        SECTION("reserve increases capacity")
        {
            arr.reserve(5);
            REQUIRE(arr.capacity() >= 5);
            REQUIRE(arr.size() == 0);

            arr.reserve(3);
            REQUIRE(arr.capacity() >= 3);
            REQUIRE(arr.size() == 0);

            arr.reserve(20);
            REQUIRE(arr.capacity() >= 20);
            REQUIRE(arr.size() == 0);
        }

        SECTION("reverse exactly sets capacity")
        {
            arr.reserve_exactly(5);
            REQUIRE(arr.capacity() == 5);
            REQUIRE(arr.size() == 0);

            arr.reserve_exactly(3);
            REQUIRE(arr.capacity() == 3);
            REQUIRE(arr.size() == 0);

            arr.reserve_exactly(20);
            REQUIRE(arr.capacity() == 20);
            REQUIRE(arr.size() == 0);
        }

        SECTION("shrink to fit")
        {
            arr.reserve(50);
            REQUIRE(arr.capacity() >= 50);
            REQUIRE(arr.size() == 0);

            arr.push_many(10, 10);
            REQUIRE(arr.size() == 10);

            arr.shrink_to_fit();
            REQUIRE(arr.capacity() == 10);
            REQUIRE(arr.size() == 10);
        }
    }

    SECTION("modify")
    {
        utils::array<int> arr;

        SECTION("checked")
        {
            arr.push(10);
            REQUIRE(arr.size() == 1);
            REQUIRE(arr.capacity() >= 1);
            REQUIRE(arr.back() == 10);

            arr.push_many(3, 7);
            REQUIRE(arr.size() == 8);
            REQUIRE(arr.capacity() >= 8);
            REQUIRE(arr.front() == 10);
            REQUIRE(arr.back() == 3);
            REQUIRE(arr[5] == 3);

            arr[4] = 9;
            REQUIRE(arr[4] == 9);
        
            arr.pop();
            REQUIRE(arr.size() == 7);
            REQUIRE(arr.back() == 3);
            REQUIRE(arr.front() == 10);
            
            arr.pop_many(6);
            REQUIRE(arr.size() == 1);
            REQUIRE(arr.back() == 10);
        }

        SECTION("unchecked")
        {
            arr.reserve(20);
            REQUIRE(arr.size() == 0);
            REQUIRE(arr.capacity() >= 20);

            arr.push_unchecked(10);
            REQUIRE(arr.size() == 1);
            REQUIRE(arr.back() == 10);

            arr.push_many_unchecked(3, 7);
            REQUIRE(arr.size() == 8);
            REQUIRE(arr.capacity() >= 8);
            REQUIRE(arr.front() == 10);
            REQUIRE(arr.back() == 3);
            REQUIRE(arr[5] == 3);

            arr[4] = 9;
            REQUIRE(arr[4] == 9);
        
            arr.pop();
            REQUIRE(arr.size() == 7);
            REQUIRE(arr.back() == 3);
            REQUIRE(arr.front() == 10);
            
            arr.pop_many(6);
            REQUIRE(arr.size() == 1);
            REQUIRE(arr.back() == 10);
        }

        SECTION("erase")
        {
            arr.push_many(10, 20);
            arr[9] = 9;
            arr[10] = 10;
            arr[11] = 11;
            
            arr.erase_unordered(arr.begin() + 10);
            REQUIRE(arr[9] == 9);
            REQUIRE(arr[10] == 10);
            REQUIRE(arr[11] == 11);
            REQUIRE(arr.size() == 19);
        }

        SECTION("clear")
        {
            arr.push_many(10, 20);
            REQUIRE(arr.size() == 20);

            arr.clear();
            REQUIRE(arr.size() == 0);
            arr.clear();
            REQUIRE(arr.size() == 0);
        }
    }

    SECTION("iteration")
    {
        const int reference[] = { 10, 5, 3, 9, 20, 11, 39, 43, 26, 90 };
        const int ref_size = sizeof(reference) / sizeof(reference[0]);

        utils::array<int> arr(reference, ref_size);
        REQUIRE(arr.size() == ref_size);

        SECTION("cstyle")
        {
            for(usize i = 0; i < arr.size(); i++)
            {
                REQUIRE(arr[i] == reference[i]);
            }
        }

        SECTION("cstyle reverse")
        {
            for(usize i = arr.size(); i != 0; i--)
            {
                REQUIRE(arr[i - 1] == reference[i - 1]);
            }
        }

        SECTION("iterator")
        {
            for(auto it = arr.begin(); it != arr.end(); it++)
            {
                REQUIRE(*it == reference[it - arr.begin()]);
            }
        }

        SECTION("const iterator")
        {
            for(auto it = arr.cbegin(); it != arr.cend(); it++)
            {
                REQUIRE(*it == reference[it - arr.cbegin()]);
            }
        }

        SECTION("reverse iterator")
        {
            for(auto it = arr.rbegin(); it != arr.rend(); it++)
            {
                REQUIRE(*it == reference[ref_size - 1 - (it - arr.rbegin())]);
            }
        }

        SECTION("const reverse iterator")
        {
            for(auto it = arr.crbegin(); it != arr.crend(); it++)
            {
                REQUIRE(*it == reference[ref_size - 1 - (it - arr.crbegin())]);
            }
        }

        SECTION("range for")
        {
            int i = 0;
            for(int& x : arr)
            {
                REQUIRE(x == reference[i]);
                i++;
            }
        }

        SECTION("const range for")
        {
            int i = 0;
            for(const int& x : arr)
            {
                REQUIRE(x == reference[i]);
                i++;
            }
        }
    }

    SECTION("raw")
    {
        const int buff[] = { 3, 5, 1, 2, 9, 7 };
        const int buff_size = sizeof(buff) / sizeof(buff[0]);

        utils::array<int> arr(buff, buff_size);
        
        const int* data = arr.data();
        for(usize i = 0; i < buff_size; i++)
        {
            REQUIRE(data[i] == buff[i]);
        }
    }

    SECTION("lifetime")
    {
        utils::array<ref_counter> arr;
        REQUIRE(ref_counter::get() == 0);
        
        arr.push_many(ref_counter(), 20);
        REQUIRE(ref_counter::get() == 20);

        arr.push();
        REQUIRE(ref_counter::get() == 21);

        arr.pop();
        REQUIRE(ref_counter::get() == 20);

        arr.pop_many(10);
        REQUIRE(ref_counter::get() == 10);

        arr.clear();
        REQUIRE(ref_counter::get() == 0);

        arr.reserve(100);
        REQUIRE(ref_counter::get() == 0);

        arr.push_unchecked();
        REQUIRE(ref_counter::get() == 1);

        arr.push_many_unchecked(ref_counter(), 20);
        REQUIRE(ref_counter::get() == 21);

        arr.erase_unordered(arr.begin() + 5);
        REQUIRE(ref_counter::get() == 20);

        arr = utils::array<ref_counter>();
        REQUIRE(ref_counter::get() == 0);

        arr.push_many(ref_counter(), 20);
        utils::array<ref_counter> copy(arr);
        REQUIRE(ref_counter::get() == 40);
    }
}

TEMPLATE_TEST_CASE("array-span test", "[array][span]", utils::span<int>, utils::const_span<int>)
{
    utils::array<int> arr({ 8, 10, 3, 7, 5, 9, 11, 23, 72 });
    TestType span(arr.begin() + 3, arr.begin() + 9);

    SECTION("array from span")
    {
        SECTION("constructor")
        {
            utils::array<int> arr(span);
            REQUIRE(arr.size() == span.size());
            REQUIRE(arr[1] == span[1]);
            REQUIRE(arr[3] == span[3]);
        }

        SECTION("assignement")
        {
            utils::array<int> arr({ 9, 3, 5, 2 });
            REQUIRE(arr.size() == 4);
            REQUIRE(arr[3] == 2);

            arr = span;
            REQUIRE(arr.size() == span.size());
            REQUIRE(arr[1] == span[1]);
            REQUIRE(arr[3] == span[3]);
        }

        SECTION("push")
        {
            utils::array<int> arr({ 3, 9, 7 });
            REQUIRE(arr.size() == 3);

            arr.push_many(span);
            REQUIRE(arr.size() == 9);
            REQUIRE(arr.back() == span.back());
            REQUIRE(arr[7] == span[4]);

            arr.push_many(span);
            REQUIRE(arr.size() == 15);
            REQUIRE(arr.back() == span.back());
        }

        SECTION("push unchecked")
        {
            utils::array<int> arr({ 3, 9, 7 });
            REQUIRE(arr.size() == 3);

            arr.reserve(20);
            arr.push_many_unchecked(span);
            REQUIRE(arr.size() == 9);
            REQUIRE(arr.back() == span.back());
            REQUIRE(arr[7] == span[4]);

            arr.push_many_unchecked(span);
            REQUIRE(arr.size() == 15);
            REQUIRE(arr.back() == span.back());
        }
    }

    SECTION("span from array")
    {
        SECTION("convert")
        {
            TestType span = arr;
            REQUIRE(span.size() == arr.size());
            REQUIRE(span.data() == arr.data());
            REQUIRE(span.back() == arr.back());
        }

        SECTION("subarray")
        {
            TestType span = arr.subarray(3, 5);
            REQUIRE(span.size() == 5);
            REQUIRE(span.data() == arr.begin() + 3);
            REQUIRE(span[2] == arr[5]);
        }

        SECTION("prefix")
        {
            TestType span = arr.prefix(3);
            REQUIRE(span.size() == 3);
            REQUIRE(span.data() == arr.begin());
            REQUIRE(span[2] == arr[2]);
        }

        SECTION("suffix")
        {
            TestType span = arr.suffix(4);
            REQUIRE(span.size() == 4);
            REQUIRE(span.data() == arr.end() - 4);
            REQUIRE(span[1] == arr[arr.size() - 4 + 1]);
        }
    }
}

TEST_CASE("basic sparse array check", "[array][sparse-array]")
{
    utils::sparse_array<int> arr;
    arr.insert(3, 7);
    REQUIRE(arr.capacity() > 3);
    REQUIRE(arr.has(3));
    REQUIRE(arr.get(3) == 7);

    SECTION("constructor")
    {
        SECTION("default")
        {
            utils::sparse_array<int> arr;
            REQUIRE(arr.capacity() == 0);
        }


        SECTION("move")
        {
            utils::sparse_array<int> arr2(::std::move(arr));
            REQUIRE(arr2.capacity() > 3);
            REQUIRE(arr2.has(3));
            REQUIRE(arr2.get(3) == 7);
        }

        SECTION("copy")
        {
            utils::sparse_array<int> arr2(arr);
            REQUIRE(arr2.capacity() > 3);
            REQUIRE(arr2.has(3));
            REQUIRE(arr2.get(3) == 7);
        }
    }

    SECTION("assignment")
    {
        SECTION("move")
        {
            utils::sparse_array<int> arr2;
            arr2.insert(2, 3);
            REQUIRE(arr2.capacity() > 2);
            REQUIRE(arr2.has(2));
            REQUIRE(arr2.get(2) == 3);

            arr2 = ::std::move(arr);
            REQUIRE(arr2.capacity() > 3);
            REQUIRE(arr2.has(3));
            REQUIRE(arr2.get(3) == 7);
            REQUIRE((!arr2.has(2)));
        }

        SECTION("copy")
        {
            utils::sparse_array<int> arr2;
            arr2.insert(2, 3);
            REQUIRE(arr2.capacity() > 2);
            REQUIRE(arr2.has(2));
            REQUIRE(arr2.get(2) == 3);

            arr2 = arr;
            REQUIRE(arr2.capacity() > 3);
            REQUIRE(arr2.has(3));
            REQUIRE(arr2.get(3) == 7);
            REQUIRE(!arr2.has(2));
        }
    }

    SECTION("reserve")
    {
        utils::sparse_array<int> arr;
        REQUIRE(arr.capacity() == 0);

        arr.reserve(50);
        REQUIRE(arr.capacity() >= 50);

        arr.reserve(20);
        REQUIRE(arr.capacity() >= 50);
    }

    SECTION("modify")
    {
        SECTION("insert")
        {
            utils::sparse_array<int> arr;
            arr.insert(4, 5);
            REQUIRE(arr.has(4));
            REQUIRE(arr.get(4) == 5);

            arr.insert(4, 3);
            REQUIRE(arr.has(4));
            REQUIRE(arr.get(4) == 3);
        }

        SECTION("insert unchecked")
        {
            utils::sparse_array<int> arr;
            arr.reserve(20);

            arr.insert_unchecked(5, 3);
            REQUIRE(arr.has(5));
            REQUIRE(arr.get(5) == 3);

            arr.insert_unchecked(5, 2);
            REQUIRE(arr.has(5));
            REQUIRE(arr.get(5) == 2);
        }

        SECTION("erase")
        {
            utils::sparse_array<int> arr;
            arr.reserve(20);
            arr.insert_unchecked(2, 10);
            arr.insert_unchecked(5, 20);
            arr.erase(2);
            arr.erase(7);
            arr.erase(21);

            REQUIRE((!arr.has(2) && arr.has(5) && !arr.has(7) && !arr.has(21)));
        }

        SECTION("erase unchecked")
        {
            utils::sparse_array<int> arr;
            arr.reserve(20);
            arr.insert_unchecked(2, 10);
            arr.insert_unchecked(5, 20);
            arr.erase(2);

            REQUIRE((!arr.has(2) && arr.has(5)));
        }

        SECTION("clear")
        {
            utils::sparse_array<int> arr;
            arr.reserve(50);
            arr.insert_unchecked(2, 5);
            arr.insert_unchecked(5, 8);
            arr.insert_unchecked(30, 7);

            REQUIRE((arr.has(2) && arr.get(2) == 5));
            REQUIRE((arr.has(5) && arr.get(5) == 8));
            REQUIRE((arr.has(30) && arr.get(30) == 7));

            arr.clear();

            REQUIRE(!arr.has(2));
            REQUIRE(!arr.has(5));
            REQUIRE(!arr.has(30));
            REQUIRE(arr.capacity() >= 50);
        }

        SECTION("get or insert")
        {
            utils::sparse_array<int> arr;
            arr.reserve(30);
            arr.insert_unchecked(20, 56);

            REQUIRE((arr.has(20) && arr.get(20) == 56));
            REQUIRE(arr.get_or_insert(19, 3) == 3);
            REQUIRE((arr.has(19) && arr.get(19) == 3));

            REQUIRE(arr.get_or_insert(20, 9) == 56);
            REQUIRE(arr.get(20) == 56);
        }
    }

    SECTION("access")
    {
        SECTION("get or")
        {
            utils::sparse_array<int> arr;
            arr.reserve(50);
            arr.insert_unchecked(2, 50);
            arr.insert_unchecked(6, 10);
            arr.insert_unchecked(10, 9);

            REQUIRE(arr.get_or(2, 9) == 50);
            REQUIRE(arr.get_or(6, 1) == 10);
            REQUIRE(arr.get_or(10, 4) == 9);

            REQUIRE(arr.get_or(1, 9) == 9);
            REQUIRE(arr.get_or(1, 8) == 8);
            REQUIRE(!arr.has(1));

            REQUIRE(arr.get_or(19, 7) == 7);
            REQUIRE(!arr.has(19));
        }

        SECTION("has")
        {
            utils::sparse_array<int> arr;
            arr.reserve(30);
            arr.insert_unchecked(9, 10);
            arr.insert_unchecked(20, 7);

            REQUIRE((arr.has(9) && arr.has(20)));
            REQUIRE((!arr.has(2) && !arr.has(7) && !arr.has(64)));
        }
    }
}

