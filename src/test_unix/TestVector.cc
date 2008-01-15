/*
**
**      $Filename:      TestVector.cc $
**      $Release:       Dortmund 2002 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Test routines for vector computing
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
#include "blz3/base/b3Array.h"
#include "blz3/base/b3Matrix.h"
#include "blz3/base/b3Vector.h"

/*************************************************************************
**                                                                      **
**                        implementation                                **
**                                                                      **
*************************************************************************/

#define MAX_DIM   1000
#define RAN_MAX   1024

class b3VectorTestSuite
{
	b3Array<const char *> m_TestComment;
	b3Array<b3_f64>       m_TestResult;

public:
	virtual ~b3VectorTestSuite() {}
	virtual b3_f64 b3Length() = 0;
	virtual b3_f64 b3Test() = 0;

	void b3PrintResult()
	{
		for (int i = 0;i < m_TestResult.b3GetCount();i++)
		{
			b3PrintF(B3LOG_NORMAL,"%2d: %3.5f - %s\n",i,m_TestResult[i],m_TestComment[i]);
		}
	}

protected:
	b3_f64 b3Random()
	{
		return B3_FRAN(RAN_MAX) + RAN_MAX * 0.5;
	}

	void b3Add(const char *text)
	{
		m_TestComment.b3Add(text);
		m_TestResult.b3Add(b3Length());
	}
};

/*************************************************************************
**                                                                      **
**                        Testing vector struct                         **
**                                                                      **
*************************************************************************/

class b3VectorArray;

class b3VectorStruct : public b3VectorTestSuite
{
	friend class b3VectorArray;
protected:
	b3_vector m_Array[MAX_DIM];

public:
	b3VectorStruct()
	{
		for (int i = 0;i < MAX_DIM;i++)
		{
			b3Vector::b3Init(&m_Array[i],b3Random(),b3Random(),b3Random());
		}
		b3Add("constructor");
	}

	virtual ~b3VectorStruct()
	{
	}

	inline b3_f64 b3Length()
	{
		b3_f64 length = 0;

		for (int i = 0;i < MAX_DIM;i++)
		{
			length += b3Vector::b3Length(&m_Array[i]);
		}
		return length;
	}

	inline b3_f64 b3Test()
	{
		b3_vector aux;
		int       k,max;

		b3Vector::b3Init(&m_Array[20]);
		for (k = 0;k < 20;k+=3)
		{
			b3Vector::b3Mul(&m_Array[k + 1],&m_Array[k + 2],&aux);
			b3Vector::b3Add(&m_Array[k],&m_Array[20]);
			b3Vector::b3Sub(&aux,&m_Array[20]);
		}
		b3Add("+/-/*");

		b3Vector::b3Scale(&m_Array[20], &m_Array[20], 1.0 / 1024.0);
		b3Add("/= 1024");

		b3Vector::b3Scale(&m_Array[20],0.25);
		b3Add("*= 0.25");

		while (k < 40)
		{
			b3Vector::b3CrossProduct(&m_Array[k],&m_Array[k+1],&m_Array[k+2]);
			k += 3;
		}
		b3Add("cross product");

		max = k + k;
		for (k = 0;k < max;k+=2)
		{
			b3Vector::b3Init(
				&m_Array[k + max],
				b3Vector::b3Distance(&m_Array[k],&m_Array[k+1]),
				b3Vector::b3SMul(&m_Array[k],&m_Array[k+1]),
				b3Vector::b3Length(&m_Array[k]));
		}
		b3Add("distance/sMul/length");

		for (k = 0;k < MAX_DIM;k++)
		{
			b3Vector::b3SetMinimum(&m_Array[k],-RAN_MAX * 0.5);
			b3Vector::b3SetMaximum(&m_Array[k], RAN_MAX * 0.5);
		}
		b3Add("minimum/maximum");

		b3Vector::b3InitBound(&m_Array[MAX_DIM - 2],&m_Array[MAX_DIM - 1]);
		for (k = 0;k < (MAX_DIM - 2);k++)
		{
			b3Vector::b3AdjustBound(&m_Array[k],&m_Array[MAX_DIM - 2],&m_Array[MAX_DIM - 1]);
		}
		b3Add("check bound");

		for (k = 0;k < MAX_DIM;k+=4)
		{
			b3Vector::b3LinearCombine(
				&m_Array[k + 1],
				&m_Array[k + 2],
				&m_Array[k + 3],
				m_Array[k].x,
				m_Array[k].y,
				&m_Array[k]);
		}
		b3Add("linear combination");

		for (k = 0;k < MAX_DIM;k++)
		{
			b3Vector::b3Normalize(&m_Array[k],k);
		}
		b3Add("normalize(k)");

		return b3Length();
	}
};

