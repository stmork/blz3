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
**      Revision 1.10  2001/10/07 20:41:32  sm
**      - Updating MSVC project status
**
**      Revision 1.9  2001/10/07 20:17:27  sm
**      - Prepared texture support.
**      - Noise procedures added.
**      - Added bump and material support.
**      - Added soft shadows.
**
**      Revision 1.8  2001/10/06 19:56:00  sm
**      - Fixing bugs concerning reflection and
**        refraction computation (both: direction
**        computation and shading)
**
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
	}

	if (m_LightActive)
	{
		b3PrintF(B3LOG_NORMAL,"  Light \"%s\" is active",m_Name);
		if (m_SoftShadow)
		{
			b3PrintF(B3LOG_NORMAL," and uses soft shadows");
		}
		if (m_SpotActive)
		{
			b3PrintF(B3LOG_NORMAL," and uses light distribution curve");
		}
		b3PrintF(B3LOG_NORMAL,".\n");
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
	m_Size        =      0.02f;
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
	m_Knots[INIT_CONTROL_MAX] += epsilon;
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
	if (!m_LightActive)
	{
		return false;
	}
	return (m_SoftShadow ?
		b3AreaIllumination(scene,surface) :
		b3PointIllumination(scene,surface));
}

b3_bool b3Light::b3PointIllumination(
	b3Scene         *scene,
	b3_illumination *surface)
{
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
			q = 1.0 - epsilon;
		}
		else
		{
			q = (LightFrac >= 1 ? epsilon : acos(LightFrac) * 2.0 / M_PI);
		}

		b3DeBoorOpened (&m_Spline,&point,0,q);
		Jit.LightFrac = LightDist * m_Distance * point.y;
	}
	else
	{
		Jit.LightFrac = LightDist * m_Distance;
	}
	Jit.LightDist = LightDist;

	scene->b3Intersect(&Jit,UpperBound - epsilon);
	scene->b3Illuminate(this,&Jit,surface,&surface->incoming->color);
	return true;
}

