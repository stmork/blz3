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
#include "blz3/base/b3Aux.h"
#include "blz3/base/b3Procedure.h"
#include "blz3/image/b3TxPool.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.17  2001/11/05 16:57:39  sm
**      - Creating demo scenes.
**      - Initializing some b3Item derived objects
**
**      Revision 1.16  2001/11/01 09:43:11  sm
**      - Some image logging cleanups.
**      - Texture preparing now in b3Prepare().
**      - Done some minor fixes.
**
**      Revision 1.15  2001/10/22 14:47:38  sm
**      - Type correction vor b3Base/b3Link. So fixed a bad behaviour
**        on Windows.
**      - Some minor fixes.
**
**      Revision 1.14  2001/10/20 16:25:07  sm
**      - Windows compile check an minor fixes
**
**      Revision 1.13  2001/10/19 18:27:28  sm
**      - Fixing LDC bug
**      - Optimizing color routines
**
**      Revision 1.12  2001/10/19 14:46:57  sm
**      - Rotation spline shape bug found.
**      - Major optimizations done.
**      - Cleanups
**
**      Revision 1.11  2001/10/15 14:45:08  sm
**      - Materials are accessing textures now.
**      - Created image viewer "bimg3"
**
**      Revision 1.10  2001/10/09 20:47:01  sm
**      - some further texture handling.
**
**      Revision 1.9  2001/10/07 20:41:32  sm
**      - Updating MSVC project status
**
**      Revision 1.8  2001/10/07 20:17:27  sm
**      - Prepared texture support.
**      - Noise procedures added.
**      - Added bump and material support.
**      - Added soft shadows.
**
**      Revision 1.7  2001/10/06 19:24:17  sm
**      - New torus intersection routines and support routines
**      - Added further shading support from materials
**      - Added stencil checking
**      - Changed support for basis transformation for shapes with
**        at least three direction vectors.
**
**      Revision 1.6  2001/10/05 20:30:46  sm
**      - Introducing Mork and Phong shading.
**      - Using light source when shading
**
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

/*************************************************************************
**                                                                      **
**                        Base material                                 **
**                                                                      **
*************************************************************************/

b3Material::b3Material(b3_size class_size,b3_u32 class_type) : b3Item(class_size,class_type)
{
}

b3Material::b3Material(b3_u32 class_type) : b3Item(sizeof(b3Material),class_type)
{
}

b3Material::b3Material(b3_u32 *src) : b3Item(src)
{
}

b3_bool b3Material::b3Prepare()
{
	return true;
}

b3_bool b3Material::b3GetColors(
	b3_polar *polar,
	b3_color *diffuse,
	b3_color *ambient,
	b3_color *specular)
{
	return false;
}

b3_f64 b3Material::b3GetReflection(b3_polar *polar)
{
	return 0.0;
}

b3_f64 b3Material::b3GetRefraction(b3_polar *polar)
{
	return 0.0;
}

b3_f64 b3Material::b3GetIndexOfRefraction(b3_polar *polar)
{
	return 1.0;
}

b3_f64 b3Material::b3GetSpecularExponent(b3_polar *polar)
{
	return 100000.0;
}

/*************************************************************************
**                                                                      **
**                        Standard material                             **
**                                                                      **
*************************************************************************/

b3MatNormal::b3MatNormal(b3_u32 class_type) : b3Material(sizeof(b3MatNormal),class_type)
{
	b3Color::b3Init(&m_AmbColor);
	b3Color::b3Init(&m_DiffColor);
	b3Color::b3Init(&m_SpecColor);
	m_Reflection =    0;
	m_Refraction =    0;
	m_RefrValue  =    1;
	m_HighLight  = 1000;
	m_Flags      =    0;
}

b3MatNormal::b3MatNormal(b3_u32 *src) : b3Material(src)
{
	b3InitColor(&m_DiffColor);
	b3InitColor(&m_AmbColor);
	b3InitColor(&m_SpecColor);
	m_Reflection = b3InitFloat();
	m_Refraction = b3InitFloat();
	m_RefrValue  = b3InitFloat();
	m_HighLight  = b3InitFloat();
	m_Flags      = b3InitInt();
}

