/*
**
**      $Filename:      b3Material.cc $
**      $Release:       Dortmund 2001, 2002, 2003, 2004 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing materials
**
**      (C) Copyright 2001, 2002, 2003, 2004  Steffen A. Mork
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
#include "blz3/base/b3Math.h"
#include "blz3/base/b3Procedure.h"
#include "blz3/base/b3Matrix.h"
#include "blz3/image/b3TxPool.h"

#define no_DEBUG_MATERIAL

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.56  2004/04/11 18:21:36  sm
**      - Raytracer redesign:
**        o The complete set of surface values moved into
**          the b3_surface data structure when calling b3GetColors()
**
**      Revision 1.55  2004/04/11 14:05:11  sm
**      - Raytracer redesign:
**        o The reflection/refraction/ior/specular exponent getter
**          are removed. The values are copied via the b3GetColors()
**          method.
**        o The polar members are renamed.
**        o The shape/bbox pointers moved into the ray structure
**      - Introduced wood bump mapping.
**
**      Revision 1.54  2004/04/10 19:12:46  sm
**      - Splitted up some header/source files:
**        o b3Wood/b3OakPlank
**        o b3MaterialSampler
**      - Cleaneup
**
**      Revision 1.53  2004/04/10 14:33:25  sm
**      - Added oak plank support.
**
**      Revision 1.52  2004/04/10 13:45:30  sm
**      - Added wooden oak planks.
**
**      Revision 1.51  2004/04/09 14:11:58  sm
**      - Removed CRs
**
**      Revision 1.50  2004/04/07 16:07:17  sm
**      - Moved wood computing outside b3MatWood for use in its own bump map.
**
**      Revision 1.49  2004/04/04 19:28:25  sm
**      - New wood dialog
**
**      Revision 1.48  2004/04/04 13:50:38  sm
**      - Optimized noise
**      - Added filtered noise
**
**      Revision 1.47  2004/04/04 12:57:52  sm
**      - Fixed wood alignment.
**
**      Revision 1.46  2004/04/04 10:49:34  sm
**      - Added further wood development.
**
**      Revision 1.45  2004/04/03 19:25:00  sm
**      - Some other wood
**
**      Revision 1.44  2004/03/18 14:07:05  sm
**      - Changed granite icons
**
**      Revision 1.43  2004/03/18 13:43:05  sm
**      - Some better granite:
**        o Introduced configurable overtones.
**        o Better turbulence computation.
**
**      Revision 1.42  2004/03/15 18:49:57  sm
**      - New example material created.
**
**      Revision 1.41  2004/03/15 10:38:37  sm
**      - Found good values for granite.
**
**      Revision 1.40  2004/03/15 08:28:46  sm
**      - Now saving granite color definitions.
**
**      Revision 1.39  2004/03/14 16:18:26  sm
**      - Added Windows support for granite.
**
**      Revision 1.38  2004/03/14 08:19:17  sm
**      - Added granite material.
**
**      Revision 1.37  2004/03/07 19:22:30  sm
**      - Shadow added to Cook/Torrance material
**
**      Revision 1.36  2004/03/05 13:20:20  sm
**      - Some additional test materials added.
**
**      Revision 1.35  2004/03/02 09:07:17  sm
**      - Added read/write support for Cook/Torrance material.
**      - Added test module for Cook/Torrance reflection model.
**      - Fixed camera name handling if camera name is empty.
**
**      Revision 1.34  2004/03/01 19:52:00  sm
**      - Some cleanup
**
**      Revision 1.33  2004/03/01 16:28:00  sm
**      - Removed tempora Cook/Torrance for normal material.
**
**      Revision 1.32  2004/03/01 14:00:32  sm
**      - Ready to go for Cook/Torrance reflectance model.
**
**      Revision 1.31  2004/03/01 11:05:24  sm
**      - Further Cook/Torrance development.
**
**      Revision 1.30  2004/02/29 18:44:55  sm
**      - Further shader development
**
**      Revision 1.29  2004/02/28 19:10:13  sm
**      - Cook/Torrance is applicable by use through material
**        shader.
**
**      Revision 1.28  2004/02/28 13:51:53  sm
**      - Added Cook/Torrance material. But this is not a material
**        it is a shader! Further reading is necessary to redesign
**        Blizzard III to use programmable shader.
**
**      Revision 1.27  2003/08/31 10:44:07  sm
**      - Further buffer overflow avoidments.
**
**      Revision 1.26  2003/07/12 17:44:47  sm
**      - Cleaned up raytracing b3Item registration
**
**      Revision 1.25  2003/03/04 20:37:38  sm
**      - Introducing new b3Color which brings some
**        performance!
**
**      Revision 1.24  2003/02/22 17:21:34  sm
**      - Changed some global variables into static class members:
**        o b3Scene::epsilon
**        o b3Scene::m_TexturePool et. al.
**        o b3SplineTemplate<class VECTOR>::bspline_errno
**
**      Revision 1.23  2002/12/11 14:47:58  sm
**      - Changed noise handling to static
**      - Fixed some error cases when image not loaded.
**      - Added some brt3 flags
**
**      Revision 1.22  2002/03/03 21:22:22  sm
**      - Added support for creating surfaces using profile curves.
**      - Added simple creating of triangle fields.
**
**      Revision 1.21  2002/03/02 19:52:39  sm
**      - Nasty UnCR
**      - Fixed some compile bugs due to incompatibilities to Visual C++
**
**      Revision 1.20  2002/02/26 20:43:28  sm
**      - Moved creation dialogs into property sheets
**      - Added material creation dialog
**
**      Revision 1.19  2001/12/30 14:16:58  sm
**      - Abstracted b3File to b3FileAbstract to implement b3FileMem (not done yet).
**      - b3Item writing implemented and updated all raytracing classes
**        to work properly.
**      - Cleaned up spline shapes and CSG shapes.
**      - Added b3Caustic class for compatibility reasons.
**
**      Revision 1.18  2001/11/08 19:31:33  sm
**      - Nasty CR/LF removal!
**      - Added TGA/RGB8/PostScript image saving.
**      - Hoping to win Peter H. for powerful MFC programming...
**
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

void b3Material::b3Register()
{
	b3PrintF (B3LOG_DEBUG,"Registering materials...\n");
#ifndef DEBUG_MATERIAL
	b3Item::b3Register(&b3MatNormal::b3StaticInit,       &b3MatNormal::b3StaticInit,       MAT_NORMAL);
#endif
	b3Item::b3Register(&b3MatTexture::b3StaticInit,      &b3MatTexture::b3StaticInit,      TEXTURE);
	b3Item::b3Register(&b3MatChess::b3StaticInit,        &b3MatChess::b3StaticInit,        CHESS);
	b3Item::b3Register(&b3MatWrapTexture::b3StaticInit,  &b3MatWrapTexture::b3StaticInit,  WRAPTEXTURE);
	b3Item::b3Register(&b3MatMarble::b3StaticInit,       &b3MatMarble::b3StaticInit,       MARBLE);
	b3Item::b3Register(&b3MatSlide::b3StaticInit,        &b3MatSlide::b3StaticInit,        SLIDE);
	b3Item::b3Register(&b3MatWood::b3StaticInit,         &b3MatWood::b3StaticInit,         WOOD);
	b3Item::b3Register(&b3MatOakPlank::b3StaticInit,     &b3MatOakPlank::b3StaticInit,     OAKPLANK);
	b3Item::b3Register(&b3MatCookTorrance::b3StaticInit, &b3MatCookTorrance::b3StaticInit, COOK_TORRANCE);
#ifndef DEBUG_MATERIAL
	b3Item::b3Register(&b3MatGranite::b3StaticInit,      &b3MatGranite::b3StaticInit,      GRANITE);
#else
	b3Item::b3Register(&b3MatGranite::b3StaticInit,      &b3MatGranite::b3StaticInit,      MAT_NORMAL);
#endif
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

b3_bool b3Material::b3GetColors(b3_ray *ray,b3_surface *surface)
{
	surface->m_Reflection  =      0.0;
	surface->m_Refraction  =      0.0;
	surface->m_Ior         =      1.0;
	surface->m_SpecularExp = 100000.0;

	return false;
}

/*************************************************************************
**                                                                      **
**                        Standard material                             **
**                                                                      **
*************************************************************************/

