/*
**
**	$Filename:	b3BumpSampler.h $ 
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
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
#include "blz3/raytrace/b3Raytrace.h"

#define DEFAULT_BUMP_TILES 3

class b3BumpSampler : public b3Sampler
{
	static b3_vector m_Light;

protected:
	b3Tx     *m_Tx;
	b3Bump   *m_Bump;
	b3_count  m_Tiles;

public:
	              b3BumpSampler(b3Tx *tx,b3_count tiles = DEFAULT_BUMP_TILES);
	void          b3SetBump(b3Bump *bump);

protected:
	b3SampleInfo *b3SampleInit(b3_count CPUs);
	void          b3SampleTask(b3SampleInfo *info);
};

#endif
