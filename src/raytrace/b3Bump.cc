/*
**
**      $Filename:      b3Bump.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing bump mapping
**
**      (C) Copyright 2001  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/raytrace/b3Raytrace.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.2  2001/08/09 15:27:34  sm
**      - Following shapes are newly supported now:
**        o disk
**        o cylinder
**        o cone
**        o ellipsoid
**        o torus
**        o triangles
**      - Done some makefile fixes
**      - Everything is Windozable
**
**      Revision 1.1.1.1  2001/07/01 12:24:59  sm
**      Blizzard III is born
**
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

void b3InitBump::b3Init()
{
	b3PrintF (B3LOG_DEBUG,"Registering bump mapping...\n");
	b3Item::b3Register(&b3BumpNoise::b3Init,   &b3BumpNoise::b3Init,   BUMP_NOISE);
	b3Item::b3Register(&b3BumpNoise::b3Init,   &b3BumpNoise::b3Init,   BUMP_MARBLE);
	b3Item::b3Register(&b3BumpTexture::b3Init, &b3BumpTexture::b3Init, BUMP_TEXTURE);
	b3Item::b3Register(&b3BumpWater::b3Init,   &b3BumpWater::b3Init,   BUMP_WATER);
	b3Item::b3Register(&b3BumpWave::b3Init,    &b3BumpWave::b3Init,    BUMP_WAVE);
	b3Item::b3Register(&b3BumpGlossy::b3Init,  &b3BumpGlossy::b3Init,  BUMP_GLOSSY);
}

b3Bump::b3Bump(b3_size class_size,b3_u32 class_type) : b3Item(class_size,class_type)
{
}

b3Bump::b3Bump(b3_u32 class_type) : b3Item(sizeof(b3Bump),class_type)
{
}

b3Bump::b3Bump(b3_u32 *src) : b3Item(src)
{
}


b3BumpNoise::b3BumpNoise(b3_u32 class_type) : b3Bump(sizeof(b3BumpNoise),class_type)
{
}

b3BumpNoise::b3BumpNoise(b3_u32 *src) : b3Bump(src)
{
	b3InitVector(&Scale);
	Size = b3InitFloat();
}


b3BumpTexture::b3BumpTexture(b3_u32 class_type) : b3Bump(sizeof(b3BumpTexture),class_type)
{
}

b3BumpTexture::b3BumpTexture(b3_u32 *src) : b3Bump(src)
{
	xStart    = b3InitFloat();
	yStart    = b3InitFloat();
	xScale    = b3InitFloat();
	yScale    = b3InitFloat();
	xTimes    = b3InitInt();
	yTimes    = b3InitInt();
	Intensity = b3InitFloat();
	Texture   = (b3Tx *)b3InitNull();
	Flags     = b3InitInt();
	b3InitString(Name,B3_TEXSTRINGLEN);
}


b3BumpWater::b3BumpWater(b3_u32 class_type) : b3Bump(sizeof(b3BumpWater),class_type)
{
}

b3BumpWater::b3BumpWater(b3_u32 *src) : b3Bump(src)
{
	ScaleFlag   = b3InitInt();
	b3InitVector(&ScaleIPoint);
	ScaleRad    = b3InitFloat();
	if ((parseIndex << 2) < (b3_index)size)
	{
		ScaleTime   = b3InitFloat();
	}
}


b3BumpWave::b3BumpWave(b3_u32 class_type) : b3Bump(sizeof(b3BumpWave),class_type)
{
}

b3BumpWave::b3BumpWave(b3_u32 *src) : b3Bump(src)
{
	Flags     = b3InitInt();
	b3InitVector(&Scale);
	Amplitude = b3InitFloat();
}


b3BumpGlossy::b3BumpGlossy(b3_u32 class_type) : b3Bump(sizeof(b3BumpGlossy),class_type)
{
}

b3BumpGlossy::b3BumpGlossy(b3_u32 *src) : b3Bump(src)
{
	Flags     = b3InitInt();
	Intensity = b3InitFloat();
}
