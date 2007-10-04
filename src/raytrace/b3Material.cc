/*
**
**	$Filename:	b3Material.cc $
**	$Release:	Dortmund 2001, 2002, 2003, 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing materials
**
**	(C) Copyright 2001, 2002, 2003, 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3RaytraceInclude.h"
#include "blz3/base/b3Procedure.h"

#define no_DEBUG_MATERIAL

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
	b3Item::b3Register(&b3MatCarPaint::b3StaticInit,     &b3MatCarPaint::b3StaticInit,     CAR_PAINT);
	b3Item::b3Register(&b3MatThinFilm::b3StaticInit,     &b3MatThinFilm::b3StaticInit,     THIN_FILM);
#ifndef DEBUG_MATERIAL
	b3Item::b3Register(&b3MatGranite::b3StaticInit,      &b3MatGranite::b3StaticInit,      GRANITE);
#else
	b3Item::b3Register(&b3MatGranite::b3StaticInit,      &b3MatGranite::b3StaticInit,      MAT_NORMAL);
#endif
}

/*************************************************************************
**                                                                      **
**                        Base class for all materials                  **
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

/*************************************************************************
**                                                                      **
**                        Standard material                             **
**                                                                      **
*************************************************************************/

b3MatNormal::b3MatNormal(b3_size class_size,b3_u32 class_type) : b3Material(class_size,class_type)
{
	b3Init();
}

b3MatNormal::b3MatNormal(b3_u32 class_type) : b3Material(sizeof(b3MatNormal),class_type)
{
	b3Init();
}

b3MatNormal::b3MatNormal(b3_u32 *src) : b3Material(src)
{
	b3InitColor(m_Diffuse);
	b3InitColor(m_Ambient);
	b3InitColor(m_Specular);
	m_Reflection  = b3InitFloat();
	m_Refraction  = b3InitFloat();
	m_Ior         = b3InitFloat();
	m_SpecularExp = b3InitFloat();
	m_Flags       = b3InitInt();
}

void b3MatNormal::b3Write()
{
	b3StoreColor(m_Diffuse);
	b3StoreColor(m_Ambient);
	b3StoreColor(m_Specular);
	b3StoreFloat(m_Reflection);
	b3StoreFloat(m_Refraction);
	b3StoreFloat(m_Ior);
	b3StoreFloat(m_SpecularExp);
	b3StoreInt  (m_Flags);
}

void b3MatNormal::b3Init()
{
	m_Diffuse     = B3_BLUE;
	m_Ambient     = m_Diffuse * 0.2;
	m_Specular    = B3_GREY;
	m_Reflection  =    0;
	m_Refraction  =    0;
	m_Ior         =    1.5;
	m_SpecularExp = 1000;
	m_Flags       =    0;
}

b3_bool b3MatNormal::b3GetSurfaceValues(b3_surface *surface)
{
	b3_material *dst = surface;

	*dst = *this;

	return true;
}

/*************************************************************************
**                                                                      **
**                        Chess material                                **
**                                                                      **
*************************************************************************/

b3MatChess::b3MatChess(b3_u32 class_type) : b3Material(sizeof(b3MatChess),class_type)
{
	m_Material[BLACK].m_Ambient  = B3_BLACK;
	m_Material[BLACK].m_Diffuse  = B3_BLACK;
	m_Material[BLACK].m_Specular = B3_GREY;

	m_Material[WHITE].m_Ambient  = B3_GREY;
	m_Material[WHITE].m_Diffuse  = B3_WHITE;
	m_Material[WHITE].m_Specular = B3_GREY;

	m_Material[BLACK].m_Reflection  = m_Material[WHITE].m_Reflection  =    0;
	m_Material[BLACK].m_Refraction  = m_Material[WHITE].m_Refraction  =    0;
	m_Material[BLACK].m_Ior         = m_Material[WHITE].m_Ior         =    1.5;
	m_Material[BLACK].m_SpecularExp = m_Material[WHITE].m_SpecularExp = 1000;

	m_Flags  = 0;
	m_xTimes = 8;
	m_yTimes = 8;
}

b3MatChess::b3MatChess(b3_u32 *src) : b3Material(src)
{
	b3InitColor(m_Material[BLACK].m_Diffuse);
	b3InitColor(m_Material[BLACK].m_Ambient);
	b3InitColor(m_Material[BLACK].m_Specular);
	b3InitColor(m_Material[WHITE].m_Diffuse);
	b3InitColor(m_Material[WHITE].m_Ambient);
	b3InitColor(m_Material[WHITE].m_Specular);
	m_Material[BLACK].m_Reflection  = b3InitFloat();
	m_Material[WHITE].m_Reflection  = b3InitFloat();
	m_Material[BLACK].m_Refraction  = b3InitFloat();
	m_Material[WHITE].m_Refraction  = b3InitFloat();
	m_Material[BLACK].m_Ior         = b3InitFloat();
	m_Material[WHITE].m_Ior         = b3InitFloat();
	m_Material[BLACK].m_SpecularExp = b3InitFloat();
	m_Material[WHITE].m_SpecularExp = b3InitFloat();
	m_Flags         = b3InitInt();
	m_xTimes        = b3InitInt();
	m_yTimes        = b3InitInt();
}

void b3MatChess::b3Write()
{
	b3StoreColor(m_Material[BLACK].m_Diffuse);
	b3StoreColor(m_Material[BLACK].m_Ambient);
	b3StoreColor(m_Material[BLACK].m_Specular);
	b3StoreColor(m_Material[WHITE].m_Diffuse);
	b3StoreColor(m_Material[WHITE].m_Ambient);
	b3StoreColor(m_Material[WHITE].m_Specular);
	b3StoreFloat(m_Material[BLACK].m_Reflection);
	b3StoreFloat(m_Material[WHITE].m_Reflection);
	b3StoreFloat(m_Material[BLACK].m_Refraction);
	b3StoreFloat(m_Material[WHITE].m_Refraction);
	b3StoreFloat(m_Material[BLACK].m_Ior);
	b3StoreFloat(m_Material[WHITE].m_Ior);
	b3StoreFloat(m_Material[BLACK].m_SpecularExp);
	b3StoreFloat(m_Material[WHITE].m_SpecularExp);
	b3StoreInt  (m_Flags);
	b3StoreCount(m_xTimes);
	b3StoreCount(m_yTimes);
}

#define CHESS_INDEX(x,y) (((b3_index)(((x) + 1) * m_xTimes) + (b3_index)(((y) + 1) * m_yTimes) + 1) & 1)

