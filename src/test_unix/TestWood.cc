/*
**
**	$Filename:	TestWood.cc $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Displaying Wood
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Lines III includes                            **
**                                                                      **
*************************************************************************/

#include "blz3/b3Config.h"
#include "blz3/system/b3DisplayView.h"
#include "blz3/base/b3Procedure.h"
#include "blz3/image/b3Tx.h"
#include "blz3/raytrace/b3Raytrace.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**  $Log$
**  Revision 1.2  2004/04/06 12:17:46  sm
**  - Optimized some noise methods.
**
**  Revision 1.1  2004/04/05 09:16:03  sm
**  - Added test wood for Lines wood dialog
**  - Optimized noise a little bit.
**
**
*/

/*************************************************************************
**                                                                      **
**                        b3TestWood implementation                     **
**                                                                      **
*************************************************************************/

#define WOOD_RES   800
#define WOOD_TILES   3

class b3WoodSampler
{
	b3MatWood *m_Material;
	b3Display *m_Display;
	b3_res     m_xMax;
	b3_res     m_yMax;
	b3_vector  m_Start;
	b3_vector  m_End;
	b3_vector  m_Step;

public:
	b3WoodSampler(b3Display *display)
	{
		// Init display
		m_Display = display;
		m_Display->b3GetRes(m_xMax,m_yMax);
		
		// Init material
		m_Material = new b3MatWood(WOOD);
		m_Material->b3Prepare();
		
		// Init box polar bound
		b3Vector::b3Init(&m_Start, 0.0, 1.0, 0.5);
		b3Vector::b3Init(&m_End,   1.0, 0.0, 0.8);
		m_Step.x = (m_End.x - m_Start.x) / m_xMax;
		m_Step.y = (m_End.y - m_Start.y) / m_yMax;
		m_Step.z = (m_End.z - m_Start.z) / m_yMax;
	}

	~b3WoodSampler()
	{
		delete m_Material;
	}

	void b3Sample()
	{
		b3_coord      x,y;
		b3_polar      polar;
		b3Color       ambient,diffuse,specular;
		b3Tx          tx;
		b3_f64        fy;
		b3_pkd_color *data;
		b3TimeSpan    span;

		tx.b3AllocTx(m_xMax,m_yMax,24);
		data = (b3_pkd_color *)tx.b3GetData();

		span.b3Start();
		for (y = 0;y < m_yMax;y++)
		{
			fy = (b3_f64)y / m_yMax;
			for (x = 0;x < m_xMax;x++)
			{
				int ix = (WOOD_TILES * x) / m_xMax;
				polar.box_polar.x = fmod((b3_f64)x * WOOD_TILES / m_xMax,1.0);
				polar.box_polar.y = 1.0 - fy;
				polar.box_polar.z = 1.0 - fy * 0.15 * ix;

				m_Material->b3GetColors(&polar,diffuse,ambient,specular);
				*data++ = diffuse;
			}
		}
		span.b3Stop();
		span.b3Print();

		m_Display->b3PutTx(&tx);
	}
};

int main(int argc,char *argv[])
{
	b3Display   *display;

	b3RaytracingItems::b3Register();
	try
	{
		// Create display
		display = new b3DisplayView(WOOD_RES,WOOD_RES,"Wood");
		b3WoodSampler sampler(display);

		sampler.b3Sample();

		// We want to see the computed picture until we make input
		// into the display window.
		display->b3Wait();

		// Delete Display
		delete display;
	}
	catch(b3DisplayException &e)
	{
		b3PrintF(B3LOG_NORMAL,"### Error occured: %s\n",e.b3GetErrorMsg());
	}
	return 0;
}
