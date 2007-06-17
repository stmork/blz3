/*
**
**	$Filename:	b3Bump.h $ 
**	$Release:	Dortmund 2004 $
**	$Revision: 1457 $
**	$Date: 2006-08-22 15:30:01 +0200 (Di, 22 Aug 2006) $
**	$Author: smork $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing class definitions for bumps
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifndef B3_RAYTRACE_BUMP_H
#define B3_RAYTRACE_BUMP_H

#include "blz3/raytrace/b3Scaling.h"
#include "blz3/base/b3Array.h"
#include "blz3/base/b3OceanWave.h"
#include "blz3/base/b3Procedure.h"
#include "blz3/base/b3Water.h"
#include "blz3/base/b3Wood.h"
#include "blz3/image/b3Tx.h"

/*************************************************************************
**                                                                      **
**                        bump mapping                                  **
**                                                                      **
*************************************************************************/

#define CLASS_BUMP          0x28000000
#define TYPE_BUMP_NOISE     0x00000001
#define TYPE_BUMP_MARBLE    0x00000002
#define TYPE_BUMP_TEXTURE   0x00000003
#define TYPE_BUMP_WATER     0x00000004
#define TYPE_BUMP_WAVE      0x00000005
#define TYPE_BUMP_GLOSSY    0x00000006
#define TYPE_BUMP_GROOVE    0x00000007
#define TYPE_BUMP_WOOD      0x00000008
#define TYPE_BUMP_OAKPLANK  0x00000009
#define TYPE_BUMP_OCEAN     0x0000000a

#define BUMP_NOISE          (CLASS_BUMP|TYPE_BUMP_NOISE)
#define BUMP_MARBLE         (CLASS_BUMP|TYPE_BUMP_MARBLE)
#define BUMP_TEXTURE        (CLASS_BUMP|TYPE_BUMP_TEXTURE)
#define BUMP_WATER          (CLASS_BUMP|TYPE_BUMP_WATER)
#define BUMP_WAVE           (CLASS_BUMP|TYPE_BUMP_WAVE)
#define BUMP_GLOSSY         (CLASS_BUMP|TYPE_BUMP_GLOSSY)
#define BUMP_GROOVE         (CLASS_BUMP|TYPE_BUMP_GROOVE)
#define BUMP_WOOD           (CLASS_BUMP|TYPE_BUMP_WOOD)
#define BUMP_OAKPLANK       (CLASS_BUMP|TYPE_BUMP_OAKPLANK)
#define BUMP_OCEAN          (CLASS_BUMP|TYPE_BUMP_OCEAN)

/*************************************************************************
**                                                                      **
**                        Base class for all bump classes               **               
**                                                                      **
*************************************************************************/

/**
 * This class provides an interface for bump mapping.
 *
 * @dotfile bump.dot "Class hierarchy of bump mapping classes."
 */
class B3_PLUGIN b3Bump : public b3Item
{
public:
	b3_f32        m_Amplitude;  //!< This attribute provides a general bump amplitude.

protected:
	B3_ITEM_BASE(b3Bump); //!< This is a base class deserialization constructor.

public:
	B3_ITEM_INIT(b3Bump); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3Bump); //!< This constructor handles deserialization.

	/**
	 * Method for registering the bump mappings into the item registry.
	 */
	static         void    b3Register();

	virtual        b3_bool b3Prepare(b3_preparation_info *info)
	{
		return true;
	}

	/**
	 * This method computes the correct normal at the intersection point
	 * given with the b3_ray structure.
	 *
	 * @param ray The b3_ray structure containing the intersection point.
	 */
	virtual void    b3BumpNormal(b3_ray *ray)
	{
	}

	/**
	 * This method signals if the bump class needs the normal derivatives
	 *
	 * @return True if the derivatives should be computed manually.
	 */
	virtual b3_bool b3NeedDeriv()
	{
		return false;
	}
};

/*************************************************************************
**                                                                      **
**                        Noise bumping                                 **
**                                                                      **
*************************************************************************/

