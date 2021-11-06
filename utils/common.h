#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <new>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

#define ASSERT(e) assert(e)
#define UNREACHABLE() assert(!"unreachable");

template <class T, size_t S>
size_t ARRAY_SIZE(T (&)[S])
{
	return S;
}

static inline u32 next_pow2_32(u32 x)
{
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	return x;
}

static inline u64 next_pow2_64(u64 x)
{
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	x |= x >> 32;
	return x;
}

#define container_of(ptr, type, member)                                                            \
	({                                                                                         \
		const typeof(((type *)0)->member) *__mptr = (ptr);                                 \
		(type *)((char *)__mptr - offsetof(type, member));                                 \
	})

#define roundup(x, y)                                                                              \
	({                                                                                         \
		const typeof(y) __y = y;                                                           \
		(((x) + (__y - 1)) / __y) * __y;                                                   \
	})