b3MatNormal::b3MatNormal(b3_size class_size,b3_u32 class_type) : b3Material(class_size,class_type)
{
	m_AmbColor.b3Init();
	m_DiffColor.b3Init();
	m_SpecColor.b3Init();
	m_Reflection =    0;
	m_Refraction =    0;
	m_RefrValue  =    1;
	m_HighLight  = 1000;
	m_Flags      =    0;
}

b3MatNormal::b3MatNormal(b3_u32 class_type) : b3Material(sizeof(b3MatNormal),class_type)
{
	m_AmbColor.b3Init();
	m_DiffColor.b3Init();
	m_SpecColor.b3Init();
	m_Reflection =    0;
	m_Refraction =    0;
	m_RefrValue  =    1;
	m_HighLight  = 1000;
	m_Flags      =    0;
}

b3MatNormal::b3MatNormal(b3_u32 *src) : b3Material(src)
{
	b3InitColor(m_DiffColor);
	b3InitColor(m_AmbColor);
	b3InitColor(m_SpecColor);
	m_Reflection = b3InitFloat();
	m_Refraction = b3InitFloat();
	m_RefrValue  = b3InitFloat();
	m_HighLight  = b3InitFloat();
	m_Flags      = b3InitInt();
}

void b3MatNormal::b3Write()
{
	b3StoreColor(m_DiffColor);
	b3StoreColor(m_AmbColor);
	b3StoreColor(m_SpecColor);
	b3StoreFloat(m_Reflection);
	b3StoreFloat(m_Refraction);
	b3StoreFloat(m_RefrValue);
	b3StoreFloat(m_HighLight);
	b3StoreInt  (m_Flags);
}

b3_bool b3MatNormal::b3GetColors(b3_ray *ray,b3_surface *surface)
{
	surface->m_Diffuse  = m_DiffColor;
	surface->m_Ambient  = m_AmbColor;
	surface->m_Specular = m_SpecColor;

	surface->m_Reflection  = m_Reflection;
	surface->m_Refraction  = m_Refraction;
	surface->m_Ior         = m_RefrValue;
	surface->m_SpecularExp = m_HighLight;

	return true;
}

