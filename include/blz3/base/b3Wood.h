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
	b3_f64 b3ComputeWood(b3_vector *polar, b3_f64 distance);
	void   b3InitWood();
	void   b3PrepareWood(b3_vector *scale);
	void   b3CopyWobbled(b3Wood *wood,b3_f64 wobble,b3_f64 fx,b3_f64 fy);
};

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
	b3_f64   b3ComputeOakPlank(b3_vector *polar, b3_f64 distance, b3_index &index);
	void     b3InitOakPlank();
	void     b3PrepareOakPlank(b3_vector *scale);
};

#endif
