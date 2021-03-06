/*
**
**	$Filename:	bench.cc $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Benchmarker
**
**      (C) Copyright 2002  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/b3Config.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.6  2005/07/01 16:27:43  sm
**	- Format fix.
**
**	Revision 1.5  2004/11/29 09:58:01  smork
**	- Changed exit states to correct defines.
**	- Added switch for disabling VBO in OpenGL renderer.
**	- Added switches for logging level in OpenGL renderer as in brt3.
**
**	Revision 1.4  2004/11/23 08:13:32  smork
**	- Added configure support.
**
**	Revision 1.3  2002/12/20 15:32:55  sm
**	- Made some ICC optimazations :-)
**
**	Revision 1.2  2002/11/18 17:30:01  sm
**	- GCC patch level define fix.
**
**	Revision 1.1  2002/11/16 14:24:00  sm
**	- Added a CPU benchmark
**	- Removed system dependend #IF from raytracing
**
**
*/

#define MAX   1000000
#if 1
#	define DIM   256
#	define FTYPE b3_f32
#else
#	define DIM   128
#	define FTYPE b3_f64
#endif


template<class TYPE, int COUNT> class vector
{
private:
	TYPE f[COUNT];

public:
	inline void clear()
	{
		init(0.0);
	}

	inline void init(TYPE value)
	{
		for (int i = 0; i < COUNT; i++)
		{
			f[i] = value;
		}
	}

	inline void add(vector<TYPE, COUNT> & src, TYPE value)
	{
		for (int i = 0; i < COUNT; i++)
		{
			f[i] += value;
		}
	}

	inline void sub(vector<TYPE, COUNT> & src, TYPE value)
	{
		for (int i = 0; i < COUNT; i++)
		{
			f[i] -= value;
		}
	}

	inline void add(vector<TYPE, COUNT> & src)
	{
		for (int i = 0; i < COUNT; i++)
		{
			f[i] += src.f[i];
		}
	}

	inline void sub(vector<TYPE, COUNT> & src)
	{
		for (int i = 0; i < COUNT; i++)
		{
			f[i] -= src.f[i];
		}
	}

	inline void mult(vector<TYPE, COUNT> & src)
	{
		for (int i = 0; i < COUNT; i++)
		{
			f[i] *= src.f[i];
		}
	}

	inline void comb1(vector<TYPE, COUNT> & a, vector<TYPE, COUNT> & b)
	{
		for (int i = 0; i < COUNT; i++)
		{
			f[i] = (f[i] + a.f[i]) * b.f[i];
		}
	}

	inline void comb2(vector<TYPE, COUNT> & a, vector<TYPE, COUNT> & b)
	{
		for (int i = 0; i < COUNT; i++)
		{
			f[i] = ((f[i] + a.f[i]) * b.f[i] + a.f[i]) * b.f[i];
		}
	}

	inline void comb4(vector<TYPE, COUNT> & a, vector<TYPE, COUNT> & b)
	{
		for (int i = 0; i < COUNT; i++)
		{
			f[i] = ((((f[i] + a.f[i]) * b.f[i] + a.f[i]) * b.f[i] + a.f[i]) * b.f[i] + a.f[i]) * b.f[i];
		}
	}

	inline void comb8(vector<TYPE, COUNT> & a, vector<TYPE, COUNT> & b)
	{
		for (int i = 0; i < COUNT; i++)
		{
			f[i] = ((((((((f[i] + a.f[i]) * b.f[i] + a.f[i]) * b.f[i] + a.f[i]) * b.f[i] + a.f[i]) * b.f[i] + a.f[i]) * b.f[i] + a.f[i]) * b.f[i] + a.f[i]) * b.f[i] + a.f[i]) * b.f[i];
		}
	}

	inline void combx(vector<TYPE, COUNT> & a, vector<TYPE, COUNT> & b, int max)
	{
		int k;

		for (int i = 0; i < COUNT; i++)
		{
			TYPE val = f[i], aVal = a.f[i], bVal = b.f[i];

			for (k = 0; k < max; k++)
			{
				val = (val + aVal) * bVal;
			}
			f[i] = val;
		}
	}

	inline TYPE result(int i = 0)
	{
		return f[i];
	}
};

class b3Test
{
	b3_count m_Max;

public:
	b3_f64   m_MFlop;
	b3_f64   m_Result;

public:
	void b3Init(b3_count max)
	{
		m_Max = max;
	}

