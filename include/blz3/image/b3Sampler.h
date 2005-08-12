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

/**
 * This structure provides sampling information about a sampling task
 * The sampling task is organized into n strips where n is the used
 * number of CPUs.
 */
struct b3SampleInfo
{
	b3Sampler    *m_Sampler;       //!< The hosting sampler.
	b3_res        m_yStart;        //!< The vertical strip start coordinate.
	b3_res        m_yEnd;          //!< The vertical strip end coordinate.
	b3_res        m_xMax;          //!< The strip width.
	b3_res        m_yMax;          //!< The overall sampling height.
	b3_pkd_color *m_Data;          //!< The resulting strip data.
	void         *m_Ptr;           //!< A pointer to custom information.
};

/**
 * This class is a frame work for sampling something into an image.
 */
class b3Sampler
{
protected:
	b3_res        m_xMax;  //!< The sample width.
	b3_res        m_yMax;  //!< The sample height.
	b3_pkd_color *m_Data;  //!< The resulting sampling data.

public:
	/**
	 * This constructor makes basic initializations to this instance.
	 */
	void     b3Sample();

protected:
	/**
	 * This method initializes the sample information structures. There are
	 * n structures to initialize for n threads. n is the number of CPUs
	 * available.
	 *
	 * @param CPUs The number of CPUs available in the system.
	 * @return The initialized sample information structures.
	 * @see b3SampleInfo
	 */
	virtual b3SampleInfo * b3SampleInit(b3_count CPUs) = 0;

	/**
	 * This method is a call back for the sampling task called from each sampling thread.
	 *
	 * @param info The threads sample information structure.
	 */
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
