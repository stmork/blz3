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

/**
 * This class implements the sampling of a material into a texture.
 */
class b3MaterialSampler : public b3Sampler
{
protected:
	b3Tx       *m_Tx;       //!< The image to sample in.
	b3Material *m_Material; //!< The material to sample.
	b3_count    m_Tiles;    //!< How many sample plains to use.

public:
	/**
	 * This constructor initializes the material sampler.
	 *
	 * @param tx The image to sample into.
	 * @param tiles The number of intersection plains to sample.
	 */
	b3MaterialSampler(b3Tx *tx,b3_count tiles = DEFAULT_MATERIAL_TILES);

	/** 
	 * This method sets a material to sample.
	 *
	 * @param material The material to sample.
	 */
	void          b3SetMaterial(b3Material *material);

protected:
	b3SampleInfo *b3SampleInit(b3_count CPUs);
	void          b3SampleTask(b3SampleInfo *info);
};

#endif