/**
 * This provides procedural bump mapping directly from Perlin noise.
 *
 * @see b3Noise
 */
class B3_PLUGIN b3BumpNoise : public b3Bump, public b3Scaling
{
public:
	B3_ITEM_INIT(b3BumpNoise); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3BumpNoise); //!< This constructor handles deserialization.

	void b3Write();
	void b3BumpNormal(b3_ray *ray);
};

/*************************************************************************
**                                                                      **
**                        Marble like bumping                           **
**                                                                      **
*************************************************************************/

/**
 * This class provides procedural bump mapping congruent th marble material.
 *
 * @see b3MatMarble
 */
class B3_PLUGIN b3BumpMarble : public b3Bump, public b3Scaling
{
public:
	B3_ITEM_INIT(b3BumpMarble); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3BumpMarble); //!< This constructor handles deserialization.

	void    b3Write();
	b3_bool b3Prepare(b3_preparation_info *info);
	void    b3BumpNormal(b3_ray *ray);
};

/*************************************************************************
**                                                                      **
**                        Classic bump mapping                          **
**                                                                      **
*************************************************************************/

/**
 * This class provides bump mapping from a texture. The blue channel
 * gives a height map relative to the normal.
 */
class B3_PLUGIN b3BumpTexture : public b3Bump
{
public:
	b3_f32         m_xStart,m_yStart;    //!< Surface coordinate start.
	b3_f32         m_xScale,m_yScale;    //!< Texture scale.
	b3_s32         m_xTimes,m_yTimes;    //!< Repeatition in x- y-direction.
	b3_s32         m_Flags;              //!< Unused.
	b3Path         m_Name;               //!< The texture file name.
	b3Tx          *m_Texture;            //!< The selected texture.

public:
	B3_ITEM_INIT(b3BumpTexture); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3BumpTexture); //!< This constructor handles deserialization.

	       void    b3Write();
	       b3_bool b3Prepare(b3_preparation_info *info);
	       void    b3BumpNormal(b3_ray *ray);
	inline b3_bool b3NeedDeriv()
	{
		return true;
	}

	/**
	 * This method sets a new texture name and loads the appropriate reference.
	 *
	 * @param name The new texture name.
	 */
	void    b3SetTexture(const char *name);

private:
	b3_bool b3GetNormalDeriv(b3_f64 lx,b3_f64 ly,b3_vector *deriv);
};

/*************************************************************************
**                                                                      **
**                        Water surface bumping                         **
**                                                                      **
*************************************************************************/

/**
 * This method provides procedural bump mapping which simulates a water surface.
 *
 * @see b3Scaling
 * @see b3Water
 */
class B3_PLUGIN b3BumpWater : public b3Bump, public b3Water, public b3Scaling
{
public:
	B3_ITEM_INIT(b3BumpWater); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3BumpWater); //!< This constructor handles deserialization.

	void    b3Write();
	b3_bool b3Prepare(b3_preparation_info *info);
	void    b3BumpNormal(b3_ray *ray);
};

/*************************************************************************
**                                                                      **
**                        Beach ripples bumping                         **
**                                                                      **
*************************************************************************/

/**
 * This class provides procedural beach ripples.
 */
class B3_PLUGIN b3BumpWave : public b3Bump, public b3Scaling
{
public:
	B3_ITEM_INIT(b3BumpWave); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3BumpWave); //!< This constructor handles deserialization.

	void    b3Write();
	b3_bool b3Prepare(b3_preparation_info *info);
	void    b3BumpNormal(b3_ray *ray);
};

/*************************************************************************
**                                                                      **
**                        A groovy kind of love                         **
**                                                                      **
*************************************************************************/

/**
 * This class provides procedural beach ripples.
 */
class B3_PLUGIN b3BumpGroove : public b3Bump, public b3Scaling
{
public:
	B3_ITEM_INIT(b3BumpGroove); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3BumpGroove); //!< This constructor handles deserialization.

	void    b3Write();
	b3_bool b3Prepare(b3_preparation_info *info);
	void    b3BumpNormal(b3_ray *ray);
};

