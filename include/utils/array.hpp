#pragma once

#include "buffer.hpp"

#include <iterator>

namespace utils
{
    namespace __detail
    {
        namespace __iterator
        {
            template<typename type>
            using span_iterator = type*;

            template<typename type>
            using span_const_iterator = const type*;

            template<typename type>
            using span_reverse_iterator = ::std::reverse_iterator<span_iterator<type>>;

            template<typename type>
            using span_const_reverse_iterator = ::std::reverse_iterator<span_const_iterator<type>>;

            template<typename type>
            using array_iterator = type*;

            template<typename type>
            using array_const_iterator = const type*;

            template<typename type>
            using array_reverse_iterator = ::std::reverse_iterator<array_iterator<type>>;

            template<typename type>
            using array_const_reverse_iterator = ::std::reverse_iterator<array_const_iterator<type>>;
        };
    };

    template<typename type>
    class span;

    /// @brief A view into a contiguous array of objects.
    /// @tparam type The type of the objects in the view.
    ///
    /// Views do not own objects, just reference them. This span
    /// is an immutable view over the data, that is it can't be
    /// used to modify the data viewed.
    template<typename type>
    class const_span
    {
    public:
        /** The type of the objects in the view. */
        typedef type value_type;
        
        /** The type of the span. */
        typedef span<value_type> span_type;
        /** The type of the constant span. */
        typedef const_span<value_type> const_span_type;

        /** Random access iterator. */
        typedef __detail::__iterator::span_const_iterator<value_type> iterator;
        /** Random const access iterator. */
        typedef __detail::__iterator::span_const_iterator<value_type> const_iterator;
        /** Random access reverse iterator. */
        typedef __detail::__iterator::span_const_reverse_iterator<value_type> reverse_iterator;
        /** Random const access reverse iterator. */
        typedef __detail::__iterator::span_const_reverse_iterator<value_type> const_reverse_iterator;

        /// @brief Constructs a span viewing nothing.
        inline const_span() noexcept :
            start(nullptr), finish(nullptr)
        {
        }

        /// @brief Move constructor.
        /// @param other The span to move.
        inline const_span(const_span_type&& other) noexcept :
            start(other.start), finish(other.finish)
        {
        }

        /// @brief Copy constructor.
        /// @param other The span to copy.
        inline const_span(const const_span_type& other) noexcept :
            start(other.start), finish(other.finish)
        {
        }

        /// @brief Constructs a span viewing the given range.
        /// @param start The beginning of the range to view.
        /// @param finish The end of the range to view.
        inline const_span(const value_type* start, const value_type* finish) noexcept :
            start(start), finish(finish)
        {
        }

        /// @brief Constructs a span viewing the given range.
        /// @param start The beginning of the range to view.
        /// @param length The length of the range to view.
        inline const_span(const value_type* start, usize length) noexcept :
            start(start), finish(start + length)
        {
        }

        inline ~const_span() noexcept
        {
        }

        /// @brief Move assignment.
        /// @param other The span to move.
        inline const_span_type& operator=(const_span_type&& other) noexcept
        {
            start = other.start;
            finish = other.finish;

            return *this;
        }

        /// @brief Copy assignment.
        /// @param other The span to copy.
        inline const_span_type& operator=(const const_span_type& other) noexcept
        {
            start = other.start;
            finish = other.finish;

            return *this;
        }

        /// @brief Remove the front element (i.e. the first element in the view).
        /// @return A const reference to the element removed.
        inline const value_type& pop_front() noexcept { return *(start++); }
        /// @brief Remove the back element (i.e. the last element in the view).
        /// @return A const reference to the element removed.
        inline const value_type& pop_back() noexcept { return *(--finish); }

        /// @brief Remove a number of elements from the front.
        /// @param n The number of elements to remove.
        inline void pop_front(usize n) noexcept { start += n; }
        /// @brief Remove a number of elements from the back.
        /// @param n The number of elements to remove.
        inline void pop_back(usize n) noexcept { finish -= n; }

