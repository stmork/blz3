/*
**
**      $Filename:      b3Types.h $
**      $Release:       Dortmund 2001 - 2021 $
**      $Revision$
**      $Date$
**      $Author$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - basic types
**
**      (C) Copyright  Steffen A. Mork
**          All Rights Reserved
**
**
**
**
*/

#pragma once

#ifndef B3_TYPES_H
#define B3_TYPES_H

#include <sys/types.h>
#include <cmath>
#include <cstdint>
#include <algorithm>
#include <numbers>

#include "blz3/b3PluginDef.h"

#if __cplusplus < 201703
#error "Need at least C++17 standard!"
#endif

#ifdef __GNUC__
#define B3_UNUSED __attribute__((unused))
#else
#define B3_UNUSED       [[maybe_unused]] //!< This macro marks an unused argument.
#endif

#define B3_FALLTHROUGH  [[fallthrough]] //!< this macro allows switch/case fall through

// Some error codes
/**
 * This enumeration shows general error states.
 */
enum b3_result
{
	B3_ERROR = -1, //!< General error state.
	B3_OK    =  0  //!< General OK state.
};

// Unsigned integer
typedef std::uint8_t        b3_u08;  //!< Granted unsigned integer with 8 bits.
typedef std::uint16_t       b3_u16;  //!< Granted unsigned integer with 16 bits.
typedef std::uint32_t       b3_u32;  //!< Granted unsigned integer with 32 bits.
typedef std::uint64_t       b3_u64;  //!< Granted unsigned integer with 64 bits.

// Signed integer
typedef std::int8_t         b3_s08;  //!< Granted signed integer with 8 bits.
typedef std::int16_t        b3_s16;  //!< Granted signed integer with 16 bits.
typedef std::int32_t        b3_s32;  //!< Granted signed integer with 32 bits.
typedef std::int64_t        b3_s64;  //!< Granted signed integer with 64 bits.

// Boolean
typedef b3_u32              b3_bool; //!< Granted boolean value with 32 bits.

// Floating point
typedef float               b3_f32;  //!< Granted single precision floating point number with 32 bits.
typedef double              b3_f64;  //!< Granted double precision floating point number with 64 bits.
typedef long double         b3_f96;  //!< Granted extended precision floating point number with at leat 80 bits.

// Some other types
typedef std::size_t         b3_size;       //!< A size specifier (like size_t).
typedef off_t               b3_offset;     //!< An offset specifier (like off_t).
typedef int                 b3_index;      //!< A signed index specifier.
typedef int                 b3_count;      //!< A signed counter.
typedef int                 b3_res;        //!< A signed display resolution specifier.
typedef b3_u32              b3_pkd_color;  //!< An unsigned color specifier using AARRGGBB as nibbles.
typedef int                 b3_coord;      //!< A signed coordinate specifier.
typedef int                 b3_loop;       //!< A signed integer for small loops (max. 32 bits, but can be more).
typedef std::ptrdiff_t      b3_ptr;        //!< A pointer for pointer arithmetics.

template<typename T> struct alignas(sizeof(T) * 4) b3_vector_3D_base
{
	T             x; //!< The x component.
	T             y; //!< The y component.
	T             z; //!< The z component.

	inline bool operator==(const b3_vector_3D_base & other)
	{
		return (x == other.x) && (y == other.y) && (z == other.z);
	}
};

// Some vectors
/**
 * This structure represents a three component vector with single floating point numbers.
 * The first element is aligned on a 16 byte boundary.
 */
typedef b3_vector_3D_base<b3_f32> b3_vector32_3D, b3_vector32, b3_vector;

/**
 * This structure represents a three component vector with double floating point numbers.
 * The first element is aligned on a 32 byte boundary.
 */
typedef b3_vector_3D_base<b3_f64> b3_vector64_3D, b3_vector64;

/**
 * This structure represents a four component vector.
 */
