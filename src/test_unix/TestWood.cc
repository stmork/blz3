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
#include "blz3/raytrace/b3MaterialSampler.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**  $Log$
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

#define WOOD_RES   200

class b3WoodSampler : public b3MaterialSampler
{
public:
	b3WoodSampler(b3Tx *tx) : b3MaterialSampler(tx)
	{
		// Init material
		m_Material = new b3MatWood(WOOD);
		m_Material->b3Prepare();
	}
	
	virtual ~b3WoodSampler()
	{
		delete m_Material;
	}
};

int main(int argc,char *argv[])
{
	b3Display   *display;

	b3RaytracingItems::b3Register();
	try
	{
		b3Tx tx;
		b3_res xMax,yMax;
		
		// Create display
		display = new b3DisplayView(WOOD_RES,WOOD_RES,"Wood");
		display->b3GetRes(xMax,yMax);
		
		tx.b3AllocTx(xMax,yMax,24);
		
		b3WoodSampler sampler(&tx);

		sampler.b3Sample();

		// We want to see the computed picture until we make input
		// into the display window.
		display->b3PutTx(&tx);
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
