/*
**
**	$Filename:	b3MaterialSampler.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
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

#pragma once

#ifndef B3_RAYTRACE_MATERIALSAMPLER_H
#define B3_RAYTRACE_MATERIALSAMPLER_H

#include "blz3/image/b3Sampler.h"
#include "blz3/raytrace/b3Material.h"

/**
 * This class implements the sampling of a material into a texture.
 */
class b3MaterialSampler : public b3Sampler, protected b3_scene_preparation
{
	static const b3_count        DEFAULT_MATERIAL_TILES = 3;

protected:
	b3Tx    *         m_Tx;        //!< The image to sample in.
	b3Material *      m_Material; //!< The material to sample.
	const b3_count    m_Tiles;     //!< How many sample plains to use.
	const b3_vector * m_BBoxSize;  //!< The bounding box dimension.

public:
	/**
	 * This constructor initializes the material sampler.
	 *
	 * @param tx The image to sample into.
	 * @param bbox_size The original bounding box size
	 * @param tiles The number of intersection plains to sample.
	 */
	b3MaterialSampler(b3Tx * tx, const b3_vector * bbox_size, const b3_count tiles = DEFAULT_MATERIAL_TILES);

	/**
	 * This method sets a material to sample.
	 *
	 * @param material The material to sample.
	 */
	void          b3SetMaterial(b3Material * material);

	/**
	 * This method sets the actual time point.
	 *
	 * @param t The new time point
	 */
	inline void   b3SetTime(const b3_f64 t)
	{
		m_t = t;
	}

protected:
	b3SampleInfo * b3SampleInit(const b3_count CPUs) override;
	void           b3SampleTask(const b3SampleInfo * info) override;
};

#endif
