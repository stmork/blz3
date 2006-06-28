/*
**
**	$Filename:	b3TxIFF.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Loading IFF images
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

#ifndef B3_IMAGE_TXIFF_H
#define B3_IMAGE_TXIFF_H

#include "blz3/b3Config.h"

/*************************************************************************
**                                                                      **
**     EA Interchange File Format (IFF) structures and definitions      **
**                                                                      **
*************************************************************************/

#define IFF_MROF		0x4d524f46		/* IFF-ILBM chunks */
#define IFF_FORM		0x464f524d		/* IFF-RGBx chunks */
#define IFF_RGB8		0x52474238
#define IFF_RGBN		0x5247424e
#define IFF_ILBM		0x494c424d
#define IFF_BMHD		0x424d4844
#define IFF_CMAP		0x434d4150
#define IFF_CAMG		0x43414d47
#define IFF_ANNO		0x414e4e4f
#define IFF_BODY		0x424f4459

#ifndef HIRES							/* Amiga resolutions IFF-ILBM-format */
#define HIRES			0x8000
#endif
#ifndef HAM
#define HAM				0x0800
#endif
#ifndef EXTRA_HALFBRITE
#define EXTRA_HALFBRITE	0x0080
#endif

#ifndef IFF_YUVN						/* possible early definitions */
#define IFF_YUVN		0x5955564e

#define IFF_YCHD		0x59434844		/* IFF-YUVN chunks */
#define IFF_DATY		0x44415459
#define IFF_DATU		0x44415455
#define IFF_DATV		0x44415456

struct b3_iff_ychd_chunk
{
	b3_u16 Width;
	b3_u16 Height;
	b3_u16 PageWidth;
	b3_u16 PageHeight;
	b3_u16 LeftEdge;
	b3_u16 TopEdge;
	b3_u08 AspectX;
	b3_u08 AspectY;
	b3_u08 Compress;
	b3_u08 Flags;
	b3_u08 Mode;
	b3_u08 Norm;
	b3_s16 reserved2;
	b3_s32 reserved3;
};

#define YCHD_MODE_400     0					/* YUVN modes */
#define YCHD_MODE_411     1
#define YCHD_MODE_422     2
#define YCHD_MODE_444     3
#define YCHD_MODE_200     8
#define YCHD_MODE_211     9
#define YCHD_MODE_222     10

#define YCHDF_LACE			1

#define YCHD_NORM_UNKNOWN	0
#define YCHD_NORM_PAL		1
#define YCHD_NORM_NTSC		2

#endif

#endif