b3_bool b3MatNormal::b3GetColors(
	b3_polar *polar,
	b3_color *diffuse,
	b3_color *ambient,
	b3_color *specular)
{
	*diffuse  = m_DiffColor;
	*ambient  = m_AmbColor;
	*specular = m_SpecColor;
	return true;
}

b3_f64 b3MatNormal::b3GetReflection(b3_polar *polar)
{
	return m_Reflection;
}

b3_f64 b3MatNormal::b3GetRefraction(b3_polar *polar)
{
	return m_Refraction;
}

b3_f64 b3MatNormal::b3GetIndexOfRefraction(b3_polar *polar)
{
	return m_RefrValue;
}

b3_f64 b3MatNormal::b3GetSpecularExponent(b3_polar *polar)
{
	return m_HighLight;
}

/*************************************************************************
**                                                                      **
**                        Chess material                                **
**                                                                      **
*************************************************************************/

b3MatChess::b3MatChess(b3_u32 class_type) : b3Material(sizeof(b3MatChess),class_type) 
{
	b3Color::b3GetColor(&m_AmbColor[0], B3_GREY);
	b3Color::b3GetColor(&m_DiffColor[0],B3_BLACK);
	b3Color::b3GetColor(&m_SpecColor[0],B3_GREY);
	
	b3Color::b3GetColor(&m_AmbColor[1], B3_GREY);
	b3Color::b3GetColor(&m_DiffColor[1],B3_WHITE);
	b3Color::b3GetColor(&m_SpecColor[1],B3_GREY);

	m_Reflection[0] = m_Reflection[1] =    0;
	m_Refraction[0] = m_Refraction[1] =    0;
	m_RefrValue[0]  = m_RefrValue[1]  =    1;
	m_HighLight[0]  = m_HighLight[1]  = 1000;

	m_Flags         =    0;
	m_xTimes        =    8;
	m_yTimes        =    8;
}

b3MatChess::b3MatChess(b3_u32 *src) : b3Material(src)
{
	b3InitColor(&m_DiffColor[0]);
	b3InitColor(&m_AmbColor[0]);
	b3InitColor(&m_SpecColor[0]);
	b3InitColor(&m_DiffColor[1]);
	b3InitColor(&m_AmbColor[1]);
	b3InitColor(&m_SpecColor[1]);
	m_Reflection[0] = b3InitFloat();
	m_Reflection[1] = b3InitFloat();
	m_Refraction[0] = b3InitFloat();
	m_Refraction[1] = b3InitFloat();
	m_RefrValue[0]  = b3InitFloat();
	m_RefrValue[1]  = b3InitFloat();
	m_HighLight[0]  = b3InitFloat();
	m_HighLight[1]  = b3InitFloat();
	m_Flags         = b3InitInt();
	m_xTimes        = b3InitInt();
	m_yTimes        = b3InitInt();
}

#define CHESS_INDEX(x,y) (((b3_index)(((x) + 1) * m_xTimes) + (b3_index)(((y) + 1) * m_yTimes) + 1) & 1)

b3_bool b3MatChess::b3GetColors(
	b3_polar *polar,
	b3_color *diffuse,
	b3_color *ambient,
	b3_color *specular)
{
	b3_index index;

	index = CHESS_INDEX(polar->polar.x,polar->polar.y);
	*diffuse  = m_DiffColor[index];
	*ambient  = m_AmbColor[index];
	*specular = m_SpecColor[index];

	return true;
}

b3_f64 b3MatChess::b3GetReflection(b3_polar *polar)
{
	return m_Reflection[CHESS_INDEX(polar->polar.x,polar->polar.y)];
}

b3_f64 b3MatChess::b3GetRefraction(b3_polar *polar)
{
	return m_Refraction[CHESS_INDEX(polar->polar.x,polar->polar.y)];
}

b3_f64 b3MatChess::b3GetIndexOfRefraction(b3_polar *polar)
{
	return m_RefrValue[CHESS_INDEX(polar->polar.x,polar->polar.y)];
}

b3_f64 b3MatChess::b3GetSpecularExponent(b3_polar *polar)
{
	return m_HighLight[CHESS_INDEX(polar->polar.x,polar->polar.y)];
}

/*************************************************************************
**                                                                      **
**                        Texture material                              **
**                                                                      **
*************************************************************************/

b3MatTexture::b3MatTexture(b3_u32 class_type) : b3Material(sizeof(b3MatTexture),class_type) 
{
}

