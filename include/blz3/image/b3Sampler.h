/*
**   
**  $Filename:  b3Sampler.cc $
**  $Release:   Dortmund 2004 $
**  $Revision$
**  $Date$
**  $Author$
**  $Developer: Steffen A. Mork $
**
**  Blizzard III - Sample something
**
**  (C) Copyright 2004  Steffen A. Mork
**      All Rights Reserved
**
**
*/

#ifndef B3_IMAGE_SAMPLER_H
#define B3_IMAGE_SAMPLER_H

#include "blz3/b3Config.h"

class b3Sampler;

struct b3SampleInfo
{
	b3Sampler    *m_Sampler;
	b3_res        m_yStart,m_yEnd;
	b3_res        m_xMax;
	b3_res        m_yMax;
	b3_pkd_color *m_Data;
	void         *m_Ptr;
};

class b3Sampler
{
protected:
	b3_res        m_xMax;
	b3_res        m_yMax;
	b3_pkd_color *m_Data;

public:
	void     b3Sample();

protected:
	virtual b3SampleInfo * b3SampleInit(b3_count CPUs) = 0;
	virtual void           b3SampleTask(b3SampleInfo *info) = 0;

private:
	static b3_u32 b3SampleThread(void *ptr);
};

class b3Tx;

class b3ImageSampler : public b3Sampler
{
public:
	                      b3ImageSampler(b3Tx *tx);
protected:
	        b3SampleInfo *b3SampleInit(b3_count CPUs);
	        void          b3SampleTask(b3SampleInfo *info);
	virtual b3_pkd_color  b3SamplePixel(b3_coord x,b3_coord y) = 0;
};

#endif