b3_bool b3MatChess::b3GetSurfaceValues(b3_surface *surface)
{
	b3_material *dst = surface;
	b3_index     index = CHESS_INDEX(surface->m_Incoming->polar.m_Polar.x,surface->m_Incoming->polar.m_Polar.y);

	*dst = m_Material[index];

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
	m_Reflection  =    0;
	m_Refraction  =    0;
	m_Ior         =    1.5;
	m_SpecularExp = 1000;

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
	m_Reflection  = b3InitFloat();
	m_Refraction  = b3InitFloat();
	m_Ior         = b3InitFloat();
	m_SpecularExp = b3InitFloat();
	m_xStart      = b3InitFloat();
	m_yStart      = b3InitFloat();
	m_xScale      = b3InitFloat();
	m_yScale      = b3InitFloat();
	m_xTimes      = b3InitInt();
	m_yTimes      = b3InitInt();
	m_Texture     = (b3Tx *)b3InitNull();
	m_Flags       = b3InitInt();
	b3InitString(m_Name,B3_TEXSTRINGLEN);
}

void b3MatTexture::b3Write()
{
	b3StoreFloat(m_Reflection);
	b3StoreFloat(m_Refraction);
	b3StoreFloat(m_Ior);
	b3StoreFloat(m_SpecularExp);
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

b3_bool b3MatTexture::b3Prepare(b3_preparation_info *prep_info)
{
	return b3Scene::b3CheckTexture(&m_Texture,m_Name);
}

void b3MatTexture::b3SetTexture(const char *name)
{
	m_Name.b3Format("%s",name);
}

b3_bool b3MatTexture::b3GetSurfaceValues(b3_surface *surface)
{
	b3_coord     x,y;
	b3_f64       fx,fy;

	fx = (surface->m_Incoming->polar.m_Polar.x - m_xStart) / m_xScale;
	if (m_Flags & MAT_XINVERT) fx = 1.0 - fx;
	if ((fx < 0) || (fx >= m_xTimes))
	{
		return false;
	}

	fy = (surface->m_Incoming->polar.m_Polar.y - m_yStart) / m_yScale;
	if (m_Flags & MAT_YINVERT) fy = 1.0 - fy;
	if ((fy < 0) || (fy >= m_yTimes))
	{
		return false;
	}
	x = (b3_coord)((fx - (b3_coord)fx) * m_Texture->xSize);
	y = (b3_coord)((fy - (b3_coord)fy) * m_Texture->ySize);

	surface->m_Diffuse = m_Texture->b3GetHdrValue(x, y);
	surface->m_Ambient = surface->m_Diffuse * 0.3;
	surface->m_Specular.b3Init(0.7f,0.7f,0.7f);

	surface->m_Reflection  = m_Reflection;
	surface->m_Refraction  = m_Refraction;
	surface->m_Ior         = m_Ior;
	surface->m_SpecularExp = m_SpecularExp;

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
	// Material values
	m_Reflection  =    0;
	m_Refraction  =    0;
	m_Ior         =    1.5;
	m_SpecularExp = 1000.0;

	// Mapping boundary values
	m_xStart = 0;
	m_xEnd   = 1;
	m_yStart = 0;
	m_yEnd   = 1;

	// Init texture
	m_Name.b3Empty();
	m_Texture = null;
	m_Flags   = 0;
}

b3MatWrapTexture::b3MatWrapTexture(b3_u32 *src) : b3Material(src)
{
	m_Reflection  = b3InitFloat();
	m_Refraction  = b3InitFloat();
	m_Ior         = b3InitFloat();
	m_SpecularExp = b3InitFloat();
	m_xStart      = b3InitFloat();
	m_yStart      = b3InitFloat();
	m_xEnd        = b3InitFloat();
	m_yEnd        = b3InitFloat();
	m_Texture     = (b3Tx *)b3InitNull();
	m_Flags       = b3InitInt();
	b3InitString(m_Name,B3_TEXSTRINGLEN);
}

void b3MatWrapTexture::b3Write()
{
	b3StoreFloat(m_Reflection);
	b3StoreFloat(m_Refraction);
	b3StoreFloat(m_Ior);
	b3StoreFloat(m_SpecularExp);
	b3StoreFloat(m_xStart);
	b3StoreFloat(m_yStart);
	b3StoreFloat(m_xEnd);
	b3StoreFloat(m_yEnd);
	b3StoreNull();
	b3StoreInt(m_Flags);
	b3StoreString(m_Name,B3_TEXSTRINGLEN);
}

b3_bool b3MatWrapTexture::b3Prepare(b3_preparation_info *prep_info)
{
	return b3Scene::b3CheckTexture(&m_Texture,m_Name);
}

void b3MatWrapTexture::b3SetTexture(const char *name)
{
	m_Name.b3Format("%s",name);
}

b3_bool b3MatWrapTexture::b3GetSurfaceValues(b3_surface *surface)
{
	b3_coord     x,y;
	b3_f64       fx,fy,xEnd,xPolar;

	if ((surface->m_Incoming->polar.m_Polar.y >= m_yStart) && (surface->m_Incoming->polar.m_Polar.y <= m_yEnd))
	{
		xEnd	= m_xEnd;
		xPolar	= surface->m_Incoming->polar.m_Polar.x;
		fy = (surface->m_Incoming->polar.m_Polar.y - m_yStart) /
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

	surface->m_Diffuse = m_Texture->b3GetHdrValue(x,y);
	surface->m_Ambient = surface->m_Diffuse * 0.3;
	surface->m_Specular.b3Init(0.7f,0.7f,0.7f);

	surface->m_Reflection  = m_Reflection;
	surface->m_Refraction  = m_Refraction;
	surface->m_Ior         = m_Ior;
	surface->m_SpecularExp = m_SpecularExp;

	return true;
}

/*************************************************************************
**                                                                      **
**                        Color sliding material                        **
**                                                                      **
*************************************************************************/

b3MatSlide::b3MatSlide(b3_u32 class_type) : b3Material(sizeof(b3MatSlide),class_type)
{
	m_Material[0].m_Diffuse  = B3_RED;
	m_Material[0].m_Ambient  = m_Material[0].m_Diffuse * 0.2;
	m_Material[0].m_Specular = B3_GREY;

	m_Material[1].m_Diffuse  = B3_GREEN;
	m_Material[1].m_Ambient  = m_Material[1].m_Diffuse * 0.2;
	m_Material[1].m_Specular = B3_GREY;

	m_Material[0].m_Reflection  = m_Material[1].m_Reflection  =    0;
	m_Material[0].m_Refraction  = m_Material[1].m_Refraction  =    0;
	m_Material[0].m_Ior         = m_Material[1].m_Ior         =    1.5;
	m_Material[0].m_SpecularExp = m_Material[1].m_SpecularExp = 1000;

	m_From = 0;
	m_To   = 1;
	m_ModeFlag = 0;
}

b3MatSlide::b3MatSlide(b3_u32 *src) : b3Material(src)
{
	b3InitColor(m_Material[0].m_Diffuse);
	b3InitColor(m_Material[0].m_Ambient);
	b3InitColor(m_Material[0].m_Specular);
	b3InitColor(m_Material[1].m_Diffuse);
	b3InitColor(m_Material[1].m_Ambient);
	b3InitColor(m_Material[1].m_Specular);
	m_From        = b3InitFloat();
	m_To          = b3InitFloat();
	m_Material[0].m_Reflection  = b3InitFloat();
	m_Material[0].m_Refraction  = b3InitFloat();
	m_Material[0].m_Ior         = b3InitFloat();
	m_Material[0].m_SpecularExp = b3InitFloat();
	m_ModeFlag    = b3InitInt();

	if (B3_PARSE_INDEX_VALID)
	{
		m_Material[1].m_Reflection  = b3InitFloat();
		m_Material[1].m_Refraction  = b3InitFloat();
		m_Material[1].m_Ior         = b3InitFloat();
		m_Material[1].m_SpecularExp = b3InitFloat();
	}
	else
	{
		m_Material[1].m_Reflection  = m_Material[0].m_Reflection;
		m_Material[1].m_Refraction  = m_Material[0].m_Refraction;
		m_Material[1].m_Ior         = m_Material[0].m_Ior;
		m_Material[1].m_SpecularExp = m_Material[0].m_SpecularExp;
	}
}

void b3MatSlide::b3Write()
{
	b3StoreColor(m_Material[0].m_Diffuse);
	b3StoreColor(m_Material[0].m_Ambient);
	b3StoreColor(m_Material[0].m_Specular);
	b3StoreColor(m_Material[1].m_Diffuse);
	b3StoreColor(m_Material[1].m_Ambient);
	b3StoreColor(m_Material[1].m_Specular);
	b3StoreFloat(m_From);
	b3StoreFloat(m_To);
	b3StoreFloat(m_Material[0].m_Reflection);
	b3StoreFloat(m_Material[0].m_Refraction);
	b3StoreFloat(m_Material[0].m_Ior);
	b3StoreFloat(m_Material[0].m_SpecularExp);
	b3StoreInt(m_ModeFlag);
	b3StoreFloat(m_Material[1].m_Reflection);
	b3StoreFloat(m_Material[1].m_Refraction);
	b3StoreFloat(m_Material[1].m_Ior);
	b3StoreFloat(m_Material[1].m_SpecularExp);
}

b3_bool b3MatSlide::b3GetSurfaceValues(b3_surface *surface)
{
	b3_f64 Factor;

	switch (m_ModeFlag)
	{
	case XSLIDE :
		Factor = (surface->m_Incoming->polar.m_Polar.x - m_From) / (m_To - m_From);
		if (Factor < 0) Factor = 0;
		if (Factor > 1) Factor = 1;
		break;
	case YSLIDE :
		Factor = (surface->m_Incoming->polar.m_Polar.y - m_From) / (m_To - m_From);
		if (Factor < 0) Factor = 0;
		if (Factor > 1) Factor = 1;
		break;
	case XSLIDE_CUT :
		Factor = (surface->m_Incoming->polar.m_Polar.x - m_From) / (m_To - m_From);
		if ((Factor < 0) || (Factor > 1))
		{
			return false;
		}
		break;
	case YSLIDE_CUT :
		Factor = (surface->m_Incoming->polar.m_Polar.y - m_From) / (m_To - m_From);
		if ((Factor < 0) || (Factor > 1))
		{
			return false;
		}
		break;

	default:
		Factor = 0.5;
		break;
	}

	b3Mix(surface,&m_Material[0],&m_Material[1], Factor);

	return true;
}

/*************************************************************************
**                                                                      **
**                        Marble material                               **
**                                                                      **
*************************************************************************/

b3MatMarble::b3MatMarble(b3_u32 class_type) : b3Material(sizeof(b3MatMarble),class_type)
{
	m_LightMaterial.m_Diffuse     = B3_LIGHT_GREY;
	m_LightMaterial.m_Ambient     = m_LightMaterial.m_Diffuse * 0.2;
	m_LightMaterial.m_Specular    = B3_GREY;
	m_LightMaterial.m_Reflection  =    0;
	m_LightMaterial.m_Refraction  =    0;
	m_LightMaterial.m_Ior         =    1;
	m_LightMaterial.m_SpecularExp = 1000;

	m_DarkMaterial.m_Diffuse     = B3_BLACK;
	m_DarkMaterial.m_Ambient     = B3_BLACK;
	m_DarkMaterial.m_Specular    = B3_GREY;
	m_DarkMaterial.m_Reflection  =    0;
	m_DarkMaterial.m_Refraction  =    0;
	m_DarkMaterial.m_Ior         =    1.5;
	m_DarkMaterial.m_SpecularExp = 1000;

	m_xTimes      =    0;
	m_yTimes      =    0;

	b3InitScaling(0.1f,B3_SCALE_IPOINT_ORIGINAL);
}

b3MatMarble::b3MatMarble(b3_u32 *src) : b3Material(src)
{
	b3InitColor(m_LightMaterial.m_Diffuse);
	b3InitColor(m_LightMaterial.m_Ambient);
	b3InitColor(m_LightMaterial.m_Specular);
	m_Scale.x   = b3InitFloat() * M_PI;
	m_Scale.y   = b3InitFloat() * M_PI;
	m_Scale.z   = b3InitFloat() * M_PI;
	m_LightMaterial.m_Reflection  = b3InitFloat();
	m_LightMaterial.m_Refraction  = b3InitFloat();
	m_LightMaterial.m_Ior         = b3InitFloat();
	m_LightMaterial.m_SpecularExp = b3InitFloat();

	m_ScaleFlags  = (b3_scaling_mode)b3InitInt();
	m_xTimes      = b3InitInt();
	m_yTimes      = b3InitInt();

	if (B3_PARSE_INDEX_VALID)
	{
		b3InitColor(m_DarkMaterial.m_Diffuse);
		b3InitColor(m_DarkMaterial.m_Ambient);
		b3InitColor(m_DarkMaterial.m_Specular);
		m_DarkMaterial.m_Reflection  = b3InitFloat();
		m_DarkMaterial.m_Refraction  = b3InitFloat();
		m_DarkMaterial.m_Ior         = b3InitFloat();
		m_DarkMaterial.m_SpecularExp = b3InitFloat();
	}
	else
	{
		m_DarkMaterial           = m_LightMaterial;
		m_DarkMaterial.m_Diffuse =
			m_DarkMaterial.m_Ambient = B3_BLACK;
	}
}

void b3MatMarble::b3Write()
{
	b3StoreColor(m_LightMaterial.m_Diffuse);
	b3StoreColor(m_LightMaterial.m_Ambient);
	b3StoreColor(m_LightMaterial.m_Specular);
	b3StoreFloat(m_Scale.x / M_PI);
	b3StoreFloat(m_Scale.y / M_PI);
	b3StoreFloat(m_Scale.z / M_PI);
	b3StoreFloat(m_LightMaterial.m_Reflection);
	b3StoreFloat(m_LightMaterial.m_Refraction);
	b3StoreFloat(m_LightMaterial.m_Ior);
	b3StoreFloat(m_LightMaterial.m_SpecularExp);
	b3StoreInt  (m_ScaleFlags);
	b3StoreCount(m_xTimes);
	b3StoreCount(m_yTimes);

	b3StoreColor(m_DarkMaterial.m_Diffuse);
	b3StoreColor(m_DarkMaterial.m_Ambient);
	b3StoreColor(m_DarkMaterial.m_Specular);
	b3StoreFloat(m_DarkMaterial.m_Reflection);
	b3StoreFloat(m_DarkMaterial.m_Refraction);
	b3StoreFloat(m_DarkMaterial.m_Ior);
	b3StoreFloat(m_DarkMaterial.m_SpecularExp);
}

b3_bool b3MatMarble::b3Prepare(b3_preparation_info *prep_info)
{
	b3PrepareScaling();
	return true;
}

b3_bool b3MatMarble::b3GetSurfaceValues(b3_surface *surface)
{
	b3_vector point;

	b3Scale(surface->m_Incoming,&m_Scale,&point);

	b3_f64 mix = b3Noise::b3Marble(&point);

	b3Mix(surface,&m_DarkMaterial,&m_LightMaterial, mix);

	return true;
}

/*************************************************************************
**                                                                      **
**                        Base class for wooden materials               **
**                                                                      **
*************************************************************************/

b3MaterialWooden::b3MaterialWooden(b3_size class_size,b3_u32 class_type) : b3Material(class_size,class_type)
{
	b3InitScaling(1.0,B3_SCALE_IPOINT_ORIGINAL);
}

b3MaterialWooden::b3MaterialWooden(b3_u32 class_type) : b3Material(sizeof(b3MaterialWooden),class_type)
{
	b3InitScaling(1.0,B3_SCALE_IPOINT_ORIGINAL);
}

b3MaterialWooden::b3MaterialWooden(b3_u32 *src) : b3Material(src)
{
}

void b3MaterialWooden::b3Init()
{
	m_LightMaterial.m_Diffuse.b3Init(0.5f,0.2f,0.067f);
	m_LightMaterial.m_Ambient  = m_LightMaterial.m_Diffuse * 0.2;
	m_LightMaterial.m_Specular = B3_LIGHT_GREY;

	m_DarkMaterial.m_Diffuse   = m_LightMaterial.m_Diffuse * 0.7;
	m_DarkMaterial.m_Ambient   = m_DarkMaterial.m_Diffuse * 0.2;
	m_DarkMaterial.m_Specular  = B3_LIGHT_GREY;

	m_DarkMaterial.m_Reflection  =   0;
	m_DarkMaterial.m_Refraction  =   0;
	m_DarkMaterial.m_Ior         =   1.5;
	m_DarkMaterial.m_SpecularExp = 200;

	m_LightMaterial.m_Reflection  =   0;
	m_LightMaterial.m_Refraction  =   0;
	m_LightMaterial.m_Ior         =   1.5;
	m_LightMaterial.m_SpecularExp = 200;
}

/*************************************************************************
**                                                                      **
**                        Wooden material                               **
**                                                                      **
*************************************************************************/

b3MatWood::b3MatWood(b3_u32 class_type) : b3MaterialWooden(sizeof(b3MatWood),class_type)
{
	b3Init();
}

b3MatWood::b3MatWood(b3_u32 *src) : b3MaterialWooden(src)
{
	b3Init();
	b3InitColor(m_DarkMaterial.m_Diffuse);
	b3InitColor(m_DarkMaterial.m_Ambient);
	b3InitColor(m_DarkMaterial.m_Specular);
	b3InitVector(&m_Scale);
	m_DarkMaterial.m_Reflection  = b3InitFloat();
	m_DarkMaterial.m_Refraction  = b3InitFloat();
	m_DarkMaterial.m_Ior         = b3InitFloat();
	m_DarkMaterial.m_SpecularExp = b3InitFloat();
	m_ScaleFlags  = (b3_scaling_mode)b3InitInt();
	m_xTimes      = b3InitInt();
	m_yTimes      = b3InitInt();
	m_LightMaterial = m_DarkMaterial;

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
		b3InitColor(m_LightMaterial.m_Diffuse);
		b3InitColor(m_DarkMaterial.m_Diffuse);
		if (B3_PARSE_INDEX_VALID)
		{
			b3InitColor(m_LightMaterial.m_Ambient);
			b3InitColor(m_DarkMaterial.m_Ambient);
			b3InitColor(m_LightMaterial.m_Specular);
			b3InitColor(m_DarkMaterial.m_Specular);
			m_LightMaterial.m_Reflection  = b3InitFloat();
			m_LightMaterial.m_Refraction  = b3InitFloat();
			m_LightMaterial.m_Ior         = b3InitFloat();
			m_LightMaterial.m_SpecularExp = b3InitFloat();
		}
		else
		{
			m_LightMaterial.m_Ambient  = m_LightMaterial.m_Diffuse * 0.2;
			m_LightMaterial.m_Specular = B3_GREY;
			m_DarkMaterial.m_Ambient   = m_DarkMaterial.m_Diffuse * 0.2;
			m_DarkMaterial.m_Specular  = B3_GREY;

			// light material surface values copied earlier.
		}
	}
}

void b3MatWood::b3Init()
{
	b3MaterialWooden::b3Init();

	// Basic parameters
	m_xTimes =   0; // unused
	m_yTimes =   0; // unused
	b3InitWood();
}

void b3MatWood::b3Write()
{
	b3StoreColor(m_DarkMaterial.m_Diffuse);
	b3StoreColor(m_DarkMaterial.m_Ambient);
	b3StoreColor(m_DarkMaterial.m_Specular);
	b3StoreVector(&m_Scale);
	b3StoreFloat(m_DarkMaterial.m_Reflection);
	b3StoreFloat(m_DarkMaterial.m_Refraction);
	b3StoreFloat(m_DarkMaterial.m_Ior);
	b3StoreFloat(m_DarkMaterial.m_SpecularExp);
	b3StoreInt  (m_ScaleFlags);
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
	b3StoreColor(m_LightMaterial.m_Diffuse);
	b3StoreColor(m_DarkMaterial.m_Diffuse);
	b3StoreColor(m_LightMaterial.m_Ambient);
	b3StoreColor(m_DarkMaterial.m_Ambient);
	b3StoreColor(m_LightMaterial.m_Specular);
	b3StoreColor(m_DarkMaterial.m_Specular);
	b3StoreFloat(m_LightMaterial.m_Reflection);
	b3StoreFloat(m_LightMaterial.m_Refraction);
	b3StoreFloat(m_LightMaterial.m_Ior);
	b3StoreFloat(m_LightMaterial.m_SpecularExp);
}

b3_bool b3MatWood::b3Prepare(b3_preparation_info *prep_info)
{
	b3PrepareWood(&m_Scale);
	b3PrepareScaling();
	return true;
}

b3_bool b3MatWood::b3GetSurfaceValues(b3_surface *surface)
{
	b3_vector point;
	b3_f64    mix;

	b3Scale(surface->m_Incoming,null,&point);
	mix = b3ComputeWood(&point, surface->m_Incoming->Q);

	b3Mix(surface,&m_DarkMaterial,&m_LightMaterial, mix);

	return true;
}

/*************************************************************************
**                                                                      **
**                        Oak planks                                    **
**                                                                      **
*************************************************************************/

b3MatOakPlank::b3MatOakPlank(b3_u32 class_type) :
		b3MaterialWooden(sizeof(b3MatOakPlank),class_type),
		b3OakPlank()
{
	b3Init();
	m_DarkMaterials  = null;
	m_LightMaterials = null;
}

b3MatOakPlank::b3MatOakPlank(b3_u32 *src) : b3MaterialWooden(src), b3OakPlank()
{
	b3Init();

	// Dark
	b3InitColor(m_DarkMaterial.m_Diffuse); // Overwritten later
	b3InitColor(m_DarkMaterial.m_Ambient);
	b3InitColor(m_DarkMaterial.m_Specular);
	b3InitVector(&m_Scale);
	m_DarkMaterial.m_Reflection  = b3InitFloat();
	m_DarkMaterial.m_Refraction  = b3InitFloat();
	m_DarkMaterial.m_Ior         = b3InitFloat();
	m_DarkMaterial.m_SpecularExp = b3InitFloat();

	m_LightMaterial = m_DarkMaterial;

	// Oak plank values
	m_ScaleFlags  = (b3_scaling_mode)b3InitInt();
	m_xTimes      = b3InitInt();
	m_yTimes      = b3InitInt();
	m_xOffset     = b3InitFloat();
	m_xScale      = b3InitFloat();
	m_yScale      = b3InitFloat();
	m_Wobble      = b3InitFloat();

	// Some diffuse colors - real use.
	b3InitColor(m_LightMaterial.m_Diffuse);
	b3InitColor(m_DarkMaterial.m_Diffuse);

	// Wood basic definition
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

	// Light
	if (B3_PARSE_INDEX_VALID)
	{
		b3InitColor(m_LightMaterial.m_Ambient);
		b3InitColor(m_LightMaterial.m_Specular);
		m_LightMaterial.m_Reflection  = b3InitFloat();
		m_LightMaterial.m_Refraction  = b3InitFloat();
		m_LightMaterial.m_Ior         = b3InitFloat();
		m_LightMaterial.m_SpecularExp = b3InitFloat();
	}

	m_DarkMaterials  = null;
	m_LightMaterials = null;
}

b3MatOakPlank::~b3MatOakPlank()
{
	if (m_DarkMaterials != null)
	{
		delete [] m_DarkMaterials;
	}

	if (m_LightMaterials != null)
	{
		delete [] m_LightMaterials;
	}
}

void b3MatOakPlank::b3Init()
{
	b3MaterialWooden::b3Init();

	// Basic parameters
	b3InitOakPlank();
}

void b3MatOakPlank::b3Write()
{
	// Dark
	b3StoreColor(m_DarkMaterial.m_Diffuse);
	b3StoreColor(m_DarkMaterial.m_Ambient);
	b3StoreColor(m_DarkMaterial.m_Specular);
	b3StoreVector(&m_Scale);
	b3StoreFloat(m_DarkMaterial.m_Reflection);
	b3StoreFloat(m_DarkMaterial.m_Refraction);
	b3StoreFloat(m_DarkMaterial.m_Ior);
	b3StoreFloat(m_DarkMaterial.m_SpecularExp);

	// Oak plank values
	b3StoreInt  (m_ScaleFlags);
	b3StoreCount(m_xTimes);
	b3StoreCount(m_yTimes);
	b3StoreFloat(m_xOffset);
	b3StoreFloat(m_xScale);
	b3StoreFloat(m_yScale);
	b3StoreFloat(m_Wobble);

	// Diffuse color
	b3StoreColor(m_LightMaterial.m_Diffuse);
	b3StoreColor(m_DarkMaterial.m_Diffuse);

	// Store wood base definitions
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

	// rest of light material
	b3StoreColor(m_LightMaterial.m_Ambient);
	b3StoreColor(m_LightMaterial.m_Specular);
	b3StoreFloat(m_LightMaterial.m_Reflection);
	b3StoreFloat(m_LightMaterial.m_Refraction);
	b3StoreFloat(m_LightMaterial.m_Ior);
	b3StoreFloat(m_LightMaterial.m_SpecularExp);
}

b3_bool b3MatOakPlank::b3Prepare(b3_preparation_info *prep_info)
{
	b3_index x,y,index = 0;
	b3_f64   fx,fy;

	b3PrepareOakPlank(&m_Scale);
	b3PrepareScaling();

	if (m_DarkMaterials != null)
	{
		delete [] m_DarkMaterials;
	}

	if (m_LightMaterials != null)
	{
		delete [] m_LightMaterials;
	}

	m_DarkMaterials  = new b3_material[m_PlankCount];
	m_LightMaterials = new b3_material[m_PlankCount];

	for (y = 0;y < m_yTimes;y++)
	{
		fy = (b3_f64)y / m_yTimes;
		for (x = 0;x < m_xTimes;x++)
		{
			fx = (b3_f64)x / m_xTimes;

			m_DarkMaterials[index].m_Diffuse = m_DarkMaterial.m_Diffuse + b3Color(
												   b3Noise::b3SignedFilteredNoiseVector(fx,fy,0),
												   b3Noise::b3SignedFilteredNoiseVector(0,fx,fy),
												   b3Noise::b3SignedFilteredNoiseVector(fy,0,fx)) * m_DarkMaterial.m_Diffuse * m_Wobble * 0.5;

			m_LightMaterials[index].m_Diffuse = m_LightMaterial.m_Diffuse + b3Color(
													b3Noise::b3SignedFilteredNoiseVector(fx,fy,0),
													b3Noise::b3SignedFilteredNoiseVector(0,fx,fy),
													b3Noise::b3SignedFilteredNoiseVector(fy,0,fx)) * m_LightMaterial.m_Diffuse * m_Wobble * 0.5;

			m_DarkMaterials[index].m_Reflection  = m_DarkMaterial.m_Reflection;
			m_DarkMaterials[index].m_Refraction  = m_DarkMaterial.m_Refraction;
			m_DarkMaterials[index].m_Ior         = m_DarkMaterial.m_Ior;
			m_DarkMaterials[index].m_SpecularExp = m_DarkMaterial.m_SpecularExp;

			m_LightMaterials[index].m_Reflection  = m_LightMaterial.m_Reflection;
			m_LightMaterials[index].m_Refraction  = m_LightMaterial.m_Refraction;
			m_LightMaterials[index].m_Ior         = m_LightMaterial.m_Ior;
			m_LightMaterials[index].m_SpecularExp = m_LightMaterial.m_SpecularExp;

			index++;
		}
	}

	return true;
}

b3_bool b3MatOakPlank::b3GetSurfaceValues(b3_surface *surface)
{
	b3_index  index;
	b3_vector point;
	b3_f64    mix;

	b3Scale(surface->m_Incoming,null,&point);
	mix = b3ComputeOakPlank(&point, surface->m_Incoming->Q, index);

	b3Mix(surface,&m_DarkMaterials[index],&m_LightMaterials[index], mix);

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
	// Colors
	m_Diffuse  = b3Color(0.79f, 0.54f, 0.2f);
	m_Specular = b3Color(0.8f,  0.8f,  0.8f);
	m_Ambient  = m_Diffuse * 0.2;

	// Material values
	m_Reflection  =    0;
	m_Refraction  =    0;
	m_Ior         =    1.5;
	m_SpecularExp = 1000.0;

	// Cook & Torrance values
	m_ka   = 0.1f;
	m_ks   = 0.6f;
	m_kd   = 0.6f;
	m_m    = 0.3f;
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

b3_bool b3MatCookTorrance::b3Prepare(b3_preparation_info *prep_info)
{
	m_Ra   = m_Ambient * m_ka;
	m_Rd   = m_Diffuse * m_kd;
	m_Mu   = b3Color(
				 b3Math::b3GetMu(m_Diffuse[b3Color::R]),
				 b3Math::b3GetMu(m_Diffuse[b3Color::G]),
				 b3Math::b3GetMu(m_Diffuse[b3Color::B]));

	return true;
}

b3_bool b3MatCookTorrance::b3GetSurfaceValues(b3_surface *surface)
{
	b3_material *dst = surface;

	*dst = *this;

	return true;
}

b3_bool b3MatCookTorrance::b3Illuminate(b3_surface *surface,b3_light_info *jit)
{
	b3_ray      *ray = surface->m_Incoming;
	b3_vector64  L;

	B3_ASSERT(ray != null);

	b3Vector::b3Init(&L,&jit->dir);
	b3Vector::b3Normalize(&L);

	b3_f64 nl = b3Vector::b3SMul(&ray->normal,&L);

	jit->m_AmbientSum += m_Ra;
#if 1
	b3Color Rf;

	if (jit->shape == null)
	{
		b3_vector64 H;

		H.x = L.x - ray->dir.x;
		H.y = L.y - ray->dir.y;
		H.z = L.z - ray->dir.z;
		b3Vector::b3Normalize(&H);

		b3_f64 nh =  b3Vector::b3SMul(&ray->normal,&H);
		b3_f64 nv = -b3Vector::b3SMul(&ray->normal,&ray->dir);
		b3_f64 vh = -b3Vector::b3SMul(&ray->dir,&H);

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
		G = b3Math::b3Limit(G);

		b3_f64 alpha = b3Math::b3Acos(nh);
		b3_f64 nh_q  = nh * nh;
		b3_f64 D     = exp(-b3Math::b3Sqr(tan(alpha) / m_m)) / (m_m * m_m * nh_q * nh_q);
		b3_f64 Rs    = (D * G) / (M_PI * nv * nl);

		b3_f64 phi = b3Math::b3Asin(nl);
		for (b3_loop i = b3Color::R;i <= b3Color::B;i++)
		{
			b3Color::b3_color_index l = (b3Color::b3_color_index)i;

			Rf[l] = b3Math::b3GetFresnel(phi,m_Mu[l]) * Rs;
		}
		Rf.b3SetAlpha(0);
		Rf.b3Min();

		jit->m_DiffuseSum  += m_Rd * nl;
		jit->m_SpecularSum += Rf * m_ks;
	}

#else
	b3_f64 rl = b3Vector::b3SMul(&surface->m_ReflRay.dir,&L);

	jit->m_DiffuseSum  += m_Diffuse * nl;
	jit->m_SpecularSum += m_Specular * b3Math::b3FastPow(fabs(rl),(b3_u32)m_SpecularExp);
#endif

	return true;
}

/*************************************************************************
**                                                                      **
**                        Granite material                              **
**                                                                      **
*************************************************************************/

b3MatGranite::b3MatGranite(b3_u32 class_type) : b3Material(sizeof(b3MatGranite),class_type)
{
	m_DarkMaterial.m_Diffuse      = B3_BLACK;
	m_LightMaterial.m_Diffuse     = b3_pkd_color(0xd0dde0);
	m_DarkMaterial.m_Ambient      = m_DarkMaterial.m_Diffuse * 0.2;
	m_LightMaterial.m_Ambient     = m_LightMaterial.m_Diffuse * 0.2;
	m_DarkMaterial.m_Specular     = B3_GREY;
	m_LightMaterial.m_Specular    = B3_GREY;
	m_DarkMaterial.m_Reflection   =
		m_LightMaterial.m_Reflection  =   0.0;
	m_DarkMaterial.m_Refraction   =
		m_LightMaterial.m_Refraction  =   0.0;
	m_DarkMaterial.m_Ior          =
		m_LightMaterial.m_Ior         =   1.5;
	m_DarkMaterial.m_SpecularExp  =
		m_LightMaterial.m_SpecularExp = 100.0;
	m_Octaves     =   2;
}

b3MatGranite::b3MatGranite(b3_u32 *src) : b3Material(src)
{
	b3Color light_diffuse,dummy;

	b3InitColor(m_DarkMaterial.m_Diffuse);
	b3InitColor(light_diffuse);
	b3InitColor(dummy);
	b3InitColor(m_DarkMaterial.m_Ambient);
	b3InitColor(m_DarkMaterial.m_Specular);
	m_DarkMaterial.m_Reflection  = b3InitFloat();
	m_DarkMaterial.m_Refraction  = b3InitFloat();
	m_DarkMaterial.m_Ior         = b3InitFloat();
	m_DarkMaterial.m_SpecularExp = b3InitFloat();
	m_Scale.x       = b3InitFloat() * M_PI;
	m_Scale.y       = b3InitFloat() * M_PI;
	m_Scale.z       = b3InitFloat() * M_PI;
	m_ScaleFlags    = (b3_scaling_mode)b3InitInt();
	m_Octaves       = b3InitCount();

	m_LightMaterial = m_DarkMaterial;
	m_LightMaterial.m_Diffuse = light_diffuse;
	if (B3_PARSE_INDEX_VALID)
	{
		b3InitColor(m_LightMaterial.m_Ambient);
		b3InitColor(m_LightMaterial.m_Specular);
		m_LightMaterial.m_Reflection  = b3InitFloat();
		m_LightMaterial.m_Refraction  = b3InitFloat();
		m_LightMaterial.m_Ior         = b3InitFloat();
		m_LightMaterial.m_SpecularExp = b3InitFloat();
	}
}

void b3MatGranite::b3Write()
{
	b3StoreColor(m_DarkMaterial.m_Diffuse);
	b3StoreColor(m_LightMaterial.m_Diffuse);
	b3StoreColor(m_DarkMaterial.m_Diffuse); // dummy
	b3StoreColor(m_DarkMaterial.m_Ambient);
	b3StoreColor(m_DarkMaterial.m_Specular);
	b3StoreFloat(m_DarkMaterial.m_Reflection);
	b3StoreFloat(m_DarkMaterial.m_Refraction);
	b3StoreFloat(m_DarkMaterial.m_Ior);
	b3StoreFloat(m_DarkMaterial.m_SpecularExp);

	b3StoreFloat(m_Scale.x / M_PI);
	b3StoreFloat(m_Scale.y / M_PI);
	b3StoreFloat(m_Scale.z / M_PI);
	b3StoreInt  (m_ScaleFlags);
	b3StoreCount(m_Octaves);

	// Granite extension
	b3StoreColor(m_LightMaterial.m_Ambient);
	b3StoreColor(m_LightMaterial.m_Specular);
	b3StoreFloat(m_LightMaterial.m_Reflection);
	b3StoreFloat(m_LightMaterial.m_Refraction);
	b3StoreFloat(m_LightMaterial.m_Ior);
	b3StoreFloat(m_LightMaterial.m_SpecularExp);
}

b3_bool b3MatGranite::b3Prepare(b3_preparation_info *prep_info)
{
	b3PrepareScaling();
	return true;
}

b3_bool b3MatGranite::b3GetSurfaceValues(b3_surface *surface)
{
	b3_vector point;
	b3_f64    granite;

	b3Scale(surface->m_Incoming,&m_Scale,&point);

	granite = b3Noise::b3Granite(&point,m_Octaves);

	b3Mix(surface,&m_DarkMaterial,&m_LightMaterial, granite);

	return true;
}

/*************************************************************************
**                                                                      **
**                        Car paint material                            **
**                                                                      **
*************************************************************************/

b3MatCarPaint::b3MatCarPaint(b3_u32 class_type) : b3Material(sizeof(b3MatCarPaint),class_type)
{
	m_Parallel.m_Diffuse     = B3_BLUE;
	m_Parallel.m_Specular    = B3_GREY;
	m_Parallel.m_Ambient     = m_Parallel.m_Diffuse * 0.2;
	m_Parallel.m_Reflection  =   0.16;
	m_Parallel.m_Refraction  =   0.0;
	m_Parallel.m_Ior         =   1.56;
	m_Parallel.m_SpecularExp = 300.0;

	m_Perpendicular = m_Parallel;

	m_Flags         = B3_MAT_CP_METALLIC;
	m_MetallicScale = 0.015;
}

b3MatCarPaint::b3MatCarPaint(b3_u32 *src) : b3Material(src)
{
	b3InitColor(m_Parallel.m_Diffuse);
	b3InitColor(m_Parallel.m_Ambient);
	b3InitColor(m_Parallel.m_Specular);
	m_Parallel.m_Reflection  = b3InitFloat();
	m_Parallel.m_Refraction  = b3InitFloat();
	m_Parallel.m_Ior         = b3InitFloat();
	m_Parallel.m_SpecularExp = b3InitFloat();

	b3InitColor(m_Perpendicular.m_Diffuse);
	b3InitColor(m_Perpendicular.m_Ambient);
	b3InitColor(m_Perpendicular.m_Specular);
	m_Perpendicular.m_Reflection  = b3InitFloat();
	m_Perpendicular.m_Refraction  = b3InitFloat();
	m_Perpendicular.m_Ior         = b3InitFloat();
	m_Perpendicular.m_SpecularExp = b3InitFloat();

	m_Flags       = b3InitInt();

	if (B3_PARSE_INDEX_VALID)
	{
		m_MetallicScale = b3InitFloat();
	}
	else
	{
		m_Flags |= B3_MAT_CP_METALLIC;
		m_MetallicScale = 0.018;
	}
}

void b3MatCarPaint::b3Write()
{
	b3StoreColor(m_Parallel.m_Diffuse);
	b3StoreColor(m_Parallel.m_Ambient);
	b3StoreColor(m_Parallel.m_Specular);
	b3StoreFloat(m_Parallel.m_Reflection);
	b3StoreFloat(m_Parallel.m_Refraction);
	b3StoreFloat(m_Parallel.m_Ior);
	b3StoreFloat(m_Parallel.m_SpecularExp);

	b3StoreColor(m_Perpendicular.m_Diffuse);
	b3StoreColor(m_Perpendicular.m_Ambient);
	b3StoreColor(m_Perpendicular.m_Specular);
	b3StoreFloat(m_Perpendicular.m_Reflection);
	b3StoreFloat(m_Perpendicular.m_Refraction);
	b3StoreFloat(m_Perpendicular.m_Ior);
	b3StoreFloat(m_Perpendicular.m_SpecularExp);

	b3StoreInt(m_Flags);
	b3StoreFloat(m_MetallicScale);
}

b3_bool b3MatCarPaint::b3Prepare(b3_preparation_info *prep_info)
{
	m_MetallicScaleHalf = m_MetallicScale * 0.5;
	return true;
}

b3_bool b3MatCarPaint::b3GetSurfaceValues(b3_surface *surface)
{
	b3_ray      *ray = surface->m_Incoming;
	b3_f64       ni;

	if (m_Flags & B3_MAT_CP_METALLIC)
	{
		b3_vector64  normal;

		b3Randomize(&ray->normal, &normal, m_MetallicScale,m_MetallicScaleHalf);

		ni = fabs(b3Vector::b3SMul(&normal,&ray->dir));
	}
	else
	{
		ni = fabs(b3Vector::b3SMul(&ray->normal,&ray->dir));
	}
	b3Mix(surface,&m_Parallel,&m_Perpendicular, ni);

	return true;
}

b3_bool b3MatCarPaint::b3Illuminate(b3_surface *surface,b3_light_info *jit)
{
	jit->m_AmbientSum += surface->m_Ambient;
	if (jit->shape == null)
	{
		b3_ray      *ray = surface->m_Incoming;
		b3_vector64  L;
		b3_vector64  refl_dir;
		b3_f64       nl;
		b3_f64       rl;

		B3_ASSERT(ray != null);

		b3Vector::b3Init(&L,&jit->dir);
		b3Vector::b3Normalize(&L);

		nl = b3Vector::b3SMul(&ray->normal,&L);

		if (m_Flags & B3_MAT_CP_METALLIC)
		{
			b3Randomize(&surface->m_ReflRay.dir, &refl_dir, m_MetallicScale, m_MetallicScaleHalf);

			rl = b3Vector::b3SMul(&refl_dir,&L);
		}
		else
		{
			rl = b3Vector::b3SMul(&surface->m_ReflRay.dir,&L);
		}

		jit->m_DiffuseSum  += surface->m_Diffuse * nl * jit->m_LightFrac;
		jit->m_SpecularSum += surface->m_Specular * b3Math::b3FastPow(fabs(rl),(b3_u32)surface->m_SpecularExp);
	}

	return true;
}

b3_bool b3MatCarPaint::b3ShadeComponents(b3_surface *surface, b3_f64 reflection, b3_f64 refraction)
{
	surface->m_Incoming->color =
		surface->m_AmbientSum +
		surface->m_DiffuseSum +
		surface->m_SpecularSum +
		surface->m_RefrRay.color * refraction +
		surface->m_ReflRay.color * reflection;
	return true;
}

/*************************************************************************
**                                                                      **
**                        Thin film material                            **
**                                                                      **
*************************************************************************/

b3Color   b3MatThinFilm::m_WaveLength(700.0,510,485.0,0); // in nano meter
b3_vector b3MatThinFilm::m_ScaleTime =
	{
		10,10,10
	};

b3MatThinFilm::b3MatThinFilm(b3_u32 class_type) : b3Material(sizeof(b3MatThinFilm),class_type)
{
	m_Diffuse     = B3_PEARL;
	m_Ambient     = m_Diffuse * 0.2;
	m_Specular    = B3_GREY;
	m_Intensity.b3Init(0.2f);
	m_Reflection  =    0;
	m_Refraction  =    0;
	m_Ior         =    1.5f;
	m_SpecularExp = 1000;
	m_Flags       =    0;
	m_Thickness   =	   7; // in micro meter
	b3InitScaling(0.05,B3_SCALE_IPOINT_ORIGINAL);
}

b3MatThinFilm::b3MatThinFilm(b3_u32 *src) : b3Material(src)
{
	b3InitColor(m_Diffuse);
	b3InitColor(m_Ambient);
	b3InitColor(m_Specular);
	m_Reflection  = b3InitFloat();
	m_Refraction  = b3InitFloat();
	m_Ior         = b3InitFloat();
	m_SpecularExp = b3InitFloat();
	m_Flags       = b3InitInt();
	m_Thickness   = b3InitFloat();
	b3InitColor (m_Intensity);
	b3InitVector(&m_Scale);
	m_ScaleFlags    = (b3_scaling_mode)b3InitInt();
}

void b3MatThinFilm::b3Write()
{
	b3StoreColor (m_Diffuse);
	b3StoreColor (m_Ambient);
	b3StoreColor (m_Specular);
	b3StoreFloat (m_Reflection);
	b3StoreFloat (m_Refraction);
	b3StoreFloat (m_Ior);
	b3StoreFloat (m_SpecularExp);
	b3StoreInt   (m_Flags);
	b3StoreFloat (m_Thickness);
	b3StoreColor (m_Intensity);
	b3StoreVector(&m_Scale);
	b3StoreInt   (m_ScaleFlags);
}

b3_bool b3MatThinFilm::b3Prepare(b3_preparation_info *prep_info)
{
	b3_scene_preparation *info = (b3_scene_preparation *)prep_info;

	m_t = info->m_t;
	b3PrepareScaling();
	return true;
}

b3_bool b3MatThinFilm::b3GetSurfaceValues(b3_surface *surface)
{
	b3Color      factor;
	b3_vector    point;
	b3_vector    shift;
	b3_vector64 *normal = &surface->m_Incoming->normal;
	b3_f64       quotient;
	b3_f64       cos_phi;
	b3_f64       sin_theta_sqr,cos_theta;
	b3_f64       wobble;

	// scale
	b3Scale(surface->m_Incoming,&m_Scale,&point);

	// Compute animation
	b3Noise::b3AnimThinFilm(m_t,&shift);
	point.x += m_ScaleTime.x * shift.x;
	point.y += m_ScaleTime.y * shift.y;
	point.z += m_ScaleTime.z * shift.z;

	wobble =
		b3Noise::b3SignedFilteredNoiseVector(point.x,    point.y,    point.z) +
		b3Noise::b3SignedFilteredNoiseVector(point.x * 2,point.y * 2,point.z * 2) * 0.5 +
		b3Noise::b3SignedFilteredNoiseVector(point.x * 4,point.y * 4,point.z * 4) * 0.25;

	// compute refraction angle
	cos_phi       = b3Vector::b3SMul(&surface->m_Incoming->dir,normal);
	sin_theta_sqr = (1.0 - cos_phi * cos_phi) / (m_Ior * m_Ior);
	cos_theta     = sqrt(1.0 - sin_theta_sqr);
	quotient      = 4000.0 * M_PI * m_Thickness * (1.5 + 0.5 * wobble) * cos_theta;

	// compute interferences
	for (int i = b3Color::R;i <= b3Color::B;i++)
	{
		factor[i] = (1 - cos(quotient / m_WaveLength[i])) * 0.5;
	}
	factor.b3SetAlpha(1);

	// copy some values
	surface->m_Ambient     = m_Ambient;
	surface->m_Diffuse     = m_Diffuse + m_Intensity * factor;
	surface->m_Specular    = m_Specular;
	surface->m_Reflection  = m_Reflection;
	surface->m_Refraction  = m_Refraction;
	surface->m_Ior         = m_Ior;
	surface->m_SpecularExp = m_SpecularExp;
	return true;
}