        /// @brief Creates a subspan of the given span.
        /// @param i The starting index of the subspan.
        /// @param n The length of the subspan.
        /// @return A const span representing the subspan of length n that starts at position i in this span.
        inline const_span_type subspan(usize i, usize n) const noexcept { return const_span_type(start + i, start + i + n); }
        /// @brief Creates a prefix of the given span.
        /// @param n The length of the prefix.
        /// @return A const span representing the prefix of the given length of the span.
        inline const_span_type prefix(usize n) const noexcept { return const_span_type(start, start + n); }
        /// @brief Creates a suffix of the given span.
        /// @param n The length of the suffix.
        /// @return A const span representing the suffix of the given length of the span.
        inline const_span_type suffix(usize n) const noexcept { return const_span_type(finish - n, finish); }

        /// @brief Returns the size of the span.
        /// @return The size of the span.
        inline usize size() const noexcept { return finish - start; }
        /// @brief Checks if the span is empty.
        /// @return true if the span is empty, false otherwise.
        inline bool empty() const noexcept { return finish == start; }

        /// @brief Access the first element in the span.
        /// @return A const reference to the first element in the span.
        inline const value_type& front() const noexcept { return *start; }
        /// @brief Access the last element in the span.
        /// @return A const reference to the last element in the span.
        inline const value_type& back() const noexcept { return *(finish - 1); }

        /// @brief Access the elements of the span.
        /// @return A const pointer to the beginning of the view of the span.
        inline const value_type* data() const noexcept { return start; }

        inline iterator begin() noexcept { return start; }
        inline iterator end() noexcept { return finish; }

        inline const_iterator begin() const noexcept { return start; }
        inline const_iterator end() const noexcept { return finish; }

        inline const_iterator cbegin() const noexcept { return start; }
        inline const_iterator cend() const noexcept { return finish; }

        inline reverse_iterator rbegin() noexcept { return ::std::make_reverse_iterator<iterator>(finish); }
        inline reverse_iterator rend() noexcept { return ::std::make_reverse_iterator<iterator>(start); }

        inline const_reverse_iterator rbegin() const noexcept { return ::std::make_reverse_iterator<const_iterator>(finish); }
        inline const_reverse_iterator rend() const noexcept { return ::std::make_reverse_iterator<const_iterator>(start); }

        inline const_reverse_iterator crbegin() const noexcept { return ::std::make_reverse_iterator<const_iterator>(finish); }
        inline const_reverse_iterator crend() const noexcept { return ::std::make_reverse_iterator<const_iterator>(start); }

        /// @brief Access a random element in the span.
        /// @param i The element to access.
        /// @return A const reference to the i-th element in the span.
        inline const value_type& operator[](usize i) const noexcept { return start[i]; }

    private:
        const value_type* start;
        const value_type* finish;
    };

    /// @brief A view into a contiguous array of objects.
    /// @tparam type The type of the objects in the view.
    ///
    /// Views do not own objects, just reference them.
    template<typename type>
    class span
    {
    public:
        /** The type of the objects in the view. */
        typedef type value_type;

        /** The type of the span. */
        typedef span<value_type> span_type;
        /** The type of the constant span. */
        typedef const_span<value_type> const_span_type;

        /** Random access iterator. */
        typedef __detail::__iterator::span_iterator<value_type> iterator;
        /** Random const access iterator. */
        typedef __detail::__iterator::span_const_iterator<value_type> const_iterator;
        /** Random access reverse iterator. */
        typedef __detail::__iterator::span_reverse_iterator<value_type> reverse_iterator;
        /** Random const access reverse iterator. */
        typedef __detail::__iterator::span_const_reverse_iterator<value_type> const_reverse_iterator;

        /// @brief Constructs a span viewing nothing.
        inline span() noexcept :
            start(nullptr), finish(nullptr)
        {
        }

        /// @brief Move constructor.
        /// @param other The span to move.
        inline span(span_type&& other) noexcept :
            start(other.start), finish(other.finish)
        {
        }

        /// @brief Copy constructor.
        /// @param other The span to copy.
        inline span(const span_type& other) noexcept :
            start(other.start), finish(other.finish)
        {
        }

