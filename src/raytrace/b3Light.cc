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
**      Revision 1.7  2001/10/05 20:30:45  sm
**      - Introducing Mork and Phong shading.
**      - Using light source when shading
**
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

static b3_f64 Limit = 0.001;

void b3InitLight::b3Init()
{
	b3PrintF (B3LOG_DEBUG,"Registering light sources...\n");

	b3Item::b3Register(&b3Light::b3Init, &b3Light::b3Init, POINT_LIGHT );
	b3Item::b3Register(&b3Light::b3Init, &b3Light::b3Init, AREA_LIGHT );
	b3Item::b3Register(&b3Light::b3Init, &b3Light::b3Init, SPOT_LIGHT );
}

b3Light::b3Light(b3_u32 class_type) : b3Item(sizeof(b3Light),class_type)
{
	b3Init();
}

b3Light::b3Light(b3_u32 *src) : b3Item(src)
{
	b3_index i;

	b3Init();
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
			m_SpotActive = true;
		}
		else
		{
			m_SpotActive = false;
		}
		m_SoftShadow = ((m_Flags & LIGHT_PENUMBRA) != 0);
	}
	else
	{
		m_SoftShadow = false;
	}
	m_LightActive = ((m_Flags & LIGHT_OFF) == 0);

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

#define INIT_DEGREE      3
#define INIT_CONTROL_MAX 7

void b3Light::b3Init()
{
	b3_index i;

	m_Color.r     =      1.0;
	m_Color.g     =      1.0;
	m_Color.b     =      1.0;
	m_Distance    =  50000.0;
	m_Position.x  =  10000.0;
	m_Position.y  = -10000.0;
	m_Position.z  =  10000.0;
	m_Size        =      0.02;
	m_JitterEdge  =      5;
	m_Flags       =      0;
	m_Direction.x =   -100.0;
	m_Direction.y =    100.0;
	m_Direction.z =   -100.0;

	m_Spline.offset    = 1;
	m_Spline.knots     = m_Knots;
	m_Spline.controls  = m_Controls;
	b3BSplineInitCurve (&m_Spline,INIT_DEGREE,INIT_CONTROL_MAX,false);
	for (i = 0;i < INIT_CONTROL_MAX;i++)
	{
		m_Controls[i].x = (b3_f64)i / (INIT_CONTROL_MAX - 1);
		m_Controls[i].y = 1.0;
		m_Controls[i].z = 0;
	}
	m_Controls[INIT_CONTROL_MAX - 1].y = 0.5;

	for (i = INIT_DEGREE;i <= INIT_CONTROL_MAX;i++)
	{
		m_Knots[i] =
			(b3_f64)(i - INIT_DEGREE) /
			(b3_f64)(INIT_CONTROL_MAX - INIT_DEGREE);
	}
	m_Knots[INIT_CONTROL_MAX] += Limit;
	b3BSplineThroughEndControl (&m_Spline);

	strcpy(m_Name,"Light");
	m_LightActive = true;
	m_SoftShadow  = false;
	m_SpotActive  = false;
}

b3_bool b3Light::b3Illuminate(
	b3Scene         *scene,
	b3_illumination *surface)
{
	b3Shape      *Shadow;
	b3_light_info Jit;
	b3_vector     point;
	b3_f64        LightDist,LightFrac,q,UpperBound;

	if (!m_LightActive)
	{
		return false;
	}

	Jit.Distr = 1;
	Jit.Size  = 0;
	Jit.pos.x = surface->incoming->ipoint.x;
	Jit.pos.y = surface->incoming->ipoint.y;
	Jit.pos.z = surface->incoming->ipoint.z;
	Jit.dir.x = m_Position.x - surface->incoming->ipoint.x;
	Jit.dir.y = m_Position.y - surface->incoming->ipoint.y;
	Jit.dir.z = m_Position.z - surface->incoming->ipoint.z;
	LightDist =
		Jit.dir.x * Jit.dir.x +
		Jit.dir.y * Jit.dir.y +
		Jit.dir.z * Jit.dir.z;
	if (LightDist == 0)
	{
		return false;
	}

	// Normalize direction
	LightDist = 1.0 / (UpperBound = sqrt(LightDist));
	Jit.dir.x	*= LightDist;
	Jit.dir.y	*= LightDist;
	Jit.dir.z	*= LightDist;


	// Compute relative brightness via LDC
	// (= light distribution curve)
	if (m_SpotActive)
	{
		LightFrac = -(
			Jit.dir.x * m_Direction.x +
			Jit.dir.y * m_Direction.y +
			Jit.dir.z * m_Direction.z);
		if (LightFrac <= 0)
		{
			q = 1.0 - Limit;
		}
		else
		{
			q = (LightFrac >= 1 ? Limit : acos(LightFrac) * 2.0 / M_PI);
		}

		b3DeBoorOpened (&m_Spline,&point,0,q);
		Jit.LightFrac  = LightDist * m_Distance * point.y;
	}
	else
	{
		Jit.LightFrac = LightDist * m_Distance;
	}
	Jit.LightDist = LightDist;

	scene->b3Intersect(&Jit,UpperBound - Limit);
	Shadow = Jit.shape;
	scene->b3Illuminate(this,&Jit,surface);
	return true;
}
