/*
**
**	$Filename:      b3Complex.h $
**	$Release:       Dortmund 2005 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Complex datatype
**
**	(C) Copyright  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

#ifndef B3_BASE_COMPLEX_H
#define B3_BASE_COMPLEX_H

#include "blz3/b3Config.h"

template<class T> class b3Complex
{
	T B3_ALIGN_16 v[2];

public:
	enum
	{
		Re = 0,
		Im = 1
	};

	inline b3Complex<T>()
	{
	}

	inline b3Complex<T>(const b3Complex<T> &complex)
	{
		for(b3_loop i = 0;i < 2;i++)
		{
			v[i] = complex.v[i];
		}
	}

	inline b3Complex<T>(const T re,const T im = 0)
	{
		v[Re] = re;
		v[Im] = im;
	}

	inline b3Complex<T> operator+=(const b3Complex<T> &a)
	{
		for(b3_loop i = 0;i < 2;i++)
		{
			v[i] += a.v[i];
		}

		return *this;
	}

	inline b3Complex<T> &operator-=(const b3Complex<T> &a)
	{
		for(b3_loop i = 0;i < 2;i++)
		{
			v[i] -= a.v[i];
		}

		return *this;
	}

	inline b3Complex<T> &operator*=(const b3Complex<T> &a)
	{
		T B3_ALIGN_16 val[2];

		val[Re] = v[Re] * a.v[Re] - v[Im] * a.v[Im];
		val[Im] = v[Im] * a.v[Re] + v[Re] * a.v[Im];

		for(b3_loop i = 0;i < 2;i++)
		{
			v[i] = val[i];
		}

		return *this;
	}

	inline b3Complex<T> &operator+=(const T a)
	{
		v[Re] += a;

		return *this;
	}

	inline b3Complex<T> &operator-=(const T a)
	{
		v[Re] -= a;

		return *this;
	}

	inline b3Complex<T> &operator*=(const T a)
	{
		for(b3_loop i = 0;i < 2;i++)
		{
			v[i] *= a;
		}

		return *this;
	}

	inline b3Complex<T> operator/=(const T a)
	{
		for(b3_loop i = 0;i < 2;i++)
		{
			v[i] /= a;
		}

		return *this;
	}

	inline b3Complex<T> &operator+(const b3Complex<T> &a)
	{
		b3Complex<T> result;

		for(b3_loop i = 0;i < 2;i++)
		{
			result.v[i] = v[i] + a.v[i];
		}
		return result;
	}

	inline b3Complex<T> &operator-(const b3Complex<T> &a)
	{
		b3Complex<T> result;

		for(b3_loop i = 0;i < 2;i++)
		{
			result.v[i] = v[i] - a.v[i];
		}
		return result;
	}

	inline b3Complex<T> &operator*(const b3Complex<T> &a)
	{
		return b3Complex<T>(
			v[Re] * a.v[Re] - v[Im] * a.v[Im],
			v[Im] * a.v[Re] + v[Re] * a.v[Im]);
	}

	inline T b3Length()
	{
		return sqrt(b3Square());
	}

	inline T b3SquareLength()
	{
		T B3_ALIGN_16 val[2];

		for(b3_loop i = 0;i < 2;i++)
		{
			val[i] = v[i] * v[i];
		}
		return val[Re] + val[Im];
	}

	inline void b3Square()
	{
		T B3_ALIGN_16 re[2];

		for (b3_loop i = 0;i < 2;i++)
		{
			re[i] = v[i] * v[i];
		}
		v[Im]  = v[Re] * v[Im];
		v[Im] += v[Im];
		v[Re]  = re[0] - re[1];
	}
}; 

#endif