	static bool run_test_a(void * ptr)
	{
		b3Test      *      test = (b3Test *)ptr;
		vector<FTYPE, DIM>  a, b, c;
		b3_count           max = test->m_Max;

		for (int k = 0; k < max; k++)
		{
			a.init(0.1);
			b.init(0.0005);
			c.init(1.00005);
			for (int i = 0; i < MAX; i++)
			{
				a.add(b);
				a.mult(c);
			}
		}
		test->m_MFlop  = (b3_f64)(MAX * DIM * 4 + 3 * DIM) * max / 1000000;
		test->m_Result = a.result(0);
		return 0;
	}

	static bool run_test_b(void * ptr)
	{
		b3Test      *      test = (b3Test *)ptr;
		vector<FTYPE, DIM>  a, b, c;
		b3_count           max = test->m_Max;

		for (int k = 0; k < max; k++)
		{
			a.init(0.1);
			b.init(0.0005);
			c.init(1.00005);

			a.combx(b, c, MAX);
		}
		test->m_MFlop  = (b3_f64)(MAX * DIM * 4 + 3 * DIM) * max / 1000000;
		test->m_Result = a.result(0);
		return 0;
	}

	static bool run_test_c(void * ptr)
	{
		b3Test      *      test = (b3Test *)ptr;
		vector<FTYPE, DIM>  a, b, c;
		b3_count           max = test->m_Max;

		for (int k = 0; k < max; k++)
		{
			a.init(0.1);
			b.init(0.0005);
			c.init(1.00005);
			for (int i = 0; i < (MAX / 8); i++)
			{
				a.comb8(b, c);
			}
		}
		test->m_MFlop  = (b3_f64)(MAX * DIM * 4 + 3 * DIM) * max / 1000000;
		test->m_Result = a.result(0);
		return 0;
	}

	static bool run_test_d(void * ptr)
	{
		b3Test       *      test = (b3Test *)ptr;
		vector<b3_f64, DIM>  a, b, c;
		b3_count            max = test->m_Max;

		for (int k = 0; k < max; k++)
		{
			a.init(0.1);
			b.init(0.0005);
			c.init(1.00005);
			for (int i = 0; i < (MAX / 8); i++)
			{
				a.comb8(b, c);
			}
		}
		test->m_MFlop  = (b3_f64)(MAX * DIM * 4 + 3 * DIM) * max / 1000000;
		test->m_Result = a.result(0);
		return 0;
	}
};

class b3Suite
{
	b3_count    CPUs;
	b3_count    m_Max;
	b3Thread  * threads;
	b3Test   *  test;

public:
	b3Suite(b3_count max)
	{
		CPUs  = b3Runtime::b3GetNumCPUs();
		m_Max = max;
	}

	void b3Execute(bool(*func)(void *))
	{
		b3TimeSpan  span;
		b3_count    i;
		b3_f64      mflops = 0, used;

		threads = new b3Thread[CPUs];
		test    = new b3Test[CPUs];

		for (i = 0; i < CPUs; i++)
		{
			test[i].b3Init(m_Max);
		}

		span.b3Start();
		for (i = 0; i < CPUs; i++)
		{
			threads[i].b3Start(func, &test[i]);
		}

		for (i = 0; i < CPUs; i++)
		{
			threads[i].b3Wait();
			threads[i].b3AddTimeSpan(&span);
		}
		span.b3Stop();
		//		span.b3Print();

		used = span.m_uTime / CPUs;
		for (i = 0; i < CPUs; i++)
		{
			mflops += test[i].m_MFlop / used;
		}
		printf("MFlop/s: %5.3f [%g]\n", mflops, test[0].m_Result);

		delete [] threads;
		delete [] test;
	}
};

int main(int argc, char * argv[])
{
	if (argc > 1)
	{
		b3_count max = 1;

		sscanf(argv[1], "%d", &max);

		b3Suite suite(max);

		suite.b3Execute(b3Test::run_test_a);
		suite.b3Execute(b3Test::run_test_b);
		suite.b3Execute(b3Test::run_test_c);
		suite.b3Execute(b3Test::run_test_d);
	}
	else
	{
		printf("USAGE:\n");
		printf("%s loopcount\n", argv[0]);
	}

	return EXIT_SUCCESS;
}
