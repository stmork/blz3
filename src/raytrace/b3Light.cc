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
**      Revision 1.6  2001/10/03 18:46:45  sm
**      - Adding illumination and recursive raytracing
**
**      Revision 1.5  2001/09/30 15:53:19  sm
**      - Removing nasty CR/LF
**
**      Revision 1.4  2001/09/01 15:54:54  sm
**      - Tidy up Size confusion in b3Item/b3World and derived classes
**      - Made (de-)activation of objects
**
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

	b3InitVector(&m_Position);
	b3InitColor(&m_Color);
	m_Distance = b3InitFloat();
	m_Flags    = b3InitInt();

	if (b3GetClassType() >= AREA_LIGHT)
	{
		m_Size       = b3InitFloat();
		m_JitterEdge = b3InitInt();
		if (b3GetClassType() >= SPOT_LIGHT)
		{
			b3InitVector(&m_Direction);

			b3InitSpline(&m_Spline,m_Controls,m_Knots);
			for (i = 0;i < B3_MAX_KNOTS;i++)    m_Knots[i] = b3InitFloat();
			for (i = 0;i < B3_MAX_CONTROLS;i++) b3InitVector(&m_Controls[i]);
		}
	}

	if (m_ItemOffset > 0)
	{
		b3InitString(m_Name,m_ItemSize - (m_ParseIndex << 2));
		b3PrintF(B3LOG_DEBUG,"Light %s loaded.\n",m_Name);
	}
	else
	{
		strcpy(m_Name,"Light");
		b3PrintF(B3LOG_DEBUG,"Unnamed light loaded.\n");
	}
}