        /// @brief Constructs a span viewing the given range.
        /// @param start The beginning of the range to view.
        /// @param finish The end of the range to view.
        inline span(value_type* start, value_type* finish) noexcept :
            start(start), finish(finish)
        {
        }

        /// @brief Constructs a span viewing the given range.
        /// @param start The beginning of the range to view.
        /// @param length The length of the range to view.
        inline span(value_type* start, usize length) noexcept :
            start(start), finish(start + length)
        {
        }

        inline ~span() noexcept
        {
        }

        /// @brief Move assignment.
        /// @param other The span to move.
        inline span_type& operator=(span_type&& other) noexcept
        {
            start = other.start;
            finish = other.finish;

            return *this;
        }

        /// @brief Copy assignment.
        /// @param other The span to copy.
        inline span_type& operator=(const span_type& other) noexcept
        {
            start = other.start;
            finish = other.finish;

            return *this;
        }

        /// @brief Remove the front element (i.e. the first element in the view).
        /// @return A reference to the element removed.
        inline value_type& pop_front() noexcept { return *(start++); }
        /// @brief Remove the back element (i.e. the last element in the view).
        /// @return A reference to the element removed.
        inline value_type& pop_back() noexcept { return *(--finish); }

        /// @brief Remove a number of elements from the front.
        /// @param n The number of elements to remove.
        inline void pop_front(usize n) noexcept { start += n; }
        /// @brief Remove a number of elements from the back.
        /// @param n The number of elements to remove.
        inline void pop_back(usize n) noexcept { finish -= n; }

        /// @brief Creates a subspan of the given span.
        /// @param i The starting index of the subspan.
        /// @param n The length of the subspan.
        /// @return A span representing the subspan of length n that starts at position i in this span.
        inline span_type subspan(usize i, usize n) const noexcept { return span_type(start + i, start + i + n); }
        /// @brief Creates a prefix of the given span.
        /// @param n The length of the prefix.
        /// @return A span representing the prefix of the given length of the span.
        inline span_type prefix(usize n) const noexcept { return span_type(start, start + n); }
        /// @brief Creates a suffix of the given span.
        /// @param n The length of the suffix.
        /// @return A span representing the suffix of the given length of the span.
        inline span_type suffix(usize n) const noexcept { return span_type(finish - n, finish); }

        /// @brief Returns the size of the span.
        /// @return The size of the span.
        inline usize size() const noexcept { return finish - start; }
        /// @brief Checks if the span is empty.
        /// @return true if the span is empty, false otherwise.
        inline bool empty() const noexcept { return finish == start; }

        /// @brief Access the first element in the span.
        /// @return A reference to the first element in the span.
        inline value_type& front() const noexcept { return *start; }
        /// @brief Access the last element in the span.
        /// @return A reference to the last element in the span.
        inline value_type& back() const noexcept { return *(finish - 1); }

        /// @brief Access the elements of the span.
        /// @return A pointer to the beginning of the view of the span.
        inline value_type* data() const noexcept { return start; }

        inline iterator begin() noexcept { return start; }
        inline iterator end() noexcept { return finish; }

        inline const_iterator begin() const noexcept { return start; }
        inline const_iterator end() const noexcept { return finish; }

        inline const_iterator cbegin() const noexcept { return start; }
        inline const_iterator cend() const noexcept { return finish; }

        inline reverse_iterator rbegin() noexcept { return ::std::make_reverse_iterator<iterator>(finish); }
        inline reverse_iterator rend() noexcept { return ::std::make_reverse_iterator<iterator>(start); }

        inline const_reverse_iterator rbegin() const noexcept { return ::std::make_reverse_iterator<const_iterator>(finish); }
        inline const_reverse_iterator rend() const noexcept { return ::std::make_reverse_iterator<const_iterator>(start); }

        inline const_reverse_iterator crbegin() const noexcept { return ::std::make_reverse_iterator<const_iterator>(finish); }
        inline const_reverse_iterator crend() const noexcept { return ::std::make_reverse_iterator<const_iterator>(start); }

        /// @brief Access a random element in the span.
        /// @param i The element to access.
        /// @return A reference to the i-th element in the span.
        inline value_type& operator[](usize i) const noexcept { return start[i]; }