b3MatTexture::b3MatTexture(b3_u32 *src) : b3Material(src)
{
	m_Reflection = b3InitFloat();
	m_Refraction = b3InitFloat();
	m_RefrValue  = b3InitFloat();
	m_HighLight  = b3InitFloat();
	m_xStart     = b3InitFloat();
	m_yStart     = b3InitFloat();
	m_xScale     = b3InitFloat();
	m_yScale     = b3InitFloat();
	m_xTimes     = b3InitInt();
	m_yTimes     = b3InitInt();
	m_Texture    = (b3Tx *)b3InitNull();
	m_Flags      = b3InitInt();
	b3InitString(m_Name,B3_TEXSTRINGLEN);
}

b3_bool b3MatTexture::b3Prepare()
{
	return b3CheckTexture(&m_Texture,m_Name);
}

b3_bool b3MatTexture::b3GetColors(
	b3_polar *polar,
	b3_color *diffuse,
	b3_color *ambient,
	b3_color *specular)
{
	b3_coord     x,y;
	b3_f64       fx,fy;

	fx = (polar->polar.x - m_xStart) / m_xScale;
	if (m_Flags & MAT_XINVERT) fx = 1.0 - fx;
	if ((fx < 0) || (fx >= m_xTimes))
	{
		return false;
	}

	fy = (polar->polar.y - m_yStart) / m_yScale;
	if (m_Flags & MAT_YINVERT) fy = 1.0 - fy;
	if ((fy < 0) || (fy >= m_yTimes))
	{
		return false;
	}
	x = (b3_coord)((fx - (b3_coord)fx) * m_Texture->xSize);
	y = (b3_coord)((fy - (b3_coord)fy) * m_Texture->ySize);

	b3Color::b3GetColor(diffuse,m_Texture->b3GetValue(x,y));
	ambient->r  = diffuse->r * 0.3;
	ambient->g  = diffuse->g * 0.3;
	ambient->b  = diffuse->b * 0.3;
	specular->r =
	specular->g =
	specular->b = 0.7f;

	return true;
}

b3_f64 b3MatTexture::b3GetReflection(b3_polar *polar)
{
	return m_Reflection;
}

b3_f64 b3MatTexture::b3GetRefraction(b3_polar *polar)
{
	return m_Refraction;
}

b3_f64 b3MatTexture::b3GetIndexOfRefraction(b3_polar *polar)
{
	return m_RefrValue;
}

b3_f64 b3MatTexture::b3GetSpecularExponent(b3_polar *polar)
{
	return m_HighLight;
}

/*************************************************************************
**                                                                      **
**                        Wrapping texture material                     **
**                                                                      **
*************************************************************************/

b3MatWrapTexture::b3MatWrapTexture(b3_u32 class_type) :
	b3Material(sizeof(b3MatWrapTexture),class_type) 
{
}

b3MatWrapTexture::b3MatWrapTexture(b3_u32 *src) : b3Material(src)
{
	m_Reflection = b3InitFloat();
	m_Refraction = b3InitFloat();
	m_RefrValue  = b3InitFloat();
	m_HighLight  = b3InitFloat();
	m_xStart     = b3InitFloat();
	m_yStart     = b3InitFloat();
	m_xEnd       = b3InitFloat();
	m_yEnd       = b3InitFloat();
	m_Texture    = (b3Tx *)b3InitNull();
	m_Flags      = b3InitInt();
	b3InitString(m_Name,B3_TEXSTRINGLEN);
}

b3_bool b3MatWrapTexture::b3Prepare()
{
	return b3CheckTexture(&m_Texture,m_Name);
}

