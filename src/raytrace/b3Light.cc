/*
**
**	$Filename:	b3Light.cc $
**	$Release:	Dortmund 2001, 2002, 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing light sources
**
**	(C) Copyright 2001, 2002, 2003  Steffen A. Mork
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

#include "b3RaytraceInclude.h"

/*************************************************************************
**                                                                      **
**                        b3Light implementation                        **
**                                                                      **
*************************************************************************/

void b3Light::b3Register()
{
	b3PrintF(B3LOG_DEBUG, "Registering light sources...\n");

	b3Item::b3Register(&b3Light::b3StaticInit, &b3Light::b3StaticInit, POINT_LIGHT);
	b3Item::b3Register(&b3Light::b3StaticInit, &b3Light::b3StaticInit, AREA_LIGHT);
	b3Item::b3Register(&b3Light::b3StaticInit, &b3Light::b3StaticInit, SPOT_LIGHT);
}

b3Light::b3Light(b3_u32 class_type) : b3Item(sizeof(b3Light), class_type)
{
	b3InitValues();
}

b3Light::b3Light(b3_u32 * src) : b3Item(src)
{
	b3_index i;

	b3InitValues();
	b3InitVector(&m_Position);
	b3InitColor(m_Color);
	m_Distance = b3InitFloat();
	m_Flags    = b3InitInt();

	if ((b3GetClassType() >= AREA_LIGHT) && B3_PARSE_INDEX_VALID)
	{
		m_Size       = b3InitFloat();
		m_JitterEdge = b3InitInt();
		if (b3GetClassType() >= SPOT_LIGHT && B3_PARSE_INDEX_VALID)
		{
			b3InitVector(&m_Direction);

			b3InitSpline(&m_Spline, m_Controls, m_Knots);
			for (i = 0; i < B3_MAX_KNOTS; i++)
			{
				m_Knots[i] = b3InitFloat();
			}
			for (i = 0; i < B3_MAX_CONTROLS; i++)
			{
				b3InitVector(&m_Controls[i]);
			}
			m_SpotActive = (m_Flags & LIGHT_SPOT_OFF ? false : true);
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
		m_SpotActive = false;
	}
	m_LightActive = ((m_Flags & LIGHT_OFF) == 0);

	if (m_ItemOffset > 0)
	{
		b3InitString(m_Name, m_ItemSize - (m_ParseIndex << 2));
	}
	else
	{
		m_Name[0] = 0;
	}

	if (m_LightActive)
	{
		b3PrintF(B3LOG_DEBUG, "  Light \"%s\" is active", m_Name);
		if (m_SoftShadow)
		{
			b3PrintF(B3LOG_DEBUG, " and uses soft shadows");
		}
		if (m_SpotActive)
		{
			b3PrintF(B3LOG_DEBUG, " and uses light distribution curve");
		}
		b3PrintF(B3LOG_DEBUG, ".\n");
	}
}

void b3Light::b3Write()
{
	b3_index i;

	m_Flags = 0;
	ClassType = SPOT_LIGHT;
	if (!m_LightActive)
	{
		m_Flags |= LIGHT_OFF;
	}
	if (m_SoftShadow)
	{
		m_Flags |= LIGHT_PENUMBRA;
	}
	if (!m_SpotActive)
	{
		m_Flags |= LIGHT_SPOT_OFF;
	}

	b3StoreVector(&m_Position);
	b3StoreColor(m_Color);
	b3StoreFloat(m_Distance);
	b3StoreInt(m_Flags);
	b3StoreFloat(m_Size);
	b3StoreInt(m_JitterEdge);
	b3StoreVector(&m_Direction);

	b3StoreSpline(&m_Spline);
	for (i = 0; i < B3_MAX_KNOTS; i++)
	{
		b3StoreFloat(m_Knots[i]);
	}
	for (i = 0; i < B3_MAX_CONTROLS; i++)
	{
		b3StoreVector(&m_Controls[i]);
	}

	b3StoreString(m_Name, B3_BOXSTRINGLEN);
}

#define INIT_DEGREE      3
#define INIT_CONTROL_MAX 7

void b3Light::b3InitValues()
{
	b3_index i;

	m_Color.b3Init(1.0, 1.0, 1.0);
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

	m_Spline.m_Offset     = 1;
	m_Spline.m_Knots      = m_Knots;
	m_Spline.m_Controls   = m_Controls;
	m_Spline.m_ControlMax = sizeof(m_Controls) / sizeof(m_Controls[0]);
	m_Spline.m_KnotMax    = sizeof(m_Knots)    / sizeof(m_Knots[0]);
	m_Spline.b3InitCurve(INIT_DEGREE, INIT_CONTROL_MAX, false);
	for (i = 0; i < INIT_CONTROL_MAX; i++)
	{
		m_Controls[i].x = (b3_f64)i / (INIT_CONTROL_MAX - 1);
		m_Controls[i].y = 1.0;
		m_Controls[i].z = 0;
	}
	m_Controls[INIT_CONTROL_MAX - 1].y = 0.5;

	for (i = INIT_DEGREE; i <= INIT_CONTROL_MAX; i++)
	{
		m_Knots[i] =
			(b3_f64)(i - INIT_DEGREE) /
			(b3_f64)(INIT_CONTROL_MAX - INIT_DEGREE);
	}
	m_Knots[INIT_CONTROL_MAX] += b3Scene::epsilon;
	m_Spline.b3ThroughEndControl();

	b3SetName("Light");
	m_LightActive = true;
	m_SoftShadow  = false;
	m_SpotActive  = false;
}

b3_bool b3Light::b3Prepare(b3_preparation_info * prep_info B3_UNUSED)
{
	if (m_LightActive)
	{
		if (m_SoftShadow)
		{
			if (m_JitterEdge > 1)
			{
				m_HalfJitter = 0.5 / (b3_f64)(m_JitterEdge - 1);
				m_FullJitter = 1.0 / (b3_f64)(m_JitterEdge * m_JitterEdge);
			}
			else
			{
				m_SoftShadow = false;
			}
		}
		if (m_SpotActive)
		{
			m_SpotDir = m_Direction;
			m_SpotActive = b3Vector::b3Normalize(&m_SpotDir) != 0;
		}
	}
	return true;
}

void b3Light::b3Dump(b3_count level)
{
	b3DumpSpace(level);
	b3PrintF(B3LOG_NORMAL, "Light %s, switched %s\n", b3GetName(), b3IsActive() ? "on" : "off");
}

const char * b3Light::b3GetName() const
{
	return m_Name;
}

void b3Light::b3SetName(const char * name)
{
	b3Item::b3SetString(m_Name, sizeof(m_Name), name);
}

b3_bool b3Light::b3Illuminate(
	b3Shader  *  shader,
	b3_surface * surface) const
{
	return (m_SoftShadow ?
			b3AreaIllumination(shader, surface) :
			b3PointIllumination(shader, surface));
}

inline b3_bool b3Light::b3PointIllumination(
	b3Shader  * shader,
	b3_surface * surface) const
{
	b3_light_info Jit;
	b3_vector     point;
	b3_f64        RecLightDist, SpotAngle, q, LightDist;

	Jit.m_Distr = 1;
	Jit.m_Size  = 0;
	Jit.pos.x   = surface->m_Incoming->ipoint.x;
	Jit.pos.y   = surface->m_Incoming->ipoint.y;
	Jit.pos.z   = surface->m_Incoming->ipoint.z;
	Jit.dir.x   = m_Position.x - surface->m_Incoming->ipoint.x;
	Jit.dir.y   = m_Position.y - surface->m_Incoming->ipoint.y;
	Jit.dir.z   = m_Position.z - surface->m_Incoming->ipoint.z;

	LightDist    = b3Vector::b3QuadLength(&Jit.dir);
	RecLightDist = 1.0 / (LightDist = sqrt(LightDist));
	b3Vector::b3Scale(&Jit.dir, RecLightDist);

	// Compute relative brightness via LDC
	// (= light distribution curve)
	if (m_SpotActive)
	{
		SpotAngle = -b3Vector::b3SMul(&Jit.dir, &m_SpotDir);
		if (SpotAngle <= 0)
		{
			q = 1.0 - b3Scene::epsilon;
		}
		else
		{
			q = (SpotAngle >= 1 ? b3Scene::epsilon : acos(SpotAngle) * 2.0 / M_PI);
		}

		m_Spline.b3DeBoorOpened(&point, 0, q);
		Jit.m_LightFrac = m_Distance * RecLightDist * point.y;
	}
	else
	{
		Jit.m_LightFrac = m_Distance * RecLightDist;
	}
	Jit.m_LightDist = RecLightDist;

	Jit.m_AmbientSum.b3Init();
	Jit.m_DiffuseSum.b3Init();
	Jit.m_SpecularSum.b3Init();

	shader->b3FindObscurer(&Jit, LightDist - b3Scene::epsilon);
	shader->b3Shade(this, &Jit, surface);

	surface->m_AmbientSum  += Jit.m_AmbientSum;
	surface->m_DiffuseSum  += Jit.m_DiffuseSum;
	surface->m_SpecularSum += Jit.m_SpecularSum;

	return true;
}

inline b3_bool b3Light::b3AreaIllumination(
	b3Shader  *  shader,
	b3_surface * surface) const
{
	b3_light_info  Jit;
	b3_bool        Edge1, Edge2, LastEdge = false, first = true;
	b3_vector      point;
	b3_f64         Factor, denomLightDist, q;
	b3_coord       x, y, xs;
	b3_count       max, Distr;
	b3_bool        equal;

	Jit.m_Distr = m_JitterEdge;
	Jit.m_Size  = m_Distance * m_Size / (b3_f64)Jit.m_Distr;

	Jit.pos           = surface->m_Incoming->ipoint;
	Jit.m_LightView.x = m_Position.x - surface->m_Incoming->ipoint.x;
	Jit.m_LightView.y = m_Position.y - surface->m_Incoming->ipoint.y;
	Jit.m_LightView.z = m_Position.z - surface->m_Incoming->ipoint.z;

	// normalizing light axis
	denomLightDist = b3Vector::b3QuadLength(&Jit.m_LightView);
	denomLightDist = 1.0 / (Jit.m_LightDist = sqrt(denomLightDist));
	b3Vector::b3Scale(&Jit.m_LightView, denomLightDist);

	if (m_SpotActive)
	{
		Factor = -b3Vector::b3SMul(&Jit.m_LightView, &m_SpotDir);
		if (Factor <= 0)
		{
			q = 1.0 - b3Scene::epsilon;
		}
		else
		{
			q = (Factor >= 1 ? b3Scene::epsilon : acos(Factor) * 2.0 / M_PI);
		}

		m_Spline.b3DeBoorOpened(&point, 0, q);
		Jit.m_LightFrac = denomLightDist * m_Distance * point.y;
	}
	else
	{
		Jit.m_LightFrac = denomLightDist * m_Distance;
	}

	Factor = denomLightDist / sqrt(
			Jit.m_LightView.x * Jit.m_LightView.x +
			Jit.m_LightView.y * Jit.m_LightView.y);
	Jit.m_xDir.x	= -Jit.m_LightView.y * Factor;
	Jit.m_xDir.y	=  Jit.m_LightView.x * Factor;
	Jit.m_xDir.z	=  0;

	b3Vector::b3CrossProduct(&Jit.m_LightView, &Jit.m_xDir, &Jit.m_yDir);
	Factor = denomLightDist / b3Vector::b3Length(&Jit.m_yDir);
	b3Vector::b3Scale(&Jit.m_yDir, Factor);

	Jit.m_AmbientSum.b3Init();
	Jit.m_DiffuseSum.b3Init();
	Jit.m_SpecularSum.b3Init();

	// The following loops check every second point of the outline
	// of the light raster. If every sample hits the same shape the
	// shadow computation is completed.
	equal = true;
	Distr = Jit.m_Distr - 1;
	xs    = 1;
	for (x = xs; x <= Distr; x += 2)
	{
		Edge1 = b3CheckSinglePoint(shader, surface, &Jit, x, 0) != nullptr;
		Edge2 =	b3CheckSinglePoint(shader, surface, &Jit, Distr, Distr - x) != nullptr;

		equal   &= (Edge1 == Edge2);
		if ((x != xs) && (!first))
		{
			equal &= (Edge1 == LastEdge);
		}
		LastEdge = Edge1;
		first = false;
	}

	for (y = 2 - xs; y < Distr; y += 2)
	{
		Edge1 = b3CheckSinglePoint(shader, surface, &Jit, 0, y) != nullptr;
		Edge2 =	b3CheckSinglePoint(shader, surface, &Jit, Distr, Distr - y) != nullptr;

		equal   &= ((Edge1 == Edge2) && (Edge1 == LastEdge));
		LastEdge = Edge1;
	}


	// This part computes the untested points in the light raster.
	if (equal)
	{
		Factor = m_HalfJitter;
	}
	else
	{
		// fill top and bottom outline
		for (x = 1 - xs; x <= Distr; x += 2)
		{
			b3CheckSinglePoint(shader, surface, &Jit, x, 0);
			b3CheckSinglePoint(shader, surface, &Jit, x, Distr);
		}

		for (y = 1; y < Distr; y++)
		{
			max = Distr + ((Jit.m_Distr + xs) & 1);
			for (x = xs; x < max; x++)
			{
				b3CheckSinglePoint(shader, surface, &Jit, x, y);
			}
			xs ^= 1;
		}

		Factor = m_FullJitter;
	}

	surface->m_AmbientSum  += Jit.m_AmbientSum  * Factor;
	surface->m_DiffuseSum  += Jit.m_DiffuseSum  * Factor;
	surface->m_SpecularSum += Jit.m_SpecularSum * Factor;

	return true;
}

inline const b3Shape * b3Light::b3CheckSinglePoint(
	b3Shader    *   shader,
	b3_surface   *  surface,
	b3_light_info * Jit,
	b3_coord        x,
	b3_coord        y) const
{
	b3_f64   jx, jy, LightDist;

	jx = ((b3_f32)x - 0.5 * Jit->m_Distr + B3_FRAN(1.0)) * Jit->m_Size;
	jy = ((b3_f32)y - 0.5 * Jit->m_Distr + B3_FRAN(1.0)) * Jit->m_Size;

	b3Vector::b3LinearCombine(&Jit->m_LightView, &Jit->m_xDir, &Jit->m_yDir, jx, jy, &Jit->dir);

	LightDist = b3Vector::b3Normalize(&Jit->dir);
	shader->b3FindObscurer(Jit, Jit->m_LightDist / LightDist - b3Scene::epsilon);
	shader->b3Shade(this, Jit, surface);

	return Jit->shape;
}

b3_f64 b3Light::b3ComputeSpotExponent() const
{
	b3_index i, max = 20;
	b3_f64   p     = 0, angle;
	b3_bool  loop  = true;

	if (m_SpotActive)
	{
		i = 0;

		do
		{
			angle = (double)i++ / (double)max;
			loop  = b3GetSpotFactor(angle) > 0.25;
		}
		while ((i < max) && loop);
		p = - 1.0 / log10(cos(angle * 0.5 * M_PI));
		b3PrintF(B3LOG_FULL, "b3Light::b3ComputeSpotExponent(%s) = %3.2f lambda=%3.2f\n",
			b3GetName(), p, angle);
	}
	return p;
}
