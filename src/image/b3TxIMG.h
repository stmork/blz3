/*
**
**	$Filename:	b3TxIMG.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Loading SGI RLE images
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

#ifndef B3_IMAGE_TXIMG_H
#define B3_IMAGE_TXIMG_H

/*************************************************************************
**                                                                      **
**                        SGI rle structure                             **
**                                                                      **
*************************************************************************/

struct HeaderSGI {
    unsigned short	imagic;		/* stuff saved on disk . . */
    unsigned short 	type;
    unsigned short 	dim;
    unsigned short 	xsize;
    unsigned short 	ysize;
    unsigned short 	zsize;
    unsigned long 	min;
    unsigned long 	max;
    unsigned long	wastebytes;	
    char 		name[80];
    unsigned long	colormap;
};

#define IMAGIC1 	0x01da
#define IMAGIC2 	0xda01

/*************************************************************************
**                                                                      **
**                        proto types                                   **
**                                                                      **
*************************************************************************/


#endif
