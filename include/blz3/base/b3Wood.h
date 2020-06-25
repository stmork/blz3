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

#pragma once

#ifndef B3_BASE_WOOD_H
#define B3_BASE_WOOD_H

#include "blz3/b3Config.h"

/**
 * This class generates shaded wood.
 */
class B3_PLUGIN b3Wood
{
protected:
	b3_matrix         m_Warp; //!< The scaling matrix of object space.

public:
	b3_f32            m_yRot; //!< The trunk rotation around the y axis.
	b3_f32            m_zRot; //!< The trunk rotation around the z axis.

	b3_f32            m_RingSpacing;            //!< Controls the ring spacing.
	b3_f32            m_RingFrequency;          //!< Controls the frequency of ring spacing.
	b3_f32            m_RingNoise;              //!< Controls how noisy the rings are.
	b3_f32            m_RingNoiseFrequency;     //!< The ring noise frequency.
	b3_f32            m_TrunkWobble;            //!< A value how much the trunk wobbles.
	b3_f32            m_TrunkWobbleFrequency;   //!< The trunk wobble frequency.
	b3_f32            m_AngularWobble;          //!< The tree growth factor.
	b3_f32            m_AngularWobbleFrequency; //!< The growth frequency.
	b3_f32            m_GrainFrequency;         //!< The wood grain frequency.
	b3_f32            m_Grainy;                 //!< The strength of the grain.
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
	b3Wood           *m_Planks;     //!< The wood array for the planks.
	b3_count          m_PlankCount; //!< The number of different wood definitions.

public:
	b3_s32            m_xTimes;     //!< The planks in x direction.
	b3_s32            m_yTimes;     //!< The planks in y direction.
	b3_f32            m_Wobble;     //!< The wobbling factor between the wood planks.
	b3_f32            m_xScale;     //!< Scaling factor in x direction.
	b3_f32            m_yScale;     //!< Scaling factor in y direction.
	b3_f32            m_xOffset;    //!< The offset in x direction.

public:
	/**
	 * This constructor initializes this instance.
	 */
	b3OakPlank();

	/**
	 * This destructor deinitializes this instance.
	 */
	virtual ~b3OakPlank();

	/**
	 * This method computes a wooden pattern.
	 *
	 * \param *polar The point to compute the wooden oak plank pattern from.
	 * \param distance Distance from eye point to give a level of detail.
	 * \param index The used wood instance.
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
