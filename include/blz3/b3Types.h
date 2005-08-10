/*
**
**      $Filename:      b3Types.h $
**      $Release:       Dortmund 2001 $
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

#ifndef B3_TYPES_H
#define B3_TYPES_H

#include <sys/types.h>
#include <math.h>

#include "blz3/b3PluginDef.h"

#if defined(__ICC)
#	define B3_SSE1
#	define B3_SSE2
#elif defined(WIN32)
#	define B3_SSE1
#	define B3_SSE2
#endif

#if defined(__ICC)
#	include <xmmintrin.h>

#	define B3_ALIGN_16  __declspec(align(16))
#	define B3_ALIGN_32  __declspec(align(32))
#	define B3_ALIGN_64  __declspec(align(64))
#else
#	define B3_ALIGN_16
#	define B3_ALIGN_32
#	define B3_ALIGN_64
#endif

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
typedef unsigned char       b3_u08;  //!< Granted unsigned integer with 8 bits.
typedef unsigned short      b3_u16;  //!< Granted unsigned integer with 16 bits.
typedef unsigned int        b3_u32;  //!< Granted unsigned integer with 32 bits.

#ifdef WIN32
typedef unsigned __int64    b3_u64;  //!< Granted unsigned integer with 64 bits.
#else
typedef unsigned long long  b3_u64;  //!< Granted unsigned integer with 64 bits.
#endif

// Signed integer
typedef   signed char       b3_s08;  //!< Granted signed integer with 8 bits.
typedef   signed short      b3_s16;  //!< Granted signed integer with 16 bits.
typedef   signed int        b3_s32;  //!< Granted signed integer with 32 bits.

#ifdef WIN32
typedef   signed __int64    b3_s64;  //!< Granted signed integer with 64 bits.
#else
typedef   signed long long  b3_s64;  //!< Granted signed integer with 64 bits.
#endif

// Boolean
typedef b3_u32              b3_bool; //!< Granted boolean value with 32 bits.

// Floating point
typedef float               b3_f32;  //!< Granted single precision floating point number with 32 bits.
typedef double              b3_f64;  //!< Granted double precision floating point number with 64 bits.
typedef long double         b3_f96;  //!< Granted extended precision floating point number with at leat 80 bits.

// Some other types
typedef size_t              b3_size;       //!< A size specifier (like size_t).
typedef off_t               b3_offset;     //!< An offset specifier (like off_t).
typedef long                b3_index;      //!< A signed index specifier.
typedef long                b3_count;      //!< A signed counter.
typedef long                b3_res;        //!< A signed resolution specifier.
typedef b3_u32              b3_pkd_color;  //!< An unsigned color specifier using AARRGGBB as nibbles.
typedef long                b3_coord;      //!< A signed coordinate specifier.
typedef int                 b3_loop;       //!< A signed integer for small loops (max. 32 bits, but can be more).

#ifdef WIN32
typedef __w64               b3_ptr;        //!< A pointer for pointer arithmetics.
#else
typedef unsigned long       b3_ptr;        //!< A pointer for pointer arithmetics.
#endif

// Some vectors
/**
 * This structure represents a three component vector with single floating point numbers.
 * The first element is aligned on a 16 byte boundary.
 */
typedef struct B3_ALIGN_16 b3_vector32_3D
{
	b3_f32 B3_ALIGN_16 x; //!< The 16 byte aligned x component.
	b3_f32             y; //!< The y component.
	b3_f32             z; //!< The z component.
} b3_vector32, b3_vector;

/**
 * This structure represents a three component vector with double floating point numbers.
 * The first element is aligned on a 32 byte boundary.
 */
typedef struct B3_ALIGN_32 b3_vector64_3D
{
	b3_f64 B3_ALIGN_16 x; //!< The 16 byte aligned x component.
	b3_f64             y; //!< The y component.
	b3_f64             z; //!< The z component.
} b3_vector64;

/**
 * This structure represents a four component vector with single floating point numbers.
 * The first element is aligned on a 16 byte boundary.
 */
typedef struct B3_ALIGN_16 b3_vector32_4D
{
	b3_f32    x; //!< The 16 byte aligned x component.
	b3_f32    y; //!< The y component.
	b3_f32    z; //!< The z component.
	b3_f32    w; //!< The w component.
} b3_vector4D;