template<typename T> struct alignas(sizeof(T) * 4) b3_vector_4D_base
{
	T             x; //!< The x component.
	T             y; //!< The y component.
	T             z; //!< The z component.
	T             w; //!< The w component.

	inline bool operator==(const b3_vector_4D_base & other)
	{
		return (x == other.x) && (y == other.y) && (z == other.z) && (w == other.w);
	}
};

/**
 * This structure represents a four component vector with single floating point numbers.
 * The first element is aligned on a 16 byte boundary.
 */
typedef b3_vector_4D_base<b3_f32> b3_vector32_4D, b3_vector4D;

/**
 * This structure represents a four component vector with double floating point numbers.
 * The first element is aligned on a 32 byte boundary.
 */
typedef b3_vector_4D_base<b3_f64> b3_vector64_4D;

/**
 * This structure represents a 4x4 matrix with floating point numbers. The
 * first element is aligned on a 32 byte boundary.
 */
template<typename T> struct alignas(32) b3_matrix_base
{
	T             m11, m12, m13, m14;
	T             m21, m22, m23, m24;
	T             m31, m32, m33, m34;
	T             m41, m42, m43, m44;
};

/**
 * This structure represents a 4x4 matrix with single precision floating point
 * numbers. The first element is aligned on a 32 byte boundary.
 */
typedef b3_matrix_base<b3_f32> b3_matrix32_4D, b3_matrix;

/**
 * This structure represents a 4x4 matrix with double precision floating point
 * numbers. The first element is aligned on a 32 byte boundary.
 */
typedef b3_matrix_base<b3_f64> b3_matrix64_4D, b3_matrix64;

/**
 * This structure represents a ray with single precision floating point numbers. The
 * first element is aligned on a 16 byte boundary.
 */
typedef struct alignas(16) b3_line32_3D
{
	b3_vector pos; //!< The base position.
	b3_vector dir; //!< The direction vector.
} b3_line32, b3_line;

/**
 * This structure represents a ray with double precision floating point numbers. The
 * first element is aligned on a 16 byte boundary.
 */
typedef struct alignas(32) b3_line64_3D
{
	b3_vector64 pos; //!< The base position.
	b3_vector64 dir; //!< The direction vector.
} b3_line64;

/**
 * This structure represents a display point as signed integer. The first element is
 * aligned on a 16 byte boundary.
 */
struct alignas(16) b3_pos
{
	/**
	 * The x and y component of a window or screen position.
	 */
	b3_s32    x, y;
};

/**
 * This structure represents a 2D point as floats. The first element is aligned on a
 * 16 byte boundary.
 */
struct alignas(16) b3_point
{
	/**
	 * The floating point representation of a point.
	 */
	b3_f32    x, y;
};

/**
 * This structure represents a 2D bounding rectangle. The lower elements should
 * be smaller than the upper elements.
 */
struct alignas(16) b3_bound_2d
{
	b3_f32    left;   //!< The lower left bound
	b3_f32    bottom; //!< The lower bottom bound
	b3_f32    right;  //!< The upper right bound
	b3_f32    top;    //!< The upper top bound
};

/**
 * This structure contains three RGB color values and an alpha value as floats. The
 * first element is aligned on a 16 byte boundary.
 */
typedef struct alignas(16) b3_color32
{
	b3_f32    a; //!< The alpha channel.
	b3_f32    r; //!< The red channel.
	b3_f32    g; //!< The green channel.
	b3_f32    b; //!< The blue channel.

	inline bool operator==(const b3_color32 & other) const
	{
		return
			(a == other.a) && (r == other.r) &&
			(g == other.g) && (b == other.b);
	}
} b3_color;

// Some OpenGL data structures
/**
 * This structure represents an OpenGL texture coordinate with two floats.
 */
struct b3_gl_texture
{
	b3_f32    s; //!< The textural x direction.
	b3_f32    t; //!< The textural y direction.
};

/**
 * This structure represents an OpenGL vector with three floats.
 */
struct b3_gl_vector
{
	b3_f32    x; //!< The x component.
	b3_f32    y; //!< The y component.
	b3_f32    z; //!< The z component.
};

// Some definitions
#ifndef M_PI
#error "Pi is undefined!"
#endif

#endif
