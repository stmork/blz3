/*
**
**	$Filename:	gif.h            $  
**	$Release:	Dortmund 1997 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	some GIF definitions
**
**	(C) Copyright 1997  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

#ifndef GIF_H
#define GIF_H

struct Extension
{
	char          magic;
	unsigned char label;
	unsigned char size;
	unsigned char flags;
	unsigned char delay[2];
	unsigned char index;
	char          term;
};

struct Descriptor
{
	char          seperator;
	unsigned char xPos[2];
	unsigned char yPos[2];
	unsigned char xSize[2];
	unsigned char ySize[2];
	unsigned char flags;
};

struct Repeator
{
	char          magic;
	unsigned char label;
	unsigned char size;
	char          text[11];
	unsigned char content[5];
};

#define VAL2(a) ((long)(((long)(a)[1] << 8) | (a)[0]))

#endif