/**
 * This structure represents a four component vector with double floating point numbers.
 * The first element is aligned on a 32 byte boundary.
 */
typedef struct B3_ALIGN_32 b3_vector64_4D
{
	b3_f64    x; //!< The 16 byte aligned x component.
	b3_f64    y; //!< The y component.
	b3_f64    z; //!< The z component.
	b3_f64    w; //!< The w component.
} b3_vector4D_64;

/**
 * This structure represents a 4x4 matrix with single precision floating point numbers. The
 * first element is aligned on a 16 byte boundary.
 */
typedef struct B3_ALIGN_64 b3_matrix32_4D
{
	b3_f32 B3_ALIGN_16 m11;
	b3_f32                 m12,m13,m14;
	b3_f32             m21,m22,m23,m24;
	b3_f32             m31,m32,m33,m34;
	b3_f32             m41,m42,m43,m44;
} b3_matrix;

/**
 * This structure represents a 4x4 matrix with double precision floating point numbers. The
 * first element is aligned on a 16 byte boundary.
 */
typedef struct B3_ALIGN_64 b3_matrix64_4D
{
	b3_f64    m11,m12,m13,m14;
	b3_f64    m21,m22,m23,m24;
    b3_f64    m31,m32,m33,m34;
    b3_f64    m41,m42,m43,m44;
} b3_matrix64;

/**
 * This structure represents a ray with single precision floating point numbers. The
 * first element is aligned on a 32 byte boundary.
 */
typedef struct B3_ALIGN_32 b3_line32_3D
{
	b3_vector pos; //!< The base position.
	b3_vector dir; //!< The direction vector.
} b3_line32, b3_line;

/**
 * This structure represents a ray with double precision floating point numbers. The
 * first element is aligned on a 64 byte boundary.
 */
typedef struct B3_ALIGN_64 b3_line64_3D
{
	b3_vector64 pos; //!< The base position.
	b3_vector64 dir; //!< The direction vector.
} b3_line64;

/**
 * This structure represents a display point as signed integer. The first element is
 * aligned on a 16 byte boundary.
 */
struct B3_ALIGN_16 b3_pos
{
	/**
	 * The x and y component of a window or screen position.
	 */
	b3_s32    x,y;
};

/**
 * This structure represents a 2D point as floats. The first element is aligned on a
 * 16 byte boundary.
 */
struct B3_ALIGN_16 b3_point
{
	/**
	 * The floating point representation of a point.
	 */
	b3_f32    x,y;
};

/**
 * This structure contains three RGB color values and an alpha value as floats. The
 * first element is aligned on a 16 byte boundary.
 */
typedef struct B3_ALIGN_16 b3_color32
{
	b3_f32 a; //!< The alpha channel.
	b3_f32 r; //!< The red channel.
	b3_f32 g; //!< The green channel.
	b3_f32 b; //!< The blue channel.
} b3_color;

// Some OpenGL data structures
/**
 * This structure represents an OpenGL texture coordinate with two floats.
 */
struct b3_gl_texture
{
	b3_f32 s; //!< The textural x direction.
	b3_f32 t; //!< The textural y direction.
};

/**
 * This structure represents an OpenGL vector with three floats.
 */
struct b3_gl_vector
{
	b3_f32 x; //!< The x component.
	b3_f32 y; //!< The y component.
	b3_f32 z; //!< The z component.
};

// Some definitions
#ifndef null
#define null 0
#endif

#ifndef false
#define false 0
#endif

#ifndef true
#define true  1
#endif

#ifndef M_PI
#define M_PI 3.14159265
#endif

#define B3_ABS(a)           ((a) < 0 ? (-(a)) : (a))
#define B3_SIGN(a)          (((a) == 0) ? 0 : ((a) < 0 ? -1 : 1))
#define B3_MIN(a,b)         ((a) < (b) ? (a) : (b))
#define B3_MAX(a,b)         ((a) < (b) ? (b) : (a))
#define B3_LIMIT(v,min,max) { if ((v) < (min)) (v) = (min); if ((v) > (max)) (v) = (max); }
#define B3_RAD(a)           ((a) * M_PI / 180.0)
#define B3_DEGREE(a)        ((a) * 180.0 / M_PI)

#endif