        /// @brief Convert the span to a constant span.
        /// @return A constant span over the same range as this span.
        inline operator const_span_type() const noexcept { return const_span_type(start, finish); }

    private:
        value_type* start;
        value_type* finish;
    };

    template<typename type> struct is_relocatable<span<type>> : ::std::true_type {};
    template<typename type> struct is_relocatable<const_span<type>> : ::std::true_type {};

    /// @brief Basic variable length array type.
    /// @tparam type The type of the elements of the array.
    /// @tparam allocator The allocator to use to allocate its data.
    ///
    /// Implementation of an array of variable length that respects RAII.
    template<typename type, typename allocator = allocator<type>>
    class array
    {
    public:
        /** The type of the elements of the array. */
        typedef type value_type;
        /** The type of the allocator of the array. */
        typedef allocator allocator_type;

        /** The type of the internal buffer used by the array. */
        typedef buffer<value_type, allocator_type> buffer_type;
        
        /** The type of the array. */
        typedef array<value_type, allocator_type> array_type;

        /** The type of a span over an array. */
        typedef span<value_type> span_type;
        /** The type of a const span over an array. */
        typedef const_span<value_type> const_span_type;

        /** Random access iterator. */
        typedef __detail::__iterator::array_iterator<value_type> iterator;
        /** Random const access iterator. */
        typedef __detail::__iterator::array_const_iterator<value_type> const_iterator;
        /** Random access reverse iterator. */
        typedef __detail::__iterator::array_reverse_iterator<value_type> reverse_iterator;
        /** Random const access reverse iterator. */
        typedef __detail::__iterator::array_const_reverse_iterator<value_type> const_reverse_iterator;

        /// @brief Structure that checks if a type of an iterator type of this object.
        /// @tparam iterator_type The type to check.
        template<typename iterator_type> struct is_iterator :
            public ::std::bool_constant
            <::std::is_same_v<iterator_type, iterator> ||
             ::std::is_same_v<iterator_type, const_iterator> ||
             ::std::is_same_v<iterator_type, reverse_iterator> ||
             ::std::is_same_v<iterator_type, const_reverse_iterator>> {};

        /// @brief Default constructor.
        ///
        /// Empty array, with no elements allocated.
        inline array() noexcept :
            buff(), finish(nullptr)
        {
        }

        /// @brief Move constructor.
        /// @param other The array moved.
        inline array(array_type&& other) noexcept :
            buff(::std::move(other.buff)), finish(other.finish)
        {
            other.finish = nullptr;
        }
        
        /// @brief Copy constructor.
        /// @param other The array copied.
        inline array(const array_type& other) noexcept :
            buff(other.capacity())
        {
            finish = buff.begin();
            const value_type* v = other.data();
            const value_type* w = v + other.size();
            while(v != w)
                push_unchecked(*(v++));
        }

        /// @brief Copy constructor.
        /// @param other The array copied.
        ///
        /// Note: copy is faster if the objects are relocatable.
        inline array(const array_type& other) noexcept requires relocatable<value_type> :
            buff(other.capacity())
        {
            finish = buff.begin() + other.size();
            ::std::memcpy(buff.data(), other.data(), other.size() * sizeof(value_type));
        }

        /// @brief Initializes the array from an initializer list.
        /// @param list The list from which to initialize the array.
        inline array(::std::initializer_list<value_type> list) noexcept :
            buff(list.size())
        {
            finish = buff.begin();
            const value_type* v = list.begin();
            const value_type* w = list.end();
            while(v != w)
                push_unchecked(*(v++));
        }

        /// @brief Initializes the array from an initializer list.
        /// @param list The list from which to initialize the array.
        ///
        /// Note: copy is faster if the objects are relocatable.
        inline array(std::initializer_list<value_type> list) noexcept requires relocatable<value_type> :
            buff(list.size())
        {
            finish = buff.begin() + list.size();
            ::std::memcpy(buff.data(), list.begin(), list.size() * sizeof(value_type));
        }

