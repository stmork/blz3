/*
**
**      $Filename:      b3Material.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing materials
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
**      Revision 1.5  2001/10/03 20:17:55  sm
**      - Minor bugfixes
**
**      Revision 1.4  2001/10/03 18:46:45  sm
**      - Adding illumination and recursive raytracing
**
**      Revision 1.3  2001/08/16 04:28:29  sm
**      - Solving conflicts
**
**      Revision 1.2  2001/08/15 19:52:57  sm
**      - First polygon rendering with Blizzard III (areas only)
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

void b3InitMaterial::b3Init()
{
	b3PrintF (B3LOG_DEBUG,"Registering materials...\n");
	b3Item::b3Register(&b3MatNormal::b3Init,      &b3MatNormal::b3Init,      MAT_NORMAL);
	b3Item::b3Register(&b3MatTexture::b3Init,     &b3MatTexture::b3Init,     TEXTURE);
	b3Item::b3Register(&b3MatChess::b3Init,       &b3MatChess::b3Init,       CHESS);
	b3Item::b3Register(&b3MatWrapTexture::b3Init, &b3MatWrapTexture::b3Init, WRAPTEXTURE);
	b3Item::b3Register(&b3MatMarble::b3Init,      &b3MatMarble::b3Init,      MARBLE);
	b3Item::b3Register(&b3MatSlide::b3Init,       &b3MatSlide::b3Init,       SLIDE);
	b3Item::b3Register(&b3MatWood::b3Init,        &b3MatWood::b3Init,        WOOD);
}

b3Material::b3Material(b3_size class_size,b3_u32 class_type) : b3Item(class_size,class_type)
{
}

b3Material::b3Material(b3_u32 class_type) : b3Item(sizeof(b3Material),class_type)
{
}

b3Material::b3Material(b3_u32 *src) : b3Item(src)
{
}

b3_f64 b3Material::b3GetReflection()
{
	return 0.0;
}

b3_f64 b3Material::b3GetRefraction()
{
	return 0.0;
}

b3_f64 b3Material::b3GetIndexOfRefraction()
{
	return 1.0;
}

b3_f64 b3Material::b3GetSpecularExponent()
{
	return 100000.0;
}

b3_bool b3Material::b3GetColors(
	b3_polar *polar,
	b3_color *diffuse,
	b3_color *ambient,
	b3_color *specular)
{
	return false;
}

b3MatNormal::b3MatNormal(b3_u32 class_type) : b3Material(sizeof(b3MatNormal),class_type)
{
}

b3MatNormal::b3MatNormal(b3_u32 *src) : b3Material(src)
{
	b3InitColor(&DiffColor);
	b3InitColor(&AmbColor);
	b3InitColor(&SpecColor);
	Reflection = b3InitFloat();
	Refraction = b3InitFloat();
	RefrValue  = b3InitFloat();
	HighLight  = b3InitFloat();
	Flags      = b3InitInt();
}

b3_bool b3MatNormal::b3GetColors(
	b3_polar *polar,
	b3_color *diffuse,
	b3_color *ambient,
	b3_color *specular)
{
	*diffuse  = DiffColor;
	*ambient  = AmbColor;
	*specular = SpecColor;
	return true;
}

b3_f64 b3MatNormal::b3GetReflection()
{
	return Reflection;
}

b3_f64 b3MatNormal::b3GetRefraction()
{
	return Refraction;
}

b3_f64 b3MatNormal::b3GetIndexOfRefraction()
{
	return RefrValue;
}

b3_f64 b3MatNormal::b3GetSpecularExponent()
{
	return HighLight;
}


b3MatTexture::b3MatTexture(b3_u32 class_type) : b3Material(sizeof(b3MatTexture),class_type) 
{
}

b3MatTexture::b3MatTexture(b3_u32 *src) : b3Material(src)
{
	Reflection = b3InitFloat();
	Refraction = b3InitFloat();
	RefrValue  = b3InitFloat();
	HighLight  = b3InitFloat();
	xStart     = b3InitFloat();
	yStart     = b3InitFloat();
	xScale     = b3InitFloat();
	yScale     = b3InitFloat();
	xTimes     = b3InitInt();
	yTimes     = b3InitInt();
	Texture    = (b3Tx *)b3InitNull();
	Flags      = b3InitInt();
	b3InitString(Name,B3_TEXSTRINGLEN);
}

b3_f64 b3MatTexture::b3GetReflection()
{
	return Reflection;
}

b3_f64 b3MatTexture::b3GetRefraction()
{
	return Refraction;
}

b3_f64 b3MatTexture::b3GetIndexOfRefraction()
{
	return RefrValue;
}

b3_f64 b3MatTexture::b3GetSpecularExponent()
{
	return HighLight;
}


b3MatChess::b3MatChess(b3_u32 class_type) : b3Material(sizeof(b3MatChess),class_type) 
{
}

b3MatChess::b3MatChess(b3_u32 *src) : b3Material(src)
{
	b3InitColor(&DiffColor[0]);
	b3InitColor(&AmbColor[0]);
	b3InitColor(&SpecColor[0]);
	b3InitColor(&DiffColor[1]);
	b3InitColor(&AmbColor[1]);
	b3InitColor(&SpecColor[1]);
	Reflection[0] = b3InitFloat();
	Reflection[1] = b3InitFloat();
	Refraction[0] = b3InitFloat();
	Refraction[1] = b3InitFloat();
	RefrValue[0]  = b3InitFloat();
	RefrValue[1]  = b3InitFloat();
	HighLight[0]  = b3InitFloat();
	HighLight[1]  = b3InitFloat();
	Flags         = b3InitInt();
	xTimes        = b3InitInt();
	yTimes        = b3InitInt();
}


b3MatWrapTexture::b3MatWrapTexture(b3_u32 class_type) : b3Material(sizeof(b3MatWrapTexture),class_type) 
{
}

b3MatWrapTexture::b3MatWrapTexture(b3_u32 *src) : b3Material(src)
{
	Reflection = b3InitFloat();
	Refraction = b3InitFloat();
	RefrValue  = b3InitFloat();
	HighLight  = b3InitFloat();
	xStart     = b3InitFloat();
	yStart     = b3InitFloat();
	xEnd       = b3InitFloat();
	yEnd       = b3InitFloat();
	Texture    = (b3Tx *)b3InitNull();
	Flags      = b3InitInt();
	b3InitString(Name,B3_TEXSTRINGLEN);
}

b3_f64 b3MatWrapTexture::b3GetReflection()
{
	return Reflection;
}

b3_f64 b3MatWrapTexture::b3GetRefraction()
{
	return Refraction;
}

b3_f64 b3MatWrapTexture::b3GetIndexOfRefraction()
{
	return RefrValue;
}

b3_f64 b3MatWrapTexture::b3GetSpecularExponent()
{
	return HighLight;
}


b3MatMarble::b3MatMarble(b3_u32 class_type) : b3Material(sizeof(b3MatMarble),class_type) 
{
}

b3MatMarble::b3MatMarble(b3_u32 *src) : b3Material(src)
{
	b3InitColor(&DiffColor);
	b3InitColor(&AmbColor);
	b3InitColor(&SpecColor);
	b3InitVector(&Scale);
	Reflection = b3InitFloat();
	Refraction = b3InitFloat();
	RefrValue  = b3InitFloat();
	HighLight  = b3InitFloat();
	Flags      = b3InitInt();
	xTimes     = b3InitInt();
	yTimes     = b3InitInt();
}

b3_f64 b3MatMarble::b3GetReflection()
{
	return Reflection;
}

b3_f64 b3MatMarble::b3GetRefraction()
{
	return Refraction;
}

b3_f64 b3MatMarble::b3GetIndexOfRefraction()
{
	return RefrValue;
}

b3_f64 b3MatMarble::b3GetSpecularExponent()
{
	return HighLight;
}


b3MatSlide::b3MatSlide(b3_u32 class_type) : b3Material(sizeof(b3MatSlide),class_type) 
{
}

b3MatSlide::b3MatSlide(b3_u32 *src) : b3Material(src)
{
	b3InitColor(&m_Diffuse[0]);
	b3InitColor(&m_Ambient[0]);
	b3InitColor(&m_Specular[0]);
	b3InitColor(&m_Diffuse[1]);
	b3InitColor(&m_Ambient[1]);
	b3InitColor(&m_Specular[1]);
	m_From       = b3InitFloat();
	m_To         = b3InitFloat();
	m_Reflection = b3InitFloat();
	m_Refraction = b3InitFloat();
	m_RefrValue  = b3InitFloat();
	m_HighLight  = b3InitFloat();
	m_ModeFlag   = b3InitInt();
}

b3_f64 b3MatSlide::b3GetReflection()
{
	return m_Reflection;
}

b3_f64 b3MatSlide::b3GetRefraction()
{
	return m_Refraction;
}

b3_f64 b3MatSlide::b3GetIndexOfRefraction()
{
	return m_RefrValue;
}

b3_f64 b3MatSlide::b3GetSpecularExponent()
{
	return m_HighLight;
}

b3_bool b3MatSlide::b3GetColors(
	b3_polar *polar,
	b3_color *diffuse,
	b3_color *ambient,
	b3_color *specular)
{
	b3_f64 Factor;

	switch (m_ModeFlag)
	{
		case XSLIDE :
			Factor = (polar->polar.x - m_From) / (m_To - m_From);
			if (Factor < 0) Factor = 0;
			if (Factor > 1) Factor = 1;
            break;
		case YSLIDE :
			Factor = (polar->polar.y - m_From) / (m_To - m_From);
			if (Factor < 0) Factor = 0;
			if (Factor > 1) Factor = 1;
            break;
		case XSLIDE_CUT :
			Factor = (polar->polar.x - m_From) / (m_To - m_From);
			if ((Factor < 0) || (Factor > 1))
			{
				return false;
			}
			break;
		case YSLIDE_CUT :
			Factor = (polar->polar.y - m_From) / (m_To - m_From);
			if ((Factor < 0) || (Factor > 1))
			{
				return false;
			}
            break;
	}

	diffuse->r = m_Diffuse[0].r + Factor * (m_Diffuse[1].r - m_Diffuse[0].r);
	diffuse->g = m_Diffuse[0].g + Factor * (m_Diffuse[1].g - m_Diffuse[0].g);
	diffuse->b = m_Diffuse[0].b + Factor * (m_Diffuse[1].b - m_Diffuse[0].b);
	if (specular != null)
	{
		ambient->r  = m_Ambient[0].r  + Factor * (m_Ambient[1].r  - m_Ambient[0].r);
		ambient->g  = m_Ambient[0].g  + Factor * (m_Ambient[1].g  - m_Ambient[0].g);
		ambient->b  = m_Ambient[0].b  + Factor * (m_Ambient[1].b  - m_Ambient[0].b);
		specular->r = m_Specular[0].r + Factor * (m_Specular[1].r - m_Specular[0].r);
		specular->g = m_Specular[0].g + Factor * (m_Specular[1].g - m_Specular[0].g);
		specular->b = m_Specular[0].b + Factor * (m_Specular[1].b - m_Specular[0].b);
	}

	return (true);
}


b3MatWood::b3MatWood(b3_u32 class_type) : b3Material(sizeof(b3MatWood),class_type) 
{
}

b3MatWood::b3MatWood(b3_u32 *src) : b3Material(src)
{
	b3InitColor(&DiffColor);
	b3InitColor(&AmbColor);
	b3InitColor(&SpecColor);
	b3InitVector(&Scale);
	Reflection = b3InitFloat();
	Refraction = b3InitFloat();
	RefrValue  = b3InitFloat();
	HighLight  = b3InitFloat();
	Flags      = b3InitInt();
	xTimes     = b3InitInt();
	yTimes     = b3InitInt();
}

b3_f64 b3MatWood::b3GetReflection()
{
	return Reflection;
}

b3_f64 b3MatWood::b3GetRefraction()
{
	return Refraction;
}

b3_f64 b3MatWood::b3GetIndexOfRefraction()
{
	return RefrValue;
}

b3_f64 b3MatWood::b3GetSpecularExponent()
{
	return HighLight;
}
