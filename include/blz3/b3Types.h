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

// Some error codes
typedef enum
{
	B3_ERROR = -1,
	B3_OK    = 0
} b3_result;

// Unsigned integer
typedef unsigned char       b3_u08;
typedef unsigned short      b3_u16;
typedef unsigned int        b3_u32;

#ifdef WIN32
typedef unsigned long int   b3_u64;
#else
typedef unsigned long long  b3_u64;
#endif

// Signed integer
typedef   signed char       b3_s08;
typedef   signed short      b3_s16;
typedef   signed int        b3_s32;

#ifdef WIN32
typedef   signed long int   b3_s64;
#else
typedef   signed long long  b3_s64;
#endif

// Boolean
typedef b3_u32              b3_bool;

// Floating point
typedef float               b3_f32;
typedef double              b3_f64;
typedef long double         b3_f96;

// Some other types
typedef size_t              b3_size;
typedef off_t               b3_offset;
typedef long                b3_index;
typedef long                b3_count;
typedef long                b3_res;
typedef b3_u32              b3_pkd_color;
typedef long                b3_coord;

// Some vectors
typedef struct b3_vector
{
	b3_f32    x;
	b3_f32    y;
	b3_f32    z;
} b3_fVector, b3_vector32_3D, b3_vector3D;

typedef struct b3_vector64
{
	b3_f64    x;
	b3_f64    y;
	b3_f64    z;
} b3_dVector, b3_dVector3D;

typedef struct b3_vector32_4D
{
	b3_f32    x;
	b3_f32    y;
	b3_f32    z;
	b3_f32    w;
} b3_fVector4D,b3_vector4D;

typedef struct b3_vector64_4D
{
	b3_f64    x;
	b3_f64    y;
	b3_f64    z;
	b3_f64    w;
} b3_dVector4D;

typedef struct b3_matrix32
{
	b3_f32    m11,m12,m13,m14;
	b3_f32    m21,m22,m23,m24;
	b3_f32    m31,m32,m33,m34;
	b3_f32    m41,m42,m43,m44;
} b3_matrix;

struct b3_matrix64
{
	b3_f64    m11,m12,m13,m14;
	b3_f64    m21,m22,m23,m24;
    b3_f32    m31,m32,m33,m34;
    b3_f32    m41,m42,m43,m44;
};

typedef struct b3_line32
{
	b3_vector pos,dir;
} b3_line;

typedef struct b3_line64
{
	b3_vector64 pos,dir;
} b3_dLine;

struct b3_pos
{
	b3_s32    x,y;
};

struct b3_color
{
	b3_f32    a,r,g,b;
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
#define B3_MIN(a,b)         ((a) < (b) ? (a) : (b))
#define B3_MAX(a,b)         ((a) < (b) ? (b) : (a))
#define B3_LIMIT(v,min,max) { if ((v) < (min)) (v) = (min); if ((v) > (max)) (v) = (max); }
#define B3_RAD(a)           ((a) * M_PI / 180.0)
#define B3_DEGREE(a)        ((a) * 180.0 / M_PI)

#endif