        /// @brief Constructs an array with the given capacity, but with size 0.
        /// @param capacity The capacity of the array.
        inline array(usize capacity) noexcept :
            buff(capacity)
        {
            finish = buff.begin();
        }

        /// @brief Constructs an array from the given cstyle contiguous range of elements.
        /// @param _start The beginning of the range.
        /// @param _finish The end of the range.
        inline array(const value_type* _start, const value_type* _finish) noexcept :
            buff(_finish - _start)
        {
            finish = buff.begin();
            while(_start != _finish)
                push_unchecked(*(_start++));
        }

        /// @brief Constructs an array from the given cstyle contiguous range of elements.
        /// @param _start The beginning of the range.
        /// @param _finish The end of the range.
        ///
        /// Note: copy is faster if the objects are relocatable.
        inline array(const value_type* _start, const value_type* _finish) noexcept requires relocatable<value_type> :
            buff(_finish - _start)
        {
            finish = buff.begin() + (_finish - _start);
            ::std::memcpy(buff.data(), _start, (_finish - _start) * sizeof(value_type));
        }

        /// @brief Constructs an array from the first elements of the given buff.
        /// @param _buff The beginning of the buff.
        /// @param n The number of elements to use from the beginning of the buff.
        inline array(const value_type* _buff, usize n) noexcept :
            buff(n)
        {
            finish = buff.begin();
            const value_type* buff_end = _buff + n;
            while(_buff != buff_end)
                push_unchecked(*(_buff++));
        }

        /// @brief Constructs an array from the first elements of the given buff.
        /// @param _buff The beginning of the buff.
        /// @param n The number of elements to use from the beginning of the buff.
        ///
        /// Note: copy is faster if the objects are relocatable.
        inline array(const value_type* _buff, usize n) noexcept requires relocatable<value_type> :
            buff(n)
        {
            finish = buff.begin() + n;
            ::std::memcpy(buff.data(), _buff, n * sizeof(value_type));
        }

        /// @brief Constructs an array from the elements of the given span.
        /// @param span The span to copy.
        inline array(const const_span_type& span) noexcept :
            buff(span.size())
        {
            finish = buff.begin();
            const value_type* start = span.begin();
            const value_type* finish = span.end();
            while(start != finish)
                push_unchecked(*(start++));
        }

        /// @brief Constructs an array from the elements of the given span.
        /// @param span The span to copy.
        ///
        /// Note: copy is faster if the objercts are relocatable.
        inline array(const const_span_type& span) noexcept requires relocatable<value_type> :
            buff(span.size())
        {
            finish = buff.begin() + span.size();
            ::std::memcpy(buff.data(), span.data(), span.size() * sizeof(value_type));
        }

        /// @brief Destructor.
        ///
        /// Calls the destructors of all the elements and then deallocates
        /// the space.
        inline ~array() noexcept
        {
            clear();
        }

        /// @brief Move assignment.
        /// @param other The array to move.
        inline array_type& operator=(array_type&& other) noexcept
        {
            clear();

            buff = ::std::move(other.buff);
            finish = other.finish; other.finish = nullptr;

            return *this;
        }

        /// @brief Copy assignment.
        /// @param other The array to copy.
        inline array_type& operator=(const array_type& other) noexcept
        {
            clear();
            reserve(other.capacity());

            const value_type* v = other.data();
            const value_type* w = v + other.size();
            while(v != w)
                push_unchecked(*(v++));
        
            return *this;
        }

        /// @brief Copy assignment.
        /// @param other The array to copy.
        ///
        /// Note: copy is faster if the objects are relocatable.
        inline array_type& operator=(const array_type& other) noexcept requires relocatable<value_type>
        {
            clear();
            reserve(other.capacity());

            finish = buff.begin() + other.size();
            ::std::memcpy(buff.data(), other.data(), other.size() * sizeof(value_type));

            return *this;
        }

        /// @brief Copy an initializer list.
        /// @param list The list to copy.
        inline array_type& operator=(::std::initializer_list<value_type> list) noexcept
        {
            clear();
            reserve(list.size());

            const value_type* v = list.begin();
            const value_type* w = list.end();
            while(v != w)
                push_unchecked(*(v++));

            return *this;
        }