/*************************************************************************
**                                                                      **
**                        glossy surface bumping (needs                 **
**                        distributed raytracing!)                      **
**                                                                      **
*************************************************************************/

/**
 * This class provides a procedural glossy surface. The best results are
 * achieved when using distributed raytracing through the b3Distributed
 * special effect.
 */
class B3_PLUGIN b3BumpGlossy : public b3Bump
{
public:
	b3_s32      m_Flags; //!< Unused.

public:
	B3_ITEM_INIT(b3BumpGlossy); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3BumpGlossy); //!< This constructor handles deserialization.

	void b3Write();
	void b3BumpNormal(b3_ray *ray);
};

/*************************************************************************
**                                                                      **
**                        Wodden bump base class                        **
**                                                                      **
*************************************************************************/

/**
 * This class provides a base class for all wooden surfaces.
 *
 * @see b3Scaling
 */
class B3_PLUGIN b3BumpWooden : public b3Bump, public b3Scaling
{
protected:
	b3_f64      m_dX; //!< This is the relative polar x derivative scaling factor.
	b3_f64      m_dY; //!< This is the relative polar y derivative scaling factor.

protected:
	B3_ITEM_BASE(b3BumpWooden); //!< This is a base class deserialization constructor.

public:
	B3_ITEM_INIT(b3BumpWooden); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3BumpWooden); //!< This constructor handles deserialization.
};

/*************************************************************************
**                                                                      **
**                        Wood bump                                     **
**                                                                      **
*************************************************************************/
 
/**
 * This class provides procedural bump mapping congruent to the wood material.
 *
 * @see b3Wood
 * @see b3MatWood
 * @see b3Scaling
 */
class B3_PLUGIN b3BumpWood : public b3BumpWooden, public b3Wood
{
public:
	B3_ITEM_INIT(b3BumpWood); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3BumpWood); //!< This constructor handles deserialization.

	inline b3_bool b3NeedDeriv()
	{
		return true;
	}

	b3_bool b3Prepare(b3_preparation_info *info);
	void    b3Write();
	void    b3BumpNormal(b3_ray *ray);
};

/*************************************************************************
**                                                                      **
**                        Oak plank bump                                **
**                                                                      **
*************************************************************************/

/**
 * This class provides procedural bump mapping congruent to the wood planks material.
 *
 * @see b3Wood
 * @see b3MatOakPlank
 * @see b3Scaling
 */
class B3_PLUGIN b3BumpOakPlank : public b3BumpWooden, public b3OakPlank
{
	b3Array<b3_f64> m_Amplitudes; //!< The amplitudes for the different planks. 

public:
	B3_ITEM_INIT(b3BumpOakPlank); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3BumpOakPlank); //!< This constructor handles deserialization.

	inline b3_bool b3NeedDeriv()
	{
		return true;
	}

	b3_bool b3Prepare(b3_preparation_info *info);
	void    b3Write();
	void    b3BumpNormal(b3_ray *ray);
};

//flags for WaterBump, WaveBump
#define BUMP_IPOINT          1
#define BUMP_U_SUPPRESS_WAVE 2
#define BUMP_V_SUPPRESS_WAVE 4

/*************************************************************************
**                                                                      **
**                        Ocean wave bump                               **
**                                                                      **
*************************************************************************/

class B3_PLUGIN b3BumpOcean : public b3Bump, public b3OceanWave, public b3Scaling
{
public:
	B3_ITEM_INIT(b3BumpOcean);
	B3_ITEM_LOAD(b3BumpOcean);
	void     b3Write();
	b3_bool  b3Prepare(b3_preparation_info *info);
	char    *b3GetName();

public:
	virtual void    b3BumpNormal(b3_ray *ray);
	virtual b3_bool b3NeedDeriv();
};


#endif
