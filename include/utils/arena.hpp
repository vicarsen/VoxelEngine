#pragma once

#include "array.hpp"

namespace utils
{
    /// @brief An arena specialized in allocating a single element of some type at a time.
    /// @tparam type The type managed by the arena.
    /// @tparam allocator The allocator to use internally.
    template<typename type, typename allocator = basic_allocator<type>>
    class basic_arena
    {
    public:
        /** The type handled by the arena. */
        typedef type value_type;
        /** The allocator used by the arena. */
        typedef allocator allocator_type;

        /** The type of the arena. */
        typedef basic_arena<value_type, allocator_type> basic_arena_type;

        /// @brief Default constructor.
        inline basic_arena() noexcept :
            buffer(), stack()
        {
        }

        /// @brief Constructs an arena with a given capacity.
        /// @param capacity The capacity of the arena.
        inline basic_arena(usize capacity) noexcept : 
            buffer(capacity), stack(capacity)
        {
            for(usize n = capacity; n != 0; n--)
                stack.push_unchecked(n - 1);
        }

        /// @brief Move constructor.
        /// @param other The arena to move.
        inline basic_arena(basic_arena_type&& other) noexcept : 
            buffer(::std::move(other.buffer)), stack(::std::move(other.stack))
        {
        }

        /// @brief Copy constructor.
        /// @param other The arena to copy.
        inline basic_arena(const basic_arena_type& other) noexcept : 
            buffer(other.buffer), stack(other.stack)
        {
        }

        inline ~basic_arena() noexcept {}

        /// @brief Move assignment.
        /// @param other The arena to move.
        inline basic_arena_type& operator=(basic_arena_type&& other) noexcept
        {
            buffer = ::std::move(other.buffer);
            stack = ::std::move(other.stack);

            return *this;
        }

        /// @brief Copy assignment.
        /// @param other The arena to copy.
        inline basic_arena_type& operator=(const basic_arena_type& other) noexcept
        {
            buffer = other.buffer;
            stack = other.stack;

            return *this;
        }

        /// @brief Assures that the arena has space for at least a number of elements.
        /// @param n The size to reserve.
        inline void reserve(usize n) noexcept
        {
            if(stack.size() < n)
                resize(buffer.capacity() + (n - stack.size()));
        }

        /// @brief Creates an element in the arena and returns its index.
        /// @param _args The arguments to use to construct the element in-place.
        /// @return The index of the constructed element.
        template<typename... args>
        inline usize create(args&&... _args) noexcept
        {
            if(stack.empty())
                resize(capacity_growth(1));

            return create_unchecked(::std::forward<args>(_args)...);
        }

        /// @brief Creates an element without checking for available space.
        /// @param _args The arguments to use to construct the element in-place.
        /// @return The index of the constructed element.
        template<typename... args>
        inline usize create_unchecked(args&&... _args) noexcept
        {
            usize top = stack.back();
            stack.pop();
            buffer.insert_unchecked(top, ::std::forward<args>(_args)...);
            return top;
        }

        /// @brief Destroys an element.
        /// @param i The index of the element to destroy.
        inline void destroy(usize i) noexcept
        {
            if(buffer.has(i))
                destroy_unchecked(i);
        }

        /// @brief Destroys an element without checking if it exists.
        /// @param i The index of the element to destroy.
        inline void destroy_unchecked(usize i) noexcept
        {
            buffer.erase_unchecked(i);
            stack.push_unchecked(i);
        }

        /// @brief Clears the arena, destroying all its elements in the process.
        inline void clear() noexcept
        {
            buffer.clear();
            stack.clear();

            for(usize i = buffer.capacity(); i != 0; i--)
                stack.push_unchecked(i - 1);
        }

        /// @brief Calculates the capacity growth of the arena to accomodate an extra number of elements.
        /// @param extra The number of elements to additionally accomodate.
        /// @return The new capacity.
        inline usize capacity_growth(usize extra) const noexcept
        {
            usize sz = buffer.capacity();
            if(sizeof(value_type) * sz <= (16 << 10))
                return (sz + extra) * 2;
            else return (sz + extra) * 3 / 2;
        }

        /// @brief Checks if the arena has an element at an index.
        /// @param i The index to check.
        /// @return true if there is an object at index i, false otherwise.
        inline bool has(usize i) const noexcept { return buffer.has(i); }

        /// @brief Returns the number of elements in the arena.
        /// @return The number of elements in the arena.
        inline usize size() const noexcept { return buffer.capacity() - stack.size(); }
        /// @brief Returns the number of elements that can be placed in the arena before a reallocation.
        /// @return The number of elements that can be placed in the arena before a reallocation.
        inline usize capacity() const noexcept { return stack.size(); }

        /// @brief Access the element at an index.
        /// @param i The index of the element to access.
        /// @return A reference to the element at index i.
        inline value_type& operator[](usize i) noexcept { return buffer[i]; }
        /// @brief Access the element at an index.
        /// @param i The index of the element to access.
        /// @return A const reference to the element at index i.
        inline const value_type& operator[](usize i) const noexcept { return buffer[i]; }

    private:
        void resize(usize n) noexcept
        {
            usize capacity = buffer.capacity();

            buffer.reserve_exactly(n);
            stack.reserve_exactly(n);

            for(usize i = n; i != capacity; i--)
                stack.push_unchecked(i - 1);
        }

    private:
        typedef sparse_array<value_type, allocator_type> sparse_array_type;
        
        typedef typename allocator_type::template rebind<usize>::allocator_type usize_allocator_type;
        typedef array<usize, usize_allocator_type> array_type;

        sparse_array_type buffer;
        array_type stack;
    };
};