        /// @brief Copy an initializer list.
        /// @param list The list to copy.
        ///
        /// Note: copy is faster if the objects are relocatable.
        inline array_type& operator=(::std::initializer_list<value_type> list) noexcept requires relocatable<value_type>
        {
            clear();
            reserve(list.size());

            finish = buff.begin() + list.size();
            ::std::memcpy(buff.data(), list.begin(), list.size() * sizeof(value_type));

            return *this;
        }

        /// @brief Copy a span.
        /// @param span The span to copy.
        inline array_type& operator=(const const_span_type& span) noexcept
        {
            clear();
            reserve(span.size());

            const value_type* start = span.begin();
            const value_type* finish = span.end();
            while(start != finish)
                push_unchecked(*(start++));

            return *this;
        }

        /// @brief Copy a span.
        /// @param span The span to copy.
        ///
        /// Note: copy is faster if the objects are relocatable.
        inline array_type& operator=(const const_span_type& span) noexcept requires relocatable<value_type>
        {
            clear();
            reserve(span.size());

            finish = buff.begin() + span.size();
            ::std::memcpy(buff.data(), span.data(), span.size() * sizeof(value_type));

            return *this;
        }

        /// @brief Assures that the array has enough space for some number of objects.
        /// @param capacity The number of objects to reserve.
        ///
        /// If the array doesn't have enough space for capacity objects, it reallocates
        /// the space to make it at least as big as capacity.
        inline void reserve(usize capacity) noexcept
        {
            if(this->capacity() < capacity)
                resize(capacity);
        }

        /// @brief Assures that the array has space for exactly some number of objects.
        /// @param capacity The number of objects to reserve.
        ///
        /// If the array doesn't have exactly enough space for capacity objects, it reallocates
        /// the space to make it exactly as big as capacity.
        inline void reserve_exactly(usize capacity) noexcept
        {
            if(this->capacity() != capacity)
                resize(capacity);
        }

        /// @brief Shrinks the array's capacity to exactly its size.
        inline void shrink_to_fit() noexcept
        {
            resize(size());
        }

        /// @brief Pushes an element to the top of the array which is constructed in-place.
        /// @param _args The arguments to pass to the constructor.
        /// @return A reference to the element constructed.
        ///
        /// If the array doesn't have enough space for the new element, reallocates the memory
        /// in such a way that a series of N pushes will result in O(N) time complexity.
        template<typename... args>
        inline value_type& push(args&&... _args) noexcept
        {
            if(finish == buff.end())
                resize(capacity_growth(1));

            allocator_type::construct_at(finish, ::std::forward<args>(_args)...);
            return *(finish++);
        }

        /// @brief Pushes an element to the top of the array which is constructed in-place.
        /// @param _args The arguments to pass to the constructor.
        /// @return A reference to the element constructed.
        ///
        /// Doesn't check if the array has enough space for the new element. Useful right
        /// after a call to reserve, but it's the user's responsability to not go past the
        /// array's capacity.
        template<typename... args>
        inline value_type& push_unchecked(args&&... _args) noexcept
        {
            allocator_type::construct_at(finish, ::std::forward<args>(_args)...);
            return *(finish++);
        }

        /// @brief Pushes many copies of the same value.
        /// @param value The value to push.
        /// @param n The number of copies to push.
        ///
        /// Just like push, checks if the array has enough space and resizes it if needed.
        inline void push_many(const value_type& value = value_type(), usize n = 1) noexcept
        {
            if(finish + n > buff.end())
                resize(capacity_growth(n));

            value_type* new_finish = finish + n;
            while(finish != new_finish)
                allocator_type::construct_at(finish++, value);
        }

        /// @brief Pushes many copies of the same value.
        /// @param value The value to push.
        /// @param n The number of copies to push.
        ///
        /// Just like push_unchecked, doesn't check the capacity of the array.
        inline void push_many_unchecked(const value_type& value = value_type(), usize n = 1) noexcept
        {
            value_type* new_finish = finish + n;
            while(finish != new_finish)
                allocator_type::construct_at(finish++, value);
        }

