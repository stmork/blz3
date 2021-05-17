/*
**
**      $Filename:      b3TxSGI.h $
**      $Release:       Dortmund 2005 $
**      $Revision$
**      $Date$
**      $Author$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - SGI classes
**
**      (C) Copyright 2005 Steffen A. Mork
**          All Rights Reserved
**
**
*/

#pragma once

#ifndef B3_IMAGE_TXSGI_H
#define B3_IMAGE_TXSGI_H

#include "blz3/b3Config.h"

/*************************************************************************
**                                                                      **
**                        SGI RLE structures                            **
**                                                                      **
*************************************************************************/

/**
 * This structure defines the SGI image file format header.
 */
struct b3HeaderSGI
{
	b3_u16	imagic;		/* stuff saved on disk . . */
	b3_u16 	type;
	b3_u16 	dim;
	b3_u16 	xsize;
	b3_u16 	ysize;
	b3_u16 	zsize;
	b3_u32 	min;
	b3_u32 	max;
	b3_u32	wastebytes;
	char 	name[80];
	b3_u32	colormap;
};

#define IMAGIC1 	0x01da
#define IMAGIC2 	0xda01

#endif
