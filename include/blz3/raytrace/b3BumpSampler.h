/*
**
**	$Filename:	b3BumpSampler.h $ 
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Sampler for testing bump maps
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifndef B3_RAYTRACE_BUMPSAMPLER_H
#define B3_RAYTRACE_BUMPSAMPLER_H

#include "blz3/image/b3Sampler.h"
#include "blz3/raytrace/b3Bump.h"

/**
 * This class visualizes bump structures.
 */
class b3BumpSampler : public b3Sampler, protected b3_scene_preparation
{
	static const b3_count        DEFAULT_BUMP_TILES = 3;
	static b3_vector             m_Light;

protected:
	b3Tx      *m_Tx;        //!< The image to sample in.
	b3Bump    *m_Bump;      //!< The bump class instance to sample.
	const b3_count   m_Tiles;     //!< The horizontal tiles.
	const b3_vector *m_BBoxSize;  //!< The bounding box dimension.

public:
	/**
	 * This constructor initializes this instance.
	 *
	 * @param tx The image to sample in.
	 * @param bbox_size The original bounding box size
	 * @param tiles The number of tiles to use.
	 */
	b3BumpSampler(b3Tx *tx, const b3_vector *bbox_size, const b3_count tiles = DEFAULT_BUMP_TILES);

	/**
	 * This method sets the bump class instance.
	 *
	 * @param bump The bump class instance to sample.
	 */
	void          b3SetBump(b3Bump *bump);

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
	b3SampleInfo *b3SampleInit(const b3_count CPUs);
	void          b3SampleTask(const b3SampleInfo *info);
};

#endif
