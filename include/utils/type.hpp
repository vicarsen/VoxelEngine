/**
 * @file
 * @brief Provides utilities for data types, type traits, and more.
 *
 * The basic sized data types are defined here, as well as other type traits used throughout the library.
 */
#pragma once

#include <cstdlib>
#include <cstdint>
#include <utility>

/** Byte type. */
typedef ::std::uint8_t byte;

/** Signed integer type of 8-bit width. */
typedef ::std::int8_t i8;
/** Signed integer type of 16-bit width. */
typedef ::std::int16_t i16;
/** Signed integer type of 32-bit width. */
typedef ::std::int32_t i32;
/** Signed integer type of 64-bit width. */
typedef ::std::int64_t i64;

/** Unsigned integer type of 8-bit width. */
typedef ::std::uint8_t u8;
/** Unsigned integer type of 16-bit width. */
typedef ::std::uint16_t u16;
/** Unsigned integer type of 32-bit width. */
typedef ::std::uint32_t u32;
/** Unsigned integer type of 64-bit width. */
typedef ::std::uint64_t u64;

/** Floating point type of 32-bit width. */
typedef float f32;
/** Floating point type of 64-bit width. */
typedef double f64;

/** Unsigned size type. */
typedef ::std::size_t usize;
/** Signed size type. */
typedef ::std::ptrdiff_t size;

namespace utils
{
    /// @brief Concept that classifies a fundamental type.
    /// @tparam type The type checked.
    template<typename type>
    concept fundamental = ::std::is_fundamental_v<type>;

    /// @brief Concept that classifies a trivially copyable type.
    /// @tparam type The type checked.
    template<typename type>
    concept trivially_copyable = ::std::is_trivially_copyable_v<type>;

    /// @brief Concept that classifies a trivially destructible type.
    /// @tparam type The type checked.
    template<typename type>
    concept trivially_destructible = ::std::is_trivially_destructible_v<type>;

    /// @brief A struct boolean type that classifies relocatable types.
    /// @tparam type The type checked.
    ///
    /// A relocatable type is a type for which objects can be moved around by simply
    /// moving their bytes (for example using memcpy).
    ///
    /// By default, all types are classified as non-relocatable, other than fundamental
    /// or trivially copyable types. It is recommended to create specializations as it
    /// can make certain algorithms faster.
    template<typename type>
    struct is_relocatable : public ::std::false_type {};

    template<typename type> requires (fundamental<type> || trivially_copyable<type>)
    struct is_relocatable<type> : public ::std::true_type {};

    /// @brief Concept that classifies a relocatable type.
    /// @tparam type The type checked.
    template<typename type>
    concept relocatable = is_relocatable<type>::value;

    /// @brief An allocator type used for handling memory allocation/deallocation,
    /// and object construction/destruction.
    /// @tparam type The type that the allocator handles.
    template<typename type>
    class allocator
    {
    public:
        /** The type allocated by the allocator. */
        typedef type value_type;
        /** The type of the allocator. */
        typedef allocator<type> allocator_type;

        allocator() = delete;
        ~allocator() = delete;

        /// @brief Allocates a single object.
        /// @return Pointer to the object allocated, or nullptr if failed.
        static inline value_type* allocate() noexcept { return reinterpret_cast<value_type*>(::std::malloc(sizeof(value_type))); }
        /// @brief Allocates a contiguous array of objects.
        /// @param n The length of the array to allocate.
        /// @return Pointer to the beginning of the array allocated, or nullptr if failed.
        static inline value_type* allocate(usize n) noexcept { return reinterpret_cast<value_type*>(::std::malloc(n * sizeof(value_type))); }

        /// @brief Reallocates a contiguous array of objects.
        /// @param ptr The beginning of the array to reallocate.
        /// @param n The length of the reallocated array.
        /// @return Pointer to the beginning of the array allocated, or nullptr if failed.
        static inline value_type* reallocate(value_type* ptr, usize n) noexcept { return reinterpret_cast<value_type*>(::std::realloc(ptr, n * sizeof(value_type))); }

        /// @brief Deallocates a memory region previously allocated.
        /// @param ptr The beginning of the region to deallocate.
        static inline void deallocate(value_type* ptr) noexcept { ::std::free(ptr); }

        /// @brief In-place constructs an object.
        /// @param ptr The memory location where to construct the object.
        /// @param _args The arguments to forward to the object's constructor.
        template<typename... args>
        static inline void construct_at(value_type* ptr, args&&... _args) noexcept { new (ptr) value_type(::std::forward<args>(_args)...); }

        /// @brief In-place destructs an object.
        /// @param ptr The memory location of the object to destruct.
        ///
        /// For trivially destructible types, it is specialized to do nothing (i.e. it gets
        /// compiled away).
        static inline void destruct_at(value_type* ptr) noexcept { ptr->~value_type(); }

        static inline void destruct_at([[maybe_unused]] value_type* ptr) noexcept requires trivially_destructible<value_type> {}
    };
};

