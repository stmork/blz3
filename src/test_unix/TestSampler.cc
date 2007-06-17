/*
**
**	$Filename:	TestSampler.cc $
**	$Release:	Dortmund 2004 $
**	$Revision: 1493 $
**	$Date: 2007-06-15 14:25:44 +0200 (Fr, 15 Jun 2007) $
**	$Author: smork $
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

#include "blz3/system/b3DisplayView.h"
#include "blz3/raytrace/b3BumpSampler.h"
#include "blz3/raytrace/b3MaterialSampler.h"

/*************************************************************************
**                                                                      **
**                        b3TestWood implementation                     **
**                                                                      **
*************************************************************************/

#define WOOD_RES   400
#define no_CREATE_ICON

static b3_vector scale =
{
  1.0,
  1.0,
  1.0
};

class b3MatWoodSampler : public b3MaterialSampler
{
public:
	b3MatWoodSampler(b3Tx *tx) : b3MaterialSampler(tx, &scale)
	{
		// Init material
		m_Material = new b3MatWood(WOOD);
		m_Material->b3Prepare(null);
	}
	
	virtual ~b3MatWoodSampler()
	{
		delete m_Material;
	}
};

class b3MatOakPlankSampler : public b3MaterialSampler
{
public:
	b3MatOakPlankSampler(b3Tx *tx) : b3MaterialSampler(tx, &scale, 1)
	{
		b3MatOakPlank *material = new b3MatOakPlank(OAKPLANK);

#ifdef CREATE_ICON
		material->m_LightWood = material->m_DarkWood * 2.0;
		material->m_xScale *= 7;
		material->m_yScale *= 8;
		material->m_xOffset = 0.52;
		material->m_RingFrequency *= 0.1;
#else
		material->m_xScale *= 2;
		material->m_yScale *= 2;
#endif

		// Init material
		m_Material = material;
		m_Material->b3Prepare(null);
	}
	
	virtual ~b3MatOakPlankSampler()
	{
		delete m_Material;
	}
};

class b3BumpWoodSampler : public b3BumpSampler
{
public:
	b3BumpWoodSampler(b3Tx *tx) : b3BumpSampler(tx, &scale, 1)
	{
		// Init material
		m_Bump = (b3Bump *)b3World::b3AllocNode(BUMP_WATER);
		m_Bump->b3Prepare(null);
	}
	
	virtual ~b3BumpWoodSampler()
	{
		delete m_Bump;
	}
};

class b3BumpOakPlankSampler : public b3BumpSampler
{
public:
	b3BumpOakPlankSampler(b3Tx *tx) : b3BumpSampler(tx, &scale, 1)
	{
		b3BumpOakPlank *bump = new b3BumpOakPlank(BUMP_OAKPLANK);

		bump->m_xScale *= 4;
		bump->m_yScale *= 4;

		// Init material
		m_Bump = bump;
		m_Bump->b3Prepare(null);
	}
	
	virtual ~b3BumpOakPlankSampler()
	{
		delete m_Bump;
	}
};

int main(int argc,char *argv[])
{
	b3Display   *display;

	b3RaytracingItems::b3Register();
	try
	{
		b3Tx   tx;
		b3_res xMax,yMax;
		
		// Create display
		display = new b3DisplayView(WOOD_RES,WOOD_RES,"Wood");
		display->b3GetRes(xMax,yMax);
		
		tx.b3AllocTx(xMax, yMax, 128);
		
		b3MatWoodSampler      sampler(&tx);
//		b3MatOakPlankSampler  sampler(&tx);
//		b3BumpWoodSampler     sampler(&tx);
//		b3BumpOakPlankSampler sampler(&tx);

		sampler.b3Sample();

		// We want to see the computed picture until we make input
		// into the display window.
		display->b3PutTx(&tx);
		display->b3Wait();

		if (argc > 1)
		{
#ifdef CREATE_ICON
			if (argc > 3)
			{
				b3Tx small,big;

				small.b3AllocTx(32,32,24);
				small.b3ScaleToGrey(&tx);
				small.b3SaveTGA(argv[2]);
			
				big.b3AllocTx(48,48,24);
				big.b3ScaleToGrey(&tx);
				big.b3SaveTGA(argv[3]);
			}
			tx.b3SaveTGA(argv[1]);
#else
			tx.b3SaveJPEG(argv[1]);
#endif
		}

		// Delete Display
		delete display;
	}
	catch(b3DisplayException &e)
	{
		b3PrintF(B3LOG_NORMAL,"### Error occured: %s\n",e.b3GetErrorMsg());
	}
	return EXIT_SUCCESS;
}
