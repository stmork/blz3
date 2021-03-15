/*
**
**	$Filename:	b3Light.h $
*	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
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

#pragma once

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

/**
 * This structure provides information for lighting from a light source.
 */
struct b3_light_info : public b3_ray
{
	b3Color   m_AmbientSum;     //!< The collected ambient term over all shader feeler.
	b3Color   m_DiffuseSum;     //!< The collected diffuse term over all shadow feeler.
	b3Color   m_SpecularSum;    //!< The collected specular term over all shadow feeler.
	b3_vector m_LightView;      //!< The normalized direction from the intersection point to the light source.
	b3_vector m_xDir;           //!< The horizontal spanning vector for soft lights.
	b3_vector m_yDir;           //!< The vertical spanning vector for soft shadows.
	b3_f64    m_Size;           //!< Spanning area for soft light sources copied from the b3Light class.
	b3_f64    m_LightFrac;      //!< The reciproce distance between the intersection point and the light source.
	b3_f64    m_LightDist;      //!< The real distance between the intersection point and the light source.
	b3_s32    m_Distr;
};

class b3Material;
class b3Shader;
class b3Scene;

struct b3_surface;

/**
 * This class represents one light source.
 */
class B3_PLUGIN b3Light : public b3Item
{
	b3_f64            m_HalfJitter;
	b3_f64            m_FullJitter;
	b3_vector         m_SpotDir;

public:
	b3_vector        m_Position;     //!< Light position.
	b3Color          m_Color;        //!< Light color.
	b3_f32           m_Distance;     //!< Relative light distance.
	b3_s32           m_Flags;        //!< Type of light source.
	b3_f32           m_Size;         //!< Spanning area for soft light sources.
	b3_s32           m_JitterEdge;   //!< The sampling size for soft light sources.
	b3_vector        m_Direction;    //!< Light direction for spot lights.
	b3Spline         m_Spline;       //!< The spline describing the light distribution curve.
	b3_f32           m_Knots[B3_MAX_KNOTS];       //!< The knot vector of the light distribution curve.
	b3_vector        m_Controls[B3_MAX_CONTROLS]; //!< The control points of the light distribution curve.
	b3_bool          m_LightActive;               //!< Activation flag of the light source.
	b3_bool          m_SoftShadow;                //!< A flag signalling soft lights.
	b3_bool          m_SpotActive;                //!< A flag signalling directional spot lights.
	char             m_Name[B3_LIGHTNAMELEN];     //!< The name of the light source.

public:
	B3_ITEM_INIT(b3Light); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3Light); //!< This constructor handles deserialization.

	/**
	 * Method for registering the light source classes into the item registry.
	 */
	static void     b3Register();
	void     b3Write() override;
	void     b3Dump(b3_count level);
	b3_bool  b3Prepare(b3_preparation_info * prep_info) override;

	/**
	 * This method returns the lights name.
	 *
	 * @return The light name.
	 */
	const char * b3GetName();

	/**
	 * This method renames the light source.
	 *
	 * @param name The new light name.
	 */
	void     b3SetName(const char * name);

	/**
	 * This method returns the activation flag of this light source.
	 *
	 * @return True if the light source is active.
	 */
	inline b3_bool  b3IsActive()
	{
		return m_LightActive;
	}

	/**
	 * This method computes the spot light exponent into an exponent
	 * usable for OpenGL light sources.
	 *
	 * @return The spot lights directional light exponent for OpenGL.
	 */
	b3_f64   b3ComputeSpotExponent();

	/**
	 * This method illuminats a surface with the given shader.
	 *
	 * @param shader The shader to use.
	 * @param surface The surface values for shader computation.
	 * @return True on success.
	 * @see b3Shader
	 * @see b3_surface.
	 */
	b3_bool  b3Illuminate(b3Shader * shader, b3_surface * surface);

private:
	void            b3InitValues();
	b3_bool         b3PointIllumination(b3Shader * shader, b3_surface * surface);
	b3_bool         b3AreaIllumination(b3Shader * shader, b3_surface * surface);
	const b3Shape * b3CheckSinglePoint(b3Shader * shader, b3_surface * surface,
		b3_light_info * Jit, b3_coord x, b3_coord y);

	inline b3_f64 b3GetSpotFactor(b3_f64 angle) const // angle inside [0..1]
	{
		if (m_SpotActive)
		{
			b3_vector point;

			m_Spline.b3DeBoorOpened(&point, 0, angle);
			return point.y;
		}
		else
		{
			return 1;
		}
	}
};

#endif