/*************************************************************************
**                                                                      **
**                        Testing vector array                          **
**                                                                      **
*************************************************************************/

class b3VectorArray : public b3VectorTestSuite
{
	b3Vector32 m_Array[MAX_DIM];

public:
	b3VectorArray(b3VectorStruct &vStruct)
	{
		for (int i = 0;i < MAX_DIM;i++)
		{
			m_Array[i] = b3Vector32(vStruct.m_Array[i]);
		}
		b3Add("constructor");
	}

	virtual ~b3VectorArray()
	{
	}

	inline b3_f64 b3Length()
	{
		b3_f64 length = 0;

		for (int i = 0;i < MAX_DIM;i++)
		{
			length += m_Array[i].b3Length();
		}
		return length;
	}

	inline b3_f64 b3Test()
	{
		int k,max;

		m_Array[20].b3Zero();
		for (k = 0;k < 20;k+=3)
		{
			m_Array[20] = m_Array[20] + m_Array[k] - m_Array[k + 1] * m_Array[k + 2];
		}
		b3Add("+/-/*");

		m_Array[20] /= 1024.0;
		b3Add("/= 1024");

		m_Array[20] *=    0.25;
		b3Add("*= 0.25");

		while (k < 40)
		{
			m_Array[k+2] = b3Vector32::b3CrossProduct(m_Array[k],m_Array[k+1]);
			k += 3;
		}
		b3Add("cross product");

		max = k + k;
		for (k = 0;k < max;k+=2)
		{
			m_Array[k + max].b3Init(
				b3Vector32::b3Distance(m_Array[k],m_Array[k+1]),
				b3Vector32::b3SMul(m_Array[k],m_Array[k+1]),
				m_Array[k].b3Length());
		}
		b3Add("distance/sMul/length");

		for (k = 0;k < MAX_DIM;k++)
		{
			m_Array[k].b3SetMinimum(-RAN_MAX * 0.5);
			m_Array[k].b3SetMaximum( RAN_MAX * 0.5);
		}
		b3Add("minimum/maximum");

		b3Vector32::b3InitBound(m_Array[MAX_DIM - 2],m_Array[MAX_DIM - 1]);
		for (k = 0;k < (MAX_DIM - 2);k++)
		{
			m_Array[k].b3AdjustBound(m_Array[MAX_DIM - 2],m_Array[MAX_DIM - 1]);
		}
		m_Array[MAX_DIM - 2].b3Print("Lower:");
		m_Array[MAX_DIM - 1].b3Print("Upper:");
		b3Add("check bound");

		for (k = 0;k < MAX_DIM;k+=4)
		{
			m_Array[k] = m_Array[k + 1] + m_Array[k + 2] * m_Array[k][X] + m_Array[k + 3] * m_Array[k][Y];
		}
		b3Add("linear combination");

		for (k = 0;k < MAX_DIM;k++)
		{
			m_Array[k].b3Normalize(k);
		}
		b3Add("normalize(k)");

		return b3Length();
	}
};

int main(int argc,char *argv[])
{
	b3VectorStruct vStruct;
	b3VectorArray  vArray(vStruct);
	b3_f64         a,b;

	a = vStruct.b3Test();
	b = vArray.b3Test();
	if (a != b)
	{
		b3PrintF(B3LOG_NORMAL,"Test failed (distance: %3.5f).\n",fabs(a - b));
		vStruct.b3PrintResult();
		vArray.b3PrintResult();
	}
	return EXIT_SUCCESS;
}
