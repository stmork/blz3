/*
**
**	$Filename:	b3MaterialSampler.h $ 
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Sampler for testing materials
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifndef B3_RAYTRACE_MATERIALSAMPLER_H
#define B3_RAYTRACE_MATERIALSAMPLER_H

#include "blz3/image/b3Sampler.h"
#include "blz3/raytrace/b3Raytrace.h"

#define DEFAULT_MATERIAL_TILES 3

class b3MaterialSampler : public b3Sampler
{
protected:
	b3Tx       *m_Tx;
	b3Material *m_Material;
	b3_count    m_Tiles;

public:
	              b3MaterialSampler(b3Tx *tx,b3_count tiles = DEFAULT_MATERIAL_TILES);
	void          b3SetMaterial(b3Material *material);

protected:
	b3SampleInfo *b3SampleInit(b3_count CPUs);
	void          b3SampleTask(b3SampleInfo *info);
};

#endif