/*************************************************************************
**                                                                      **
**                        Chess material                                **
**                                                                      **
*************************************************************************/

b3MatChess::b3MatChess(b3_u32 class_type) : b3Material(sizeof(b3MatChess),class_type) 
{
	m_AmbColor[0]  = b3Color(B3_GREY);
	m_DiffColor[0] = b3Color(B3_BLACK);
	m_SpecColor[0] = b3Color(B3_GREY);

	m_AmbColor[1]  = b3Color(B3_GREY);
	m_DiffColor[1] = b3Color(B3_WHITE);
	m_SpecColor[1] = b3Color(B3_GREY);
	
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
	b3InitColor(m_DiffColor[0]);
	b3InitColor(m_AmbColor[0]);
	b3InitColor(m_SpecColor[0]);
	b3InitColor(m_DiffColor[1]);
	b3InitColor(m_AmbColor[1]);
	b3InitColor(m_SpecColor[1]);
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

void b3MatChess::b3Write()
{
	b3StoreColor(m_DiffColor[0]);
	b3StoreColor(m_AmbColor[0]);
	b3StoreColor(m_SpecColor[0]);
	b3StoreColor(m_DiffColor[1]);
	b3StoreColor(m_AmbColor[1]);
	b3StoreColor(m_SpecColor[1]);
	b3StoreFloat(m_Reflection[0]);
	b3StoreFloat(m_Reflection[1]);
	b3StoreFloat(m_Refraction[0]);
	b3StoreFloat(m_Refraction[1]);
	b3StoreFloat(m_RefrValue[0]);
	b3StoreFloat(m_RefrValue[1]);
	b3StoreFloat(m_HighLight[0]);
	b3StoreFloat(m_HighLight[1]);
	b3StoreInt  (m_Flags);
	b3StoreCount(m_xTimes);
	b3StoreCount(m_yTimes);
}

#define CHESS_INDEX(x,y) (((b3_index)(((x) + 1) * m_xTimes) + (b3_index)(((y) + 1) * m_yTimes) + 1) & 1)

b3_bool b3MatChess::b3GetColors(b3_ray *ray,b3_surface *surface)
{
	b3_index index;

	index    = CHESS_INDEX(ray->polar.m_Polar.x,ray->polar.m_Polar.y);
	surface->m_Diffuse  = m_DiffColor[index];
	surface->m_Ambient  = m_AmbColor[index];
	surface->m_Specular = m_SpecColor[index];
	
	surface->m_Reflection  = m_Reflection[index];
	surface->m_Refraction  = m_Refraction[index];
	surface->m_Ior         = m_RefrValue[index];
	surface->m_SpecularExp = m_HighLight[index];

	return true;
}

/*************************************************************************
**                                                                      **
**                        Texture material                              **
**                                                                      **
*************************************************************************/

b3MatTexture::b3MatTexture(b3_u32 class_type) : b3Material(sizeof(b3MatTexture),class_type) 
{
	// Init material values
	m_Reflection =    0;
	m_Refraction =    0;
	m_RefrValue  =    1;
	m_HighLight  = 1000;

	// Init texture repetition values
	m_xStart     =  0;
	m_yStart     =  1;
	m_xScale     =  1;
	m_yScale     = -1;
	m_xTimes     =  2;
	m_yTimes     =  2;

	// Init texture
	m_Name.b3Empty();
	m_Texture = null;
	m_Flags   = 0;
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

void b3MatTexture::b3Write()
{
	b3StoreFloat(m_Reflection);
	b3StoreFloat(m_Refraction);
	b3StoreFloat(m_RefrValue);
	b3StoreFloat(m_HighLight);
	b3StoreFloat(m_xStart);
	b3StoreFloat(m_yStart);
	b3StoreFloat(m_xScale);
	b3StoreFloat(m_yScale);
	b3StoreCount(m_xTimes);
	b3StoreCount(m_yTimes);
	b3StoreNull();
	b3StoreInt(m_Flags);
	b3StoreString(m_Name,B3_TEXSTRINGLEN);
}

b3_bool b3MatTexture::b3Prepare()
{
	return b3Scene::b3CheckTexture(&m_Texture,m_Name);
}

void b3MatTexture::b3SetTexture(const char *name)
{
	m_Name.b3Format("%s",name);
}

b3_bool b3MatTexture::b3GetColors(b3_ray *ray,b3_surface *surface)
{
	b3_coord     x,y;
	b3_f64       fx,fy;

	fx = (ray->polar.m_Polar.x - m_xStart) / m_xScale;
	if (m_Flags & MAT_XINVERT) fx = 1.0 - fx;
	if ((fx < 0) || (fx >= m_xTimes))
	{
		return false;
	}

	fy = (ray->polar.m_Polar.y - m_yStart) / m_yScale;
	if (m_Flags & MAT_YINVERT) fy = 1.0 - fy;
	if ((fy < 0) || (fy >= m_yTimes))
	{
		return false;
	}
	x = (b3_coord)((fx - (b3_coord)fx) * m_Texture->xSize);
	y = (b3_coord)((fy - (b3_coord)fy) * m_Texture->ySize);

	surface->m_Diffuse = b3Color(m_Texture->b3GetValue(x,y));
	surface->m_Ambient = surface->m_Diffuse * 0.3;
	surface->m_Specular.b3Init(0.7f,0.7f,0.7f);

	surface->m_Reflection  = m_Reflection;
	surface->m_Refraction  = m_Refraction;
	surface->m_Ior         = m_RefrValue;
	surface->m_SpecularExp = m_HighLight;

	return true;
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

void b3MatWrapTexture::b3Write()
{
	b3StoreFloat(m_Reflection);
	b3StoreFloat(m_Refraction);
	b3StoreFloat(m_RefrValue);
	b3StoreFloat(m_HighLight);
	b3StoreFloat(m_xStart);
	b3StoreFloat(m_yStart);
	b3StoreFloat(m_xEnd);
	b3StoreFloat(m_yEnd);
	b3StoreNull();
	b3StoreInt(m_Flags);
	b3StoreString(m_Name,B3_TEXSTRINGLEN);
}

b3_bool b3MatWrapTexture::b3Prepare()
{
	return b3Scene::b3CheckTexture(&m_Texture,m_Name);
}

b3_bool b3MatWrapTexture::b3GetColors(b3_ray *ray,b3_surface *surface)
{
	b3_coord     x,y;
	b3_f64       fx,fy,xEnd,xPolar;

	if ((ray->polar.m_Polar.y >= m_yStart) && (ray->polar.m_Polar.y <= m_yEnd))
	{
		xEnd	= m_xEnd;
		xPolar	= ray->polar.m_Polar.x;
		fy = (ray->polar.m_Polar.y - m_yStart) /
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

	surface->m_Diffuse = b3Color(m_Texture->b3GetValue(x,y));
	surface->m_Ambient = surface->m_Diffuse * 0.3;
	surface->m_Specular.b3Init(0.7f,0.7f,0.7f);

	surface->m_Reflection  = m_Reflection;
	surface->m_Refraction  = m_Refraction;
	surface->m_Ior         = m_RefrValue;
	surface->m_SpecularExp = m_HighLight;

	return true;
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
	b3InitColor(m_Diffuse[0]);
	b3InitColor(m_Ambient[0]);
	b3InitColor(m_Specular[0]);
	b3InitColor(m_Diffuse[1]);
	b3InitColor(m_Ambient[1]);
	b3InitColor(m_Specular[1]);
	m_From       = b3InitFloat();
	m_To         = b3InitFloat();
	m_Reflection = b3InitFloat();
	m_Refraction = b3InitFloat();
	m_RefrValue  = b3InitFloat();
	m_HighLight  = b3InitFloat();
	m_ModeFlag   = b3InitInt();
}

void b3MatSlide::b3Write()
{
	b3StoreColor(m_Diffuse[0]);
	b3StoreColor(m_Ambient[0]);
	b3StoreColor(m_Specular[0]);
	b3StoreColor(m_Diffuse[1]);
	b3StoreColor(m_Ambient[1]);
	b3StoreColor(m_Specular[1]);
	b3StoreFloat(m_From);
	b3StoreFloat(m_To);
	b3StoreFloat(m_Reflection);
	b3StoreFloat(m_Refraction);
	b3StoreFloat(m_RefrValue);
	b3StoreFloat(m_HighLight);
	b3StoreInt(m_ModeFlag);
}

b3_bool b3MatSlide::b3GetColors(b3_ray *ray,b3_surface *surface)
{
	b3_f64 Factor;

	switch (m_ModeFlag)
	{
		case XSLIDE :
			Factor = (ray->polar.m_Polar.x - m_From) / (m_To - m_From);
			if (Factor < 0) Factor = 0;
			if (Factor > 1) Factor = 1;
            break;
		case YSLIDE :
			Factor = (ray->polar.m_Polar.y - m_From) / (m_To - m_From);
			if (Factor < 0) Factor = 0;
			if (Factor > 1) Factor = 1;
            break;
		case XSLIDE_CUT :
			Factor = (ray->polar.m_Polar.x - m_From) / (m_To - m_From);
			if ((Factor < 0) || (Factor > 1))
			{
				return false;
			}
			break;
		case YSLIDE_CUT :
			Factor = (ray->polar.m_Polar.y - m_From) / (m_To - m_From);
			if ((Factor < 0) || (Factor > 1))
			{
				return false;
			}
            break;

		default:
			Factor = 0.5;
			break;
	}

	surface->m_Diffuse  = m_Diffuse[0]  + (m_Diffuse[1]  - m_Diffuse[0]) * Factor;
	surface->m_Ambient  = m_Ambient[0]  + (m_Ambient[1]  - m_Ambient[0]) * Factor;
	surface->m_Specular = m_Specular[0] + (m_Specular[1] - m_Specular[0]) * Factor;

	surface->m_Reflection  = m_Reflection;
	surface->m_Refraction  = m_Refraction;
	surface->m_Ior         = m_RefrValue;
	surface->m_SpecularExp = m_HighLight;

	return true;
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
	b3InitColor(m_DiffColor);
	b3InitColor(m_AmbColor);
	b3InitColor(m_SpecColor);
	b3InitVector(&m_Scale);
	m_Reflection = b3InitFloat();
	m_Refraction = b3InitFloat();
	m_RefrValue  = b3InitFloat();
	m_HighLight  = b3InitFloat();
	m_Flags      = b3InitInt();
	m_xTimes     = b3InitInt();
	m_yTimes     = b3InitInt();
}

void b3MatMarble::b3Write()
{
	b3StoreColor(m_DiffColor);
	b3StoreColor(m_AmbColor);
	b3StoreColor(m_SpecColor);
	b3StoreVector(&m_Scale);
	b3StoreFloat(m_Reflection);
	b3StoreFloat(m_Refraction);
	b3StoreFloat(m_RefrValue);
	b3StoreFloat(m_HighLight);
	b3StoreInt  (m_Flags);
	b3StoreCount(m_xTimes);
	b3StoreCount(m_yTimes);
}

b3_bool b3MatMarble::b3GetColors(b3_ray *ray,b3_surface *surface)
{
	b3Color   mask;
	b3_vector d;

	d.x = ray->polar.m_BoxPolar.x * m_Scale.x * M_PI;
	d.y = ray->polar.m_BoxPolar.y * m_Scale.y * M_PI;
	d.z = ray->polar.m_BoxPolar.z * m_Scale.z * M_PI;

	b3Noise::b3Marble(&d,mask);

	surface->m_Diffuse  = m_DiffColor * mask;
	surface->m_Ambient  = m_AmbColor  * mask;
	surface->m_Specular = m_SpecColor * mask;

	surface->m_Reflection  = m_Reflection;
	surface->m_Refraction  = m_Refraction;
	surface->m_Ior         = m_RefrValue;
	surface->m_SpecularExp = m_HighLight;

	return true;
}

/*************************************************************************
**                                                                      **
**                        Wooden material                               **
**                                                                      **
*************************************************************************/

b3MatWood::b3MatWood(b3_u32 class_type) : b3Material(sizeof(b3MatWood),class_type) 
{
	b3Init();
}

b3MatWood::b3MatWood(b3_u32 *src) : b3Material(src)
{
	b3Init();
	b3InitColor(m_DiffColor);
	b3InitColor(m_AmbColor);
	b3InitColor(m_SpecColor);
	b3InitVector(&m_Scale);
	m_Reflection = b3InitFloat();
	m_Refraction = b3InitFloat();
	m_RefrValue  = b3InitFloat();
	m_HighLight  = b3InitFloat();
	m_Flags      = b3InitInt();
	m_xTimes     = b3InitInt();
	m_yTimes     = b3InitInt();

	// Load extensions if available
	if (B3_PARSE_INDEX_VALID)
	{
		m_yRot                   = b3InitFloat();
		m_zRot                   = b3InitFloat();
		m_RingSpacing            = b3InitFloat();
		m_RingFrequency          = b3InitFloat();
		m_RingNoise              = b3InitFloat();
		m_RingNoiseFrequency     = b3InitFloat();
		m_TrunkWobble            = b3InitFloat();
		m_TrunkWobbleFrequency   = b3InitFloat();
		m_AngularWobble          = b3InitFloat();
		m_AngularWobbleFrequency = b3InitFloat();
		m_GrainFrequency         = b3InitFloat();
		m_Grainy                 = b3InitFloat();
		m_Ringy                  = b3InitFloat();
		b3InitColor(m_LightWood);
		b3InitColor(m_DarkWood);
	}
}

void b3MatWood::b3Init()
{
	// Basic parameters
	m_AmbColor.b3Init(0.2,0.2,0.2);
	m_DiffColor.b3Init(0.1,0.2,0.9);
	m_SpecColor.b3Init(0.8,0.8,0.8);
	m_LightWood.b3Init(0.5,0.2,0.067);
//	m_DarkWood.b3Init(0.15,0.077,0.028);
	m_DarkWood = m_LightWood * 0.7;
	m_Reflection =   0;
	m_Refraction =   0;
	m_RefrValue  =   1;
	m_HighLight  = 200;
	m_Flags      =   0;
	m_xTimes     =   0;
	m_yTimes     =   0;
	b3InitWood();
}

b3_bool b3MatWood::b3Prepare()
{
	b3PrepareWood();
	return true;
}

void b3MatWood::b3Write()
{
	b3StoreColor(m_DiffColor);
	b3StoreColor(m_AmbColor);
	b3StoreColor(m_SpecColor);
	b3StoreVector(&m_Scale);
	b3StoreFloat(m_Reflection);
	b3StoreFloat(m_Refraction);
	b3StoreFloat(m_RefrValue);
	b3StoreFloat(m_HighLight);
	b3StoreInt  (m_Flags);
	b3StoreCount(m_xTimes);
	b3StoreCount(m_yTimes);
	
	// Store extensions
	b3StoreFloat(m_yRot);
	b3StoreFloat(m_zRot);
	b3StoreFloat(m_RingSpacing);
	b3StoreFloat(m_RingFrequency);
	b3StoreFloat(m_RingNoise);
	b3StoreFloat(m_RingNoiseFrequency);
	b3StoreFloat(m_TrunkWobble);
	b3StoreFloat(m_TrunkWobbleFrequency);
	b3StoreFloat(m_AngularWobble);
	b3StoreFloat(m_AngularWobbleFrequency);
	b3StoreFloat(m_GrainFrequency);
	b3StoreFloat(m_Grainy);
	b3StoreFloat(m_Ringy);
	b3StoreColor(m_LightWood);
	b3StoreColor(m_DarkWood);
}

b3_bool b3MatWood::b3GetColors(b3_ray *ray,b3_surface *surface)
{
	b3_vector point;
	b3_f64    mix;

//	b3Vector::b3Init(&point,&ray->ipoint);
	b3Vector::b3Init(&point,&ray->polar.m_BoxPolar);
	
	mix = b3ComputeWood(&point);

	surface->m_Diffuse  = b3Color::b3Mix(m_LightWood,m_DarkWood,mix);
	surface->m_Ambient  = m_AmbColor;
	surface->m_Specular = m_SpecColor;

	surface->m_Reflection  = m_Reflection;
	surface->m_Refraction  = m_Refraction;
	surface->m_Ior         = m_RefrValue;
	surface->m_SpecularExp = m_HighLight;

	return true;
}

/*************************************************************************
**                                                                      **
**                        Oak planks                                    **
**                                                                      **
*************************************************************************/

b3MatOakPlank::b3MatOakPlank(b3_u32 class_type) :
	b3Material(sizeof(b3MatOakPlank),class_type),
	b3OakPlank()
{
	b3Init();
	m_DarkColors  = null;
	m_LightColors = null;
}

b3MatOakPlank::b3MatOakPlank(b3_u32 *src) : b3Material(src), b3OakPlank()
{
	b3Init();
	b3InitColor(m_DiffColor);
	b3InitColor(m_AmbColor);
	b3InitColor(m_SpecColor);

	b3InitVector(&m_Scale);
	m_Reflection = b3InitFloat();
	m_Refraction = b3InitFloat();
	m_RefrValue  = b3InitFloat();
	m_HighLight  = b3InitFloat();
	m_Flags      = b3InitInt();
	m_xTimes     = b3InitInt();
	m_yTimes     = b3InitInt();
	m_xOffset    = b3InitFloat();
	m_xScale     = b3InitFloat();
	m_yScale     = b3InitFloat();
	m_Wobble     = b3InitFloat();

	// Wood basic definition
	b3InitColor(m_LightWood);
	b3InitColor(m_DarkWood);
	m_yRot                   = b3InitFloat();
	m_zRot                   = b3InitFloat();
	m_RingSpacing            = b3InitFloat();
	m_RingFrequency          = b3InitFloat();
	m_RingNoise              = b3InitFloat();
	m_RingNoiseFrequency     = b3InitFloat();
	m_TrunkWobble            = b3InitFloat();
	m_TrunkWobbleFrequency   = b3InitFloat();
	m_AngularWobble          = b3InitFloat();
	m_AngularWobbleFrequency = b3InitFloat();
	m_GrainFrequency         = b3InitFloat();
	m_Grainy                 = b3InitFloat();
	m_Ringy                  = b3InitFloat();

	m_DarkColors  = null;
	m_LightColors = null;
}

b3MatOakPlank::~b3MatOakPlank()
{
	if (m_DarkColors != null)
	{
		delete [] m_DarkColors;
	}

	if (m_LightColors != null)
	{
		delete [] m_LightColors;
	}
}

void b3MatOakPlank::b3Init()
{
	// Basic parameters
	m_AmbColor.b3Init(0.2,0.2,0.2);
	m_DiffColor.b3Init(0.1,0.2,0.9);
	m_SpecColor.b3Init(0.8,0.8,0.8);
	m_LightWood.b3Init(0.5,0.2,0.067);
//	m_DarkWood.b3Init(0.15,0.077,0.028);
	m_DarkWood = m_LightWood * 0.7;
	m_Reflection =   0;
	m_Refraction =   0;
	m_RefrValue  =   1;
	m_HighLight  = 200;
	m_Flags      =   0;
	b3InitOakPlank();
}

b3_bool b3MatOakPlank::b3Prepare()
{
	b3_index x,y,index = 0;
	b3_f64   fx,fy;

	b3PrepareOakPlank();
	
	if (m_DarkColors != null)
	{
		delete [] m_DarkColors;
	}

	if (m_LightColors != null)
	{
		delete [] m_LightColors;
	}

	m_DarkColors  = new b3Color[m_PlankCount];
	m_LightColors = new b3Color[m_PlankCount];

	for (y = 0;y < m_yTimes;y++)
	{
		fy = (b3_f64)y / m_yTimes;
		for (x = 0;x < m_xTimes;x++)
		{
			fx = (b3_f64)x / m_xTimes;

			m_DarkColors[index] = m_DarkWood + b3Color(
				b3Noise::b3SignedFilteredNoiseVector(fx,fy,0),
				b3Noise::b3SignedFilteredNoiseVector(0,fx,fy),
				b3Noise::b3SignedFilteredNoiseVector(fy,0,fx)) * m_DarkWood * m_Wobble * 0.5;

			m_LightColors[index] = m_LightWood + b3Color(
				b3Noise::b3SignedFilteredNoiseVector(fx,fy,0),
				b3Noise::b3SignedFilteredNoiseVector(0,fx,fy),
				b3Noise::b3SignedFilteredNoiseVector(fy,0,fx)) * m_LightWood * m_Wobble * 0.5;
			index++;
		}
	}
	
	return true;
}

void b3MatOakPlank::b3Write()
{
	b3StoreColor(m_DiffColor);
	b3StoreColor(m_AmbColor);
	b3StoreColor(m_SpecColor);
	b3StoreVector(&m_Scale);
	b3StoreFloat(m_Reflection);
	b3StoreFloat(m_Refraction);
	b3StoreFloat(m_RefrValue);
	b3StoreFloat(m_HighLight);
	b3StoreInt  (m_Flags);
	b3StoreCount(m_xTimes);
	b3StoreCount(m_yTimes);
	b3StoreFloat(m_xOffset);
	b3StoreFloat(m_xScale);
	b3StoreFloat(m_yScale);
	b3StoreFloat(m_Wobble);
	
	// Store wood base definitions
	b3StoreColor(m_LightWood);
	b3StoreColor(m_DarkWood);
	b3StoreFloat(m_yRot);
	b3StoreFloat(m_zRot);
	b3StoreFloat(m_RingSpacing);
	b3StoreFloat(m_RingFrequency);
	b3StoreFloat(m_RingNoise);
	b3StoreFloat(m_RingNoiseFrequency);
	b3StoreFloat(m_TrunkWobble);
	b3StoreFloat(m_TrunkWobbleFrequency);
	b3StoreFloat(m_AngularWobble);
	b3StoreFloat(m_AngularWobbleFrequency);
	b3StoreFloat(m_GrainFrequency);
	b3StoreFloat(m_Grainy);
	b3StoreFloat(m_Ringy);
}

b3_bool b3MatOakPlank::b3GetColors(b3_ray *ray,b3_surface *surface)
{
	b3_index index;
	b3_f64   mix = b3ComputeOakPlank(&ray->polar.m_ObjectPolar,index);

	surface->m_Diffuse  = b3Color::b3Mix(m_LightColors[index],m_DarkColors[index],mix);
	surface->m_Ambient  = m_AmbColor;
	surface->m_Specular = m_SpecColor;

	surface->m_Reflection  = m_Reflection;
	surface->m_Refraction  = m_Refraction;
	surface->m_Ior         = m_RefrValue;
	surface->m_SpecularExp = m_HighLight;

	return true;
}

/*************************************************************************
**                                                                      **
**                        Cook and Torrance reflection model            **
**                                                                      **
*************************************************************************/

b3MatCookTorrance::b3MatCookTorrance(b3_u32 class_type) :
	b3MatNormal(sizeof(b3MatCookTorrance),class_type)
{
	m_DiffColor = b3Color(0.79,0.54,0.2);
//	m_DiffColor = b3Color(0.7,0.32,0.2);
	m_SpecColor = b3Color(0.8,0.8,0.8);
	m_AmbColor  = m_DiffColor * 0.2;
	m_ka   = 0.1;
	m_ks   = 0.6;
	m_kd   = 0.6;
	m_m    = 0.3;
}

b3MatCookTorrance::b3MatCookTorrance(b3_u32 *src) : b3MatNormal(src)
{
	m_ka = b3InitFloat();
	m_kd = b3InitFloat();
	m_ks = b3InitFloat();
	m_m  = b3InitFloat();
}

void b3MatCookTorrance::b3Write()
{
	b3MatNormal::b3Write();

	b3StoreFloat(m_ka);
	b3StoreFloat(m_kd);
	b3StoreFloat(m_ks);
	b3StoreFloat(m_m);
}

b3_bool b3MatCookTorrance::b3Prepare()
{
	m_Ra   = m_AmbColor * m_ka;
	m_Mu   = b3Color(
		b3Math::b3GetMu(m_DiffColor[b3Color::R]),
		b3Math::b3GetMu(m_DiffColor[b3Color::G]),
		b3Math::b3GetMu(m_DiffColor[b3Color::B]));

	return true;
}

b3_bool b3MatCookTorrance::b3Illuminate(b3_ray_fork *ray,b3_light_info *jit,b3Color &acc)
{
	b3Color     result;
	b3_vector64 L;

	B3_ASSERT(ray->incoming != null);	

	b3Vector::b3Init(&L,&jit->dir);
	b3Vector::b3Normalize(&L);

	b3_f64 nl = b3Vector::b3SMul(&ray->incoming->normal,&L);

#if 1
	b3Color Rf;
	if (jit->shape == null)
	{
		b3_vector64 H;

		H.x = L.x - ray->incoming->dir.x;
		H.y = L.y - ray->incoming->dir.y;
		H.z = L.z - ray->incoming->dir.z;
		b3Vector::b3Normalize(&H);

		b3_f64 nh =  b3Vector::b3SMul(&ray->incoming->normal,&H);
		b3_f64 nv = -b3Vector::b3SMul(&ray->incoming->normal,&ray->incoming->dir);
		b3_f64 vh = -b3Vector::b3SMul(&ray->incoming->dir,&H);

		b3_f64 Gm = 2 * nh * nv / vh;
		b3_f64 Gs = 2 * nh * nl / vh;

		b3_f64 G = 1;
		if (Gm < G)
		{
			G = Gm;
		}
		if (Gs < G)
		{
			G = Gs;
		}
		G = b3Math::b3Limit(G,0,1);

		b3_f64 alpha = acos(nh);
		b3_f64 nh_q  = nh * nh;
		b3_f64 D     = exp(-b3Math::b3Sqr(tan(alpha) / m_m)) / (m_m * m_m * nh_q * nh_q);
		b3_f64 Rs    = (D * G) / (M_PI * nv * nl);

		b3_f64 phi = asin(nl);
		for (int i = 0;i < 4;i++)
		{
			b3Color::b3_color_index l = (b3Color::b3_color_index)i;

			Rf[l] = b3Math::b3GetFresnel(phi,m_Mu[l]) * Rs;
		}
		Rf.b3Min();
	}
	else
	{
		Rf = 0;
	}
	
	result = m_Ra + m_DiffColor * nl * m_kd + Rf * m_ks;
#else
	b3_f64 rl = b3Vector::b3SMul(&ray->refl_ray.dir,&L);

	result = m_Ra + m_DiffColor * nl + m_SpecColor * pow(fabs(rl),m_HighLight);
#endif

	acc += result;

	return true;
}

/*************************************************************************
**                                                                      **
**                        Granite material                              **
**                                                                      **
*************************************************************************/

b3MatGranite::b3MatGranite(b3_u32 class_type) : b3Material(sizeof(b3MatGranite),class_type) 
{
	m_DarkColor  = b3Color(B3_BLACK);
	m_LightColor = b3Color(b3_pkd_color(0xd0dde0));
	m_DiffColor  = b3Color(0.8, 0.8, 0.8);
	m_AmbColor   = b3Color(0.1, 0.1, 0.1);
	m_DiffColor  = b3Color(0.8, 0.8, 0.8);
	m_Reflection =   0.0;
	m_Refraction =   0.0;
	m_RefrValue  =   1.0;
	m_HighLight  = 100.0;
	m_Overtone   =   2;
	m_Flags      =   0;
	b3Vector::b3Init(&m_Scale,10,10,10);
}

b3MatGranite::b3MatGranite(b3_u32 *src) : b3Material(src)
{
	b3InitColor(m_DarkColor);
	b3InitColor(m_LightColor);
	b3InitColor(m_DiffColor);
	b3InitColor(m_AmbColor);
	b3InitColor(m_SpecColor);
	b3InitVector(&m_Scale);
	m_Reflection = b3InitFloat();
	m_Refraction = b3InitFloat();
	m_RefrValue  = b3InitFloat();
	m_HighLight  = b3InitFloat();
	m_Flags      = b3InitInt();
	m_Overtone   = b3InitCount();
}

void b3MatGranite::b3Write()
{
	b3StoreColor(m_DarkColor);
	b3StoreColor(m_LightColor);
	b3StoreColor(m_DiffColor);
	b3StoreColor(m_AmbColor);
	b3StoreColor(m_SpecColor);
	b3StoreVector(&m_Scale);
	b3StoreFloat(m_Reflection);
	b3StoreFloat(m_Refraction);
	b3StoreFloat(m_RefrValue);
	b3StoreFloat(m_HighLight);
	b3StoreInt  (m_Flags);
	b3StoreCount(m_Overtone);
}

b3_bool b3MatGranite::b3GetColors(b3_ray *ray,b3_surface *surface)
{
	b3Color   mask;
	b3_vector d;
	b3_loop   i;
	b3_f64    sum = 0;
	b3_f64    freq = 1.0;

	d.x = ((ray->polar.m_BoxPolar.x - 0.5) * m_Scale.x * M_PI);
	d.y = ((ray->polar.m_BoxPolar.y - 0.5) * m_Scale.y * M_PI);
	d.z = ((ray->polar.m_BoxPolar.z - 0.5) * m_Scale.z * M_PI);

	for (i = 0;i < m_Overtone;i++)
	{
		sum += b3Noise::b3FilteredNoiseVector(
			4 * freq * d.x,
			4 * freq * d.y,
			4 * freq * d.z) / freq;
		freq += freq; // = freq *= 2;
	}

	if (sum < 0)
	{
		mask = m_DarkColor;
	}
	else if (sum > 1)
	{
		mask = m_LightColor;
	}
	else
	{
		mask = b3Color::b3Mix(m_DarkColor,m_LightColor,sum);
	}
	
	surface->m_Diffuse  = m_DiffColor * mask;
	surface->m_Ambient  = m_AmbColor  * mask;
	surface->m_Specular = m_SpecColor * mask;

	surface->m_Reflection  = m_Reflection;
	surface->m_Refraction  = m_Refraction;
	surface->m_Ior         = m_RefrValue;
	surface->m_SpecularExp = m_HighLight;

	return true;
}
