/**
 * @file
 * @brief Basic buffer structures.
 */
#pragma once

#include "type.hpp"

#include <cstring>

namespace utils
{
    /// @brief A buffer of objects.
    /// @tparam type The type of the objects contained.
    /// @tparam allocator The type of the allocator to use for allocating the memory.
    ///
    /// A buffer that manages allocation and deallocation of a contiguous
    /// array of objects of some type in a RAII style. Used as the base
    /// structure for most containers.
    template<typename type, typename allocator = allocator<type>>
    class buffer
    {
    public:
        /** Type of an element of the buffer. */
        typedef type value_type;
        /** Type of the buffer. */
        typedef buffer<type> buffer_type;
        /** Type of the allocator of the buffer. */
        typedef allocator allocator_type;

        /// @brief Constructs an empty buffer.
        inline buffer() noexcept :
            start(nullptr), finish(nullptr)
        {
        }

        /// @brief Constructs a new buffer identical to other.
        /// @param other The buffer to copy.
        ///
        /// Only copies the data in the given buffer. No constructors or copy
        /// operators are called.
        inline buffer(const buffer_type& other) noexcept
        {
            usize s = other.finish - other.start;

            start  = allocator_type::allocate(s);
            finish = start + s;

            ::std::memcpy(start, other.start, s * sizeof(value_type));
        }

        /// @brief Moves other into this buffer.
        /// @param other The buffer to move.
        ///
        /// After the constructor finishes, other will have no ownership
        /// of the space it owned before, and the newly constructed buffer
        /// keeps the ownership.
        inline buffer(buffer_type&& other) noexcept :
            start(other.start), finish(other.finish)
        {
            other.start  = nullptr;
            other.finish = nullptr;
        }

        /// @brief Constructs a new buffer of given size.
        /// @param s The size of the newly constructed buffer.
        ///
        /// Creates a new buffer that can contain s objects of type value_type,
        /// without calling any constructors, just allocating the memory, and
        /// taking ownership of it.
        inline buffer(usize s) noexcept
        {
            start  = allocator_type::allocate(s);
            finish = start + s;
        }

        /// @brief Destructs the buffer, freeing its memory.
        ///
        /// If the buffer owns any memory at the time of destruction, it
        /// frees it, ensuring no memory leaks arise.
        inline ~buffer() noexcept
        {
            allocator_type::deallocate(start);
        }

        /// @brief Copies a buffer.
        /// @param other The buffer to copy.
        ///
        /// Only copies the data in the given buffer. No constructors or copy
        /// operators are called.
        inline buffer_type& operator=(const buffer_type& other) noexcept
        {
            usize s = other.finish - other.start;

            start  = allocator_type::reallocate(start, s);
            finish = start + s;

            ::std::memcpy(start, other.start, s * sizeof(value_type));

            return *this;
        }

        /// @brief Moves other into this buffer.
        /// @param other The buffer to move.
        ///
        /// After the constructor finishes, other will have no ownership
        /// of the space it owned before, and this buffer keeps ownership
        /// of it. The old space owned by this buffer is deallocated.
        inline buffer_type& operator=(buffer_type&& other) noexcept
        {
            allocator_type::deallocate(start);

            start  =  other.start; other.start  = nullptr;
            finish = other.finish; other.finish = nullptr;
            return *this;
        }

        /// @brief Resizes the buffer.
        /// @param s The new size of the buffer.
        ///
        /// Changes the size of the buffer to hold s objects of type
        /// value_type, discarding the data in the old buffer in the
        /// process.
        inline void resize(usize s) noexcept
        {
            start  = allocator_type::reallocate(start, s);
            finish = start + s;
        }

        /// @brief Access an element of the buffer.
        /// @param i The index of the element to access.
        inline value_type& operator[](usize i) noexcept { return start[i]; }

        /// @brief Access an element of the buffer.
        /// @param i The index of the element to access.
        inline const value_type& operator[](usize i) const noexcept { return start[i]; }

        /// @brief Get the buffer length.
        /// @return The buffer's length.
        ///
        /// Returns the length of the buffer as the number of objects
        /// of type value_type that it can contain.
        inline usize size() const noexcept { return finish - start; }

        /// @brief Get a pointer to the contiguous array of data owned by the buffer.
        /// @return A pointer to the array owned by the buffer.
        inline value_type* data() noexcept { return start; }
        /// @brief Get a pointer to the contiguous array of data owned by the buffer.
        /// @return A pointer to the array owned by the buffer.
        inline const value_type* data() const noexcept { return start; }

        /// @brief Get a pointer (iterator) to the beginning of the data owned by the buffer.
        /// @return A pointer (iterator) to the beginning of the array owned by the buffer.
        inline value_type* begin() noexcept { return start; }
        /// @brief Get a pointer (iterator) to the beginning of the data owned by the buffer.
        /// @return A pointer (iterator) to the beginning of the array owned by the buffer.
        inline const value_type* begin() const noexcept { return start; }

        /// @brief Get a pointer (iterator) to the end of the data owned by the buffer.
        /// @return A pointer (iterator) to the end of the array owned by the buffer.
        inline value_type* end() noexcept { return finish; }
        /// @brief Get a pointer (iterator) to the end of the data owned by the buffer.
        /// @return A pointer (iterator) to the end of the array owned by the buffer.
        inline const value_type* end() const noexcept { return finish; }

    private:
        value_type* start;
        value_type* finish;
    };
};

