/*
**
**	$Filename:      b3Light.cc $
**	$Release:       Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Raytracing light sources
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/raytrace/b3Raytrace.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.3  2001/08/20 14:16:48  sm
**      - Putting data into cmaera and light combobox.
**      - Selecting camera and light.
**
**      Revision 1.2  2001/08/02 15:37:17  sm
**      - Now we are able to draw Blizzard Scenes with OpenGL.
**
**      Revision 1.1.1.1  2001/07/01 12:24:59  sm
**      Blizzard III is born
**
*/

void b3InitLight::b3Init()
{
	b3PrintF (B3LOG_DEBUG,"Registering light sources...\n");

	b3Item::b3Register(&b3Light::b3Init, &b3Light::b3Init, POINT_LIGHT );
	b3Item::b3Register(&b3Light::b3Init, &b3Light::b3Init, AREA_LIGHT );
	b3Item::b3Register(&b3Light::b3Init, &b3Light::b3Init, SPOT_LIGHT );
}

b3Light::b3Light(b3_u32 class_type) : b3Item(sizeof(b3Light),class_type)
{
}

b3Light::b3Light(b3_u32 *src) : b3Item(src)
{
	b3_index i;

	b3InitVector(&Position);
	b3InitColor(&Color);
	Distance = b3InitFloat();
	Flags    = b3InitInt();
Name[0] = 0;
return;
	if (b3GetClassType() >= AREA_LIGHT)
	{
		Size     = b3InitFloat();
		JitterEdge = b3InitInt();
		if (b3GetClassType() >= SPOT_LIGHT)
		{
			b3InitVector(&Direction);

			b3InitSpline(&Spline,Controls,Knots);
			for (i = 0;i < B3_MAX_KNOTS;i++)    Knots[i] = b3InitFloat();
			for (i = 0;i < B3_MAX_CONTROLS;i++) b3InitVector(&Controls[i]);
		}
	}

	if (Offset > 0)
	{
		b3InitString(Name,size - (parseIndex << 2));
		b3PrintF(B3LOG_DEBUG,"Light %s loaded.\n","Scheisse");
	}
	else
	{
		b3PrintF(B3LOG_DEBUG,"Unnamed light loaded.\n");
	}
}