b3_bool b3Light::b3AreaIllumination (
	b3Scene         *scene,
	b3_illumination *surface)
{
	b3Shape       *Edge1,*Edge2,*LastEdge = null;
	b3_light_info  Jit;
	b3_vector      point;
	b3_f64         Factor,LightDist,q;
	b3_coord       x,y,xs;
	b3_count       Distr,max;
	b3_bool        equal;				

	Jit.Distr = m_JitterEdge;
	Jit.Size  = m_Distance * m_Size / (b3_f64)Jit.Distr;

	// computing light axis between ipoint and light
	// Changed by SAM 9-04-1993
	Jit.pos         = surface->incoming->ipoint;
	Jit.LightView.x = m_Position.x - surface->incoming->ipoint.x;
	Jit.LightView.y = m_Position.y - surface->incoming->ipoint.y;
	Jit.LightView.z = m_Position.z - surface->incoming->ipoint.z;
	LightDist =
		Jit.LightView.x * Jit.LightView.x +
		Jit.LightView.y * Jit.LightView.y +
		Jit.LightView.z * Jit.LightView.z;
	if (LightDist == 0)
	{
		return false;
	}

	// normalizing light axis
	LightDist = 1.0 / (Jit.Q = sqrt(LightDist));
	Jit.LightView.x	*= LightDist;
	Jit.LightView.y	*= LightDist;
	Jit.LightView.z	*= LightDist;


	// inserted Nov. 1994, SAM
	if (m_SpotActive)
	{
		Factor = -(
			Jit.LightView.x * m_Direction.x +
			Jit.LightView.y * m_Direction.y +
			Jit.LightView.z * m_Direction.z);
		if (Factor <= 0) q = 1.0 - epsilon;
		else
		{
			q = (Factor >= 1 ? epsilon : acos(Factor) * 2.0 / M_PI);
		}

		b3DeBoorOpened (&m_Spline,&point,0,q);
		Jit.LightFrac  = LightDist * m_Distance * point.y;
	}
	else
	{
		Jit.LightFrac = LightDist * m_Distance;
	}
	Jit.LightDist = LightDist;

	Factor = LightDist / sqrt(
		Jit.LightView.x * Jit.LightView.x +
		Jit.LightView.y * Jit.LightView.y);
	Jit.xDir.x	= -Jit.LightView.y * Factor;
	Jit.xDir.y	=  Jit.LightView.x * Factor;
	Jit.xDir.z	=  0;

	Jit.yDir.x	=  Jit.LightView.y * Jit.xDir.z - Jit.LightView.z * Jit.xDir.y;
	Jit.yDir.y	=  Jit.LightView.z * Jit.xDir.x - Jit.LightView.x * Jit.xDir.z;
	Jit.yDir.z	=  Jit.LightView.x * Jit.xDir.y - Jit.LightView.y * Jit.xDir.x;
	Factor = LightDist / sqrt(
		Jit.yDir.x * Jit.yDir.x +
		Jit.yDir.y * Jit.yDir.y +
		Jit.yDir.z * Jit.yDir.z);
	Jit.yDir.x *= Factor;
	Jit.yDir.y *= Factor;
	Jit.yDir.z *= Factor;

	Jit.Result.a =
	Jit.Result.r =
	Jit.Result.g =
	Jit.Result.b = 0;

	// The following loops check every second point of the outline
	// of the light raster. If every sample hits the same shape the
	// shadow computation is completed.
	equal = true;
	Distr = Jit.Distr - 1;
	xs    = B3_IRAN(2);
	for (x = xs;x <= Distr;x += 2)
	{
		Edge1 = b3CheckSinglePoint (scene,surface,&Jit,x,0);
		Edge2 =	b3CheckSinglePoint (scene,surface,&Jit,Distr,Distr-x);

		equal   &= (Edge1 == Edge2);
		if (x != xs) equal &= (Edge1 == LastEdge);
		LastEdge = Edge1;
	}
	for (y = 2 - xs;y < Distr;y += 2)
	{
		Edge1 = b3CheckSinglePoint (scene,surface,&Jit,0,y);
		Edge2 =	b3CheckSinglePoint (scene,surface,&Jit,Distr,Distr-y);

		equal   &= ((Edge1 == Edge2) && (Edge1 == LastEdge));
		LastEdge = Edge1;
	}


	// This part computes the untested points in the light raster.
	if (equal)
	{
		Factor = 0.5 / Distr;
	}
	else
	{
		// fill top and bottom outline
		for (x = 1 - xs;x <= Distr;x += 2)
		{
			b3CheckSinglePoint (scene,surface,&Jit,x,0);
			b3CheckSinglePoint (scene,surface,&Jit,x,Distr);
		}

		for (y = 1;y < Distr;y++)
		{
			max = Distr + ((Jit.Distr + xs) & 1);
			for (x = xs;x < max;x++)
			{
				b3CheckSinglePoint (scene,surface,&Jit,x,y);
			}
			xs ^= 1;
		}

		Factor = 1.0 / (Jit.Distr * Jit.Distr);
	}

	surface->incoming->color.r += Jit.Result.r * Factor;
	surface->incoming->color.g += Jit.Result.g * Factor;
	surface->incoming->color.b += Jit.Result.b * Factor;
	return true;
}

b3Shape *b3Light::b3CheckSinglePoint (
	b3Scene         *scene,                                
	b3_illumination *surface,
	b3_light_info   *Jit,
	b3_coord         x,
	b3_coord         y)
{
	b3_f64   jx,jy,LightDist;

	jx = (((b3_f64)x + 0.25 + B3_FRAN(0.5)) - 0.5 * Jit->Distr) * Jit->Size;
	jy = (((b3_f64)y + 0.25 + B3_FRAN(0.5)) - 0.5 * Jit->Distr) * Jit->Size;

	Jit->dir.x = Jit->LightView.x + jx * Jit->xDir.x + jy * Jit->yDir.x;
	Jit->dir.y = Jit->LightView.y + jx * Jit->xDir.y + jy * Jit->yDir.y;
	Jit->dir.z = Jit->LightView.z + jx * Jit->xDir.z + jy * Jit->yDir.z;

	LightDist =
		Jit->dir.x * Jit->dir.x +
		Jit->dir.y * Jit->dir.y +
		Jit->dir.z * Jit->dir.z;
	if (LightDist != 0)
	{
		LightDist   = 1.0 / sqrt(LightDist);
		Jit->dir.x *= LightDist;
		Jit->dir.y *= LightDist;
		Jit->dir.z *= LightDist;
	}
	Jit->LightDist = LightDist;

	scene->b3Intersect(Jit,Jit->Q - epsilon);
	scene->b3Illuminate(this,Jit,surface,&Jit->Result);

	return Jit->shape;
}
