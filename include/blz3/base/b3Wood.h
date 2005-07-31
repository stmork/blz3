/*
**
**	$Filename:	b3Wood.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Woodlike procedural textures
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_BASE_WOOD_H
#define B3_BASE_WOOD_H

#include "blz3/b3Config.h"

/**
 * This class generates shaded wood.
 */
class B3_PLUGIN b3Wood
{
protected:
	b3_matrix         m_Warp;

public:
	b3_f32            m_yRot;
	b3_f32            m_zRot;
	b3_f32            m_RingSpacing;
	b3_f32            m_RingFrequency;
	b3_f32            m_RingNoise;
	b3_f32            m_RingNoiseFrequency;
	b3_f32            m_TrunkWobble;
	b3_f32            m_TrunkWobbleFrequency;
	b3_f32            m_AngularWobble;
	b3_f32            m_AngularWobbleFrequency;
	b3_f32            m_GrainFrequency;
	b3_f32            m_Grainy;
	b3_f32            m_Ringy;

public:
	/**
	 * This method computes a wooden pattern.
	 *
	 * \param *polar The point to compute the wooden pattern from.
	 * \param distance Distance from eye point to give a level of detail.
	 * \return The wooden pattern.
	 */
	b3_f64 b3ComputeWood(b3_vector *polar, b3_f64 distance);

	/**
	 * This method initializes the class with default propertiy values.
	 */
	void   b3InitWood();

	/**
	 * This method initializes the class. The method must be reinvocated
	 * if the material properties are changed.
	 *
	 * \param *scale 
	 */
	void   b3PrepareWood(b3_vector *scale);

	/**
	 * This method copies the property values from this class into a given
	 * instance by slightly disturbing the values. The value wobble gives
	 * a disturbance factor and the values fx,fy determines the position inside
	 * an oak plank.
	 *
	 * \param *wood 
	 * \param wobble 
	 * \param fx 
	 * \param fy 
	 * \see b3OakPlank
	 */
	void   b3CopyWobbled(b3Wood *wood,b3_f64 wobble,b3_f64 fx,b3_f64 fy);
};

/**
 * This class generates oak planked wood.
 */
class B3_PLUGIN b3OakPlank : public b3Wood
{
	b3_f64            m_rxScale;
	b3_f64            m_ryScale;
	b3_f64            m_rxTimes;
	b3_f64            m_ryTimes;

protected:
	b3Wood           *m_Planks;
	b3_count          m_PlankCount;

public:
	b3_s32            m_xTimes,m_yTimes; // Used!
	b3_f32            m_Wobble;
	b3_f32            m_xScale;
	b3_f32            m_yScale;
	b3_f32            m_xOffset;

public:
	         b3OakPlank();
	virtual ~b3OakPlank();

	/**
	 * This method computes a wooden pattern.
	 *
	 * \param *polar The point to compute the wooden oak plank pattern from.
	 * \param distance Distance from eye point to give a level of detail.
	 * \return The wooden oak plank pattern.
	 */
	b3_f64   b3ComputeOakPlank(b3_vector *polar, b3_f64 distance, b3_index &index);

	/**
	 * This method initializes the class with default propertiy values.
	 */
	void     b3InitOakPlank();

	/**
	 * This method initializes the class. The method must be reinvocated
	 * if the material properties are changed.
	 *
	 * \param *scale 
	 */
	void     b3PrepareOakPlank(b3_vector *scale);
};

#endif