b3_bool b3MatWrapTexture::b3GetColors(
	b3_polar *polar,
	b3_color *diffuse,
	b3_color *ambient,
	b3_color *specular)
{
	b3_coord     x,y;
	b3_f64       fx,fy,xEnd,xPolar;

	if ((polar->polar.y >= m_yStart) && (polar->polar.y <= m_yEnd))
	{
		xEnd	= m_xEnd;
		xPolar	= polar->polar.x;
		fy = (polar->polar.y - m_yStart) /
			(m_yEnd - m_yStart);
		if (m_Flags & MAT_YINVERT) fy = 1.0 - fy;
		if ((fy < 0) || (fy > 1))
		{
			return false;
		}
		y = (long)(fy * m_Texture->ySize);
		if (m_xStart > xEnd)
		{
			if ((xPolar >= m_xStart) || (xPolar <= xEnd))
			{
				if (xPolar < 0)
				{
					return false;
				}
				if (xPolar < m_xStart) xPolar++;
				xEnd++;
				fx = (xPolar - m_xStart) / (xEnd - m_xStart);
				if (m_Flags & MAT_XINVERT) fx = 1.0 - fx;
				x  = (b3_coord)(fx * m_Texture->xSize);
			}
			else
			{
				return false;
			}
		}
		else
		{
			if ((xPolar <= m_xStart) || (xPolar >= xEnd))
			{
				return false;
			}
			fx = (xPolar - m_xStart) / (xEnd  - m_xStart);
			if (m_Flags & MAT_XINVERT) fx = 1.0 - fx;
			x  = (long)(fx * m_Texture->xSize);
		}
	}
	else
	{
		return false;
	}

	b3Color::b3GetColor(diffuse,m_Texture->b3GetValue(x,y));
	b3Color::b3Scale(diffuse,0.3,ambient);
	specular->r =
	specular->g =
	specular->b = 0.7f;

	return true;
}

b3_f64 b3MatWrapTexture::b3GetReflection(b3_polar *polar)
{
	return m_Reflection;
}

b3_f64 b3MatWrapTexture::b3GetRefraction(b3_polar *polar)
{
	return m_Refraction;
}

b3_f64 b3MatWrapTexture::b3GetIndexOfRefraction(b3_polar *polar)
{
	return m_RefrValue;
}

b3_f64 b3MatWrapTexture::b3GetSpecularExponent(b3_polar *polar)
{
	return m_HighLight;
}

/*************************************************************************
**                                                                      **
**                        Color sliding material                        **
**                                                                      **
*************************************************************************/

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

		default:
			Factor = 0.5;
			break;
	}

	diffuse->r  = m_Diffuse[0].r  + Factor * (m_Diffuse[1].r  - m_Diffuse[0].r);
	diffuse->g  = m_Diffuse[0].g  + Factor * (m_Diffuse[1].g  - m_Diffuse[0].g);
	diffuse->b  = m_Diffuse[0].b  + Factor * (m_Diffuse[1].b  - m_Diffuse[0].b);
	ambient->r  = m_Ambient[0].r  + Factor * (m_Ambient[1].r  - m_Ambient[0].r);
	ambient->g  = m_Ambient[0].g  + Factor * (m_Ambient[1].g  - m_Ambient[0].g);
	ambient->b  = m_Ambient[0].b  + Factor * (m_Ambient[1].b  - m_Ambient[0].b);
	specular->r = m_Specular[0].r + Factor * (m_Specular[1].r - m_Specular[0].r);
	specular->g = m_Specular[0].g + Factor * (m_Specular[1].g - m_Specular[0].g);
	specular->b = m_Specular[0].b + Factor * (m_Specular[1].b - m_Specular[0].b);

	return true;
}

b3_f64 b3MatSlide::b3GetReflection(b3_polar *polar)
{
	return m_Reflection;
}

b3_f64 b3MatSlide::b3GetRefraction(b3_polar *polar)
{
	return m_Refraction;
}

b3_f64 b3MatSlide::b3GetIndexOfRefraction(b3_polar *polar)
{
	return m_RefrValue;
}

b3_f64 b3MatSlide::b3GetSpecularExponent(b3_polar *polar)
{
	return m_HighLight;
}

/*************************************************************************
**                                                                      **
**                        Marble material                               **
**                                                                      **
*************************************************************************/

b3MatMarble::b3MatMarble(b3_u32 class_type) : b3Material(sizeof(b3MatMarble),class_type) 
{
}

b3MatMarble::b3MatMarble(b3_u32 *src) : b3Material(src)
{
	b3InitColor(&m_DiffColor);
	b3InitColor(&m_AmbColor);
	b3InitColor(&m_SpecColor);
	b3InitVector(&m_Scale);
	m_Reflection = b3InitFloat();
	m_Refraction = b3InitFloat();
	m_RefrValue  = b3InitFloat();
	m_HighLight  = b3InitFloat();
	m_Flags      = b3InitInt();
	m_xTimes     = b3InitInt();
	m_yTimes     = b3InitInt();
}