        /// @brief Pushes copies of the elements of a span into the array.
        /// @param span The span to push.
        ///
        /// Just like push, checks if the array has enough space and resizes it if needed.
        inline void push_many(const const_span_type& span) noexcept
        {
            if(finish + span.size() > buff.end())
                resize(capacity_growth(span.size()));

            value_type* new_finish = finish + span.size();
            const value_type* current = span.begin();
            while(finish != new_finish)
                allocator_type::construct_at(finish++, *(current++));
        }

        /// @brief Pushes copies of the elements of a span into the array.
        /// @param span The span to push.
        ///
        /// Just like push_unchecked, doesn't check the capacity of the array.
        inline void push_many_unchecked(const const_span_type& span) noexcept
        {
            value_type* new_finish = finish + span.size();
            const value_type* current = span.begin();
            while(finish != new_finish)
                allocator_type::construct_at(finish++, *(current++));
        }

        /// @brief Pops the top element of the array.
        inline void pop() noexcept
        {
            allocator_type::destruct_at(--finish);
        }

        /// @brief Pops the top elements of the array.
        /// @param n The number of elements to pop.
        inline void pop_many(usize n) noexcept
        {
            value_type* new_finish = finish - n;
            while(finish != new_finish)
                allocator_type::destruct_at(--finish);
        }

        /// @brief Erases a random element, without keeping the same ordering.
        /// @param it An iterator to the element to erase.
        /// @return An iterator to the next element after the one erased.
        ///
        /// Erases the element pointed at by it by destructing it and moving the top of the array
        /// to its position. Don't use this function if the order of the elements in the array
        /// matters.
        template<typename iterator_type> requires is_iterator<iterator_type>::value
        iterator_type erase_unordered(iterator_type it)
        {
            --finish;
            if(it != finish)
                *it = ::std::move(*finish);
            allocator_type::destruct_at(finish);
            return it;
        }

        /// @brief Clears the array.
        inline void clear() noexcept
        {
            while(finish != buff.begin())
                allocator_type::destruct_at(--finish);
        }

        /// @brief Calculates the new capacity of the array that accommodates an additional number of objects.
        /// @param extra The additional number of objects that the new capacity must accommodate.
        /// @return The new capacity.
        inline usize capacity_growth(usize extra) const noexcept
        {
            usize sz = size();
            if(sizeof(value_type) * sz <= (16 << 10))
                return (sz + extra) * 2;
            else return (sz + extra) * 3 / 2;
        }

        /// @brief Returns a span over a part of this array.
        /// @param i The beginning index of the span.
        /// @param n The length of the span.
        /// @return A span over the elements at indices [i, i + n).
        inline span_type subarray(usize i, usize n) noexcept { return span_type(buff.begin() + i, buff.begin() + i + n); }
        /// @brief Returns a const span over a part of this array.
        /// @param i The beginning index of the span.
        /// @param n The length of the span.
        /// @return A const span over the elements at indices [i, i + n).
        inline const_span_type subarray(usize i, usize n) const noexcept { return const_span_type(buff.begin() + i, buff.begin() + i + n); }

        /// @brief Returns a span over a prefix of the array.
        /// @param n The length of the prefix.
        /// @return A span over the elements at indices [0, n).
        inline span_type prefix(usize n) noexcept { return span_type(buff.begin(), buff.begin() + n); }
        /// @brief Returns a const span over a prefix of the array.
        /// @param n The length of the prefix.
        /// @return A const span over the elements at indices [0, n).
        inline const_span_type prefix(usize n) const noexcept { return const_span_type(buff.begin(), buff.begin() + n); }

        /// @brief Returns a span over a suffix of the array.
        /// @param n The length of the suffix.
        /// @return A span over the elements at indices [size() - n, size()).
        inline span_type suffix(usize n) noexcept { return span_type(finish - n, finish); }
        /// @brief Returns a const span over a suffix of the array.
        /// @param n The length of the suffix.
        /// @return A const span over the elements at indices [size() - n, size()).
        inline const_span_type suffix(usize n) const noexcept { return const_span_type(finish - n, finish); }

