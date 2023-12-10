/**
 * @file
 * @brief Provides utilities for data types, type traits, and more.
 *
 * The basic sized data types are defined here, as well as other type traits used throughout the library.
 */
#pragma once

#include <cstdint>

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
/** Signed integer type of 128-bit width. */
typedef __int128_t i128;

/** Unsigned integer type of 8-bit width. */
typedef ::std::uint8_t u8;
/** Unsigned integer type of 16-bit width. */
typedef ::std::uint16_t u16;
/** Unsigned integer type of 32-bit width. */
typedef ::std::uint32_t u32;
/** Unsigned integer type of 64-bit width. */
typedef ::std::uint64_t u64;
/** Unsigned integer type of 128-bit width. */
typedef __uint128_t u128;

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
};