b3_bool b3MatMarble::b3GetColors(
	b3_polar *polar,
	b3_color *diffuse,
	b3_color *ambient,
	b3_color *specular)
{
	b3_color   mask;
	b3_vector  d;

	d.x = polar->box_polar.x * m_Scale.x * M_PI;
	d.y = polar->box_polar.y * m_Scale.y * M_PI;
	d.z = polar->box_polar.z * m_Scale.z * M_PI;

	noise_procedures.b3Marble (&d,&mask);

	diffuse->a  = 0;
	diffuse->r  = m_DiffColor.r * mask.r;
	diffuse->g  = m_DiffColor.g * mask.g;
	diffuse->b  = m_DiffColor.b * mask.b;

	ambient->a  = 0;
	ambient->r  = m_AmbColor.r  * mask.r;
	ambient->g  = m_AmbColor.g  * mask.g;
	ambient->b  = m_AmbColor.b  * mask.b;

	specular->a	= 0;
	specular->r	= m_SpecColor.r * mask.r;
	specular->g	= m_SpecColor.g * mask.g;
	specular->b	= m_SpecColor.b * mask.b;

	return true;
}

b3_f64 b3MatMarble::b3GetReflection(b3_polar *polar)
{
	return m_Reflection;
}

b3_f64 b3MatMarble::b3GetRefraction(b3_polar *polar)
{
	return m_Refraction;
}

b3_f64 b3MatMarble::b3GetIndexOfRefraction(b3_polar *polar)
{
	return m_RefrValue;
}

b3_f64 b3MatMarble::b3GetSpecularExponent(b3_polar *polar)
{
	return m_HighLight;
}

/*************************************************************************
**                                                                      **
**                        Wooden material                               **
**                                                                      **
*************************************************************************/

b3MatWood::b3MatWood(b3_u32 class_type) : b3Material(sizeof(b3MatWood),class_type) 
{
}

b3MatWood::b3MatWood(b3_u32 *src) : b3Material(src)
{
	b3InitColor(&m_DiffColor);
	b3InitColor(&m_AmbColor);
	b3InitColor(&m_SpecColor);
	b3InitVector(&m_Scale);
	m_Reflection = b3InitFloat();
	m_Refraction = b3InitFloat();
	m_RefrValue  = b3InitFloat();
	m_HighLight  = b3InitFloat();
	m_Flags      = b3InitInt();
	m_xTimes     = b3InitInt();
	m_yTimes     = b3InitInt();
}

b3_bool b3MatWood::b3GetColors(
	b3_polar *polar,
	b3_color *diffuse,
	b3_color *ambient,
	b3_color *specular)
{
	b3_color  mask;
	b3_vector d;

	d.x = ((polar->box_polar.x - 0.5) * m_Scale.x * M_PI);
	d.y = ((polar->box_polar.y - 0.5) * m_Scale.y * M_PI);
	d.z = ((polar->box_polar.z - 0.5) * m_Scale.z * M_PI);

	noise_procedures.b3Wood (&d,&mask);

	diffuse->a  = 0;
	diffuse->r  = m_DiffColor.r * mask.r;
	diffuse->g  = m_DiffColor.g * mask.g;
	diffuse->b  = m_DiffColor.b * mask.b;

	ambient->a  = 0;
	ambient->r  = m_AmbColor.r  * mask.r;
	ambient->g  = m_AmbColor.g  * mask.g;
	ambient->b  = m_AmbColor.b  * mask.b;

	specular->a = 0;
	specular->r = m_SpecColor.r * mask.r;
	specular->g = m_SpecColor.g * mask.g;
	specular->b = m_SpecColor.b * mask.b;

	return true;
}

b3_f64 b3MatWood::b3GetReflection(b3_polar *polar)
{
	return m_Reflection;
}

b3_f64 b3MatWood::b3GetRefraction(b3_polar *polar)
{
	return m_Refraction;
}

b3_f64 b3MatWood::b3GetIndexOfRefraction(b3_polar *polar)
{
	return m_RefrValue;
}

b3_f64 b3MatWood::b3GetSpecularExponent(b3_polar *polar)
{
	return m_HighLight;
}