        inline iterator begin() noexcept { return buff.begin(); }
        inline iterator end() noexcept { return finish; }

        inline const_iterator begin() const noexcept { return buff.begin(); }
        inline const_iterator end() const noexcept { return finish; }

        inline const_iterator cbegin() const noexcept { return buff.begin(); }
        inline const_iterator cend() const noexcept { return finish; }

        inline reverse_iterator rbegin() noexcept { return ::std::make_reverse_iterator<iterator>(finish); }
        inline reverse_iterator rend() noexcept { return ::std::make_reverse_iterator<iterator>(buff.begin()); }

        inline const_reverse_iterator crbegin() const noexcept { return ::std::make_reverse_iterator<const_iterator>(finish); }
        inline const_reverse_iterator crend() const noexcept { return ::std::make_reverse_iterator<const_iterator>(buff.begin()); }

        /// @brief Calculates the size of the array.
        /// @return The size of the array.
        inline usize size() const noexcept { return finish - buff.begin(); }
        /// @brief Calculates the capacity of the array.
        /// @return The capacity of the array.
        inline usize capacity() const noexcept { return buff.size(); }
        /// @brief Checks if the array is empty.
        /// @return true if the array is empty, false otherwise.
        inline bool empty() const noexcept { return finish == buff.begin(); }

        /// @brief Returns the beginning of the memory containing the array.
        /// @return A pointer to the beginning of the memory containg the array.
        inline value_type* data() noexcept { return buff.data(); }
        inline const value_type* data() const noexcept { return buff.data(); }

        /// @brief Returns a reference to the element at the front of the array.
        /// @return A reference to the element at the front of the array.
        inline value_type& front() noexcept { return *buff.begin(); }
        /// @brief Returns a reference to the element at the front of the array.
        /// @return A reference to the element at the front of the array.
        inline const value_type& front() const noexcept { return *buff.begin(); }

        /// @brief Returns a reference to the element at the back of the array.
        /// @return A reference to the element at the back of the array.
        inline value_type& back() noexcept { return *(finish - 1); }
        /// @brief Returns a reference to the element at the back of the array.
        /// @return A reference to the element at the back of the array.
        inline const value_type& back() const noexcept { return *(finish - 1); }

        /// @brief Accesses an element of the array.
        /// @param i The index of the element to access.
        /// @return A reference to the element at index i.
        inline value_type& operator[](usize i) noexcept { return buff[i]; }
        /// @brief Accesses an element of the array.
        /// @param i The index of the element to access.
        /// @return A reference to the element at index i.
        inline const value_type& operator[](usize i) const noexcept { return buff[i]; }

        /// @brief Create a span over the whole array.
        /// @return A span over the whole array.
        inline operator span_type() noexcept { return span_type(buff.begin(), finish); }
        /// @brief Create a const span over the whole array.
        /// @return A const span over the whole array.
        inline operator const_span_type() const noexcept { return const_span_type(buff.begin(), finish); }

    private:
        void resize(usize n) noexcept
        {
            buffer_type new_buffer(n);

            value_type* v = new_buffer.begin();
            value_type* w = buff.begin();
            while(w != finish)
            {
                allocator_type::construct_at(v++, ::std::move(*w));
                allocator_type::destruct_at(w++);
            }

            buff = ::std::move(new_buffer);
            finish = v;
        }

        void resize(usize n) noexcept requires relocatable<value_type>
        {
            usize sz = size();
            buff.resize(n);
            finish = buff.begin() + sz;
        }

    private:
        buffer_type buff;
        value_type* finish;
    };

    template<typename type> struct is_relocatable<__detail::__iterator::array_iterator<type>> : public ::std::true_type {};
    template<typename type> struct is_relocatable<__detail::__iterator::array_const_iterator<type>> : public ::std::true_type {};
    template<typename type> struct is_relocatable<__detail::__iterator::array_reverse_iterator<type>> : public ::std::true_type {};
    template<typename type> struct is_relocatable<__detail::__iterator::array_const_reverse_iterator<type>> : public ::std::true_type {};
    template<typename type, typename allocator> struct is_relocatable<array<type, allocator>> : public ::std::true_type {};
};

