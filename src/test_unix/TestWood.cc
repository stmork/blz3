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

#include "blz3/system/b3DisplayView.h"
#include "blz3/raytrace/b3BumpSampler.h"
#include "blz3/raytrace/b3MaterialSampler.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**  $Log$
**  Revision 1.9  2004/04/19 09:00:52  sm
**  - Added bump sampler.
**  - Reactivated bump sampler in bump dialogs.
**
**  Revision 1.8  2004/04/11 14:05:11  sm
**  - Raytracer redesign:
**    o The reflection/refraction/ior/specular exponent getter
**      are removed. The values are copied via the b3GetColors()
**      method.
**    o The polar members are renamed.
**    o The shape/bbox pointers moved into the ray structure
**  - Introduced wood bump mapping.
**
**  Revision 1.7  2004/04/10 13:45:30  sm
**  - Added wooden oak planks.
**
**  Revision 1.6  2004/04/09 14:11:58  sm
**  - Removed CRs
**
**  Revision 1.5  2004/04/09 11:09:01  sm
**  - Removed any display reference from sampler
**
**  Revision 1.4  2004/04/09 08:49:16  sm
**  - Splitted up sampler for Lines use and capable for
**    using other metherials.
**
**  Revision 1.3  2004/04/08 14:34:42  sm
**  - Multithreading support for wood example.
**
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

#define WOOD_RES   320
#define no_CREATE_ICON

class b3MatWoodSampler : public b3MaterialSampler
{
public:
	b3MatWoodSampler(b3Tx *tx) : b3MaterialSampler(tx)
	{
		// Init material
		m_Material = new b3MatWood(WOOD);
		m_Material->b3Prepare();
	}
	
	virtual ~b3MatWoodSampler()
	{
		delete m_Material;
	}
};

class b3MatOakPlankSampler : public b3MaterialSampler
{
public:
	b3MatOakPlankSampler(b3Tx *tx) : b3MaterialSampler(tx,1)
	{
		b3MatOakPlank *material = new b3MatOakPlank(OAKPLANK);

#ifdef CREATE_ICON
		material->m_LightWood = material->m_DarkWood * 2.0;
		material->m_xScale *= 7;
		material->m_yScale *= 8;
		material->m_xOffset = 0.52;
		material->m_RingFrequency *= 0.1;
#else
		material->m_xScale *= 4;
		material->m_yScale *= 4;
#endif

		// Init material
		m_Material = material;
		m_Material->b3Prepare();
	}
	
	virtual ~b3MatOakPlankSampler()
	{
		delete m_Material;
	}
};

class b3BumpWoodSampler : public b3BumpSampler
{
public:
	b3BumpWoodSampler(b3Tx *tx) : b3BumpSampler(tx)
	{
		// Init material
		m_Bump = new b3BumpWood(BUMP_WOOD);
		m_Bump->b3Prepare();
	}
	
	virtual ~b3BumpWoodSampler()
	{
		delete m_Bump;
	}
};

class b3BumpOakPlankSampler : public b3BumpSampler
{
public:
	b3BumpOakPlankSampler(b3Tx *tx) : b3BumpSampler(tx,1)
	{
		b3BumpOakPlank *bump = new b3BumpOakPlank(BUMP_OAKPLANK);

		bump->m_xScale *= 4;
		bump->m_yScale *= 4;

		// Init material
		m_Bump = bump;
		m_Bump->b3Prepare();
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
		
		tx.b3AllocTx(xMax,yMax,24);
		
//		b3MatWoodSampler      sampler(&tx);
//		b3MatOakPlankSampler  sampler(&tx);
		b3BumpWoodSampler     sampler(&tx);
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
	return 0;
}
