/*
**
**	$Filename:	b3Light.h $ 
*	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing class definitions for light sources
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifndef B3_RAYTRACE_LIGHT_H
#define B3_RAYTRACE_LIGHT_H

#include "blz3/raytrace/b3Base.h"

/*************************************************************************
**                                                                      **
**                        light sources                                 **
**                                                                      **
*************************************************************************/

#define CLASS_LIGHT             0x72000000
#define TYPE_POINTLIGHT         0x00000002
#define TYPE_AREALIGHT          0x00000001
#define TYPE_SPOTLIGHT          0x00000003
#define POINT_LIGHT             (CLASS_LIGHT|TYPE_POINTLIGHT)
#define AREA_LIGHT              (CLASS_LIGHT|TYPE_AREALIGHT)
#define SPOT_LIGHT              (CLASS_LIGHT|TYPE_SPOTLIGHT)

#define LIGHT_OFF               1L
#define LIGHT_PENUMBRA          2L
#define LIGHT_SPOT_OFF          4L

#define LIGHT_NAMEOFF(Node) ((char *)(Node)+BINDEX_OFFSET(Node))
#define LIGHT_NAMELEN(Node) (BINDEX_OFFSET(Node) > 0 ? \
	BINDEX_LENGTH(Node) - BINDEX_OFFSET(Node) : 0)

// POINT_LIGHT
class b3Material;
class b3Scene;
class B3_PLUGIN b3Light : public b3Item
{
	b3_f64            m_HalfJitter;
	b3_f64            m_FullJitter;
	b3_vector         m_SpotDir;

public:
	b3_vector        m_Position;     // Position
	b3Color          m_Color;        // Farbe
	b3_f32           m_Distance;     // rel. Reichweite
	b3_s32           m_Flags;        // Lampentyp
	b3_f32           m_Size;         // Auffaecherung
	b3_s32           m_JitterEdge;   // Kantenaufspaltung
	b3_vector        m_Direction;    // Abstrahlrichtung
	b3Spline         m_Spline;       // Spline Kurve
	b3_f32           m_Knots[B3_MAX_KNOTS];
	b3_vector        m_Controls[B3_MAX_CONTROLS];
	b3_bool          m_LightActive;
	b3_bool          m_SoftShadow;
	b3_bool          m_SpotActive;
	char             m_Name[B3_LIGHTNAMELEN]; // Lampenname

public:
	B3_ITEM_INIT(b3Light);
	B3_ITEM_LOAD(b3Light);

	static void     b3Register();
	       void     b3Write();
	       b3_bool  b3Illuminate(b3Scene *scene,b3_ray_fork *surface,b3Material *material);
	       b3_bool  b3Prepare();
	       b3_bool  b3IsActive();
	       char    *b3GetName();
		   void     b3SetName(const char *name);

	inline b3_f64 b3GetSpotFactor(b3_f64 angle) // angle inside [0..1]
	{
		if (m_SpotActive)
		{
			b3_vector point;

			m_Spline.b3DeBoorOpened(&point,0,angle);
			return point.y;
		}
		else
		{
			return 1;
		}
	}

private:
	void         b3InitValues();
	b3_bool      b3PointIllumination(b3Scene *scene,b3_ray_fork *surface,b3Material *material);
	b3_bool      b3AreaIllumination(b3Scene  *scene,b3_ray_fork *surface,b3Material *material);
	b3Shape     *b3CheckSinglePoint (b3Scene *scene,b3_ray_fork *surface,
		b3_light_info *Jit,b3_coord x,b3_coord y,b3Material *material);
};

#endif
