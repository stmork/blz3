/*
**
**	$Filename:	b3Material.h $ 
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing class definitions for materials
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifndef B3_RAYTRACE_MATERIAL_H
#define B3_RAYTRACE_MATERIAL_H

#include "blz3/raytrace/b3Scaling.h"
#include "blz3/base/b3Math.h"
#include "blz3/base/b3Wood.h"
#include "blz3/image/b3Tx.h"

/*************************************************************************
**                                                                      **
**                        Materials                                     **
**                                                                      **
*************************************************************************/

#define CLASS_MATERIAL         0x40000000
#define TYPE_NORMMATERIAL      0x00000001
#define TYPE_MAT_TEXTURE       0x00000002
#define TYPE_MAT_CHESS         0x00000003
#define TYPE_MAT_WRAPTEXTURE   0x00000004
#define TYPE_MAT_MARBLE        0x00000005
#define TYPE_MAT_SLIDE         0x00000006
#define TYPE_MAT_WOOD          0x00000007
#define TYPE_MAT_COOK_TORRANCE 0x00000008
#define TYPE_MAT_GRANITE       0x00000009
#define TYPE_MAT_OAKPLANK      0x0000000a
#define TYPE_MAT_CAR_PAINT     0x0000000b
#define TYPE_MAT_THIN_FILM     0x0000000c

// WARNING: GL uses define MATERIAL, too!
#define MAT_NORMAL          (CLASS_MATERIAL|TYPE_NORMMATERIAL)
#define MATERIAL            (CLASS_MATERIAL|TYPE_NORMMATERIAL)
#define TEXTURE             (CLASS_MATERIAL|TYPE_MAT_TEXTURE)
#define CHESS               (CLASS_MATERIAL|TYPE_MAT_CHESS)
#define WRAPTEXTURE         (CLASS_MATERIAL|TYPE_MAT_WRAPTEXTURE)
#define MARBLE              (CLASS_MATERIAL|TYPE_MAT_MARBLE)
#define SLIDE               (CLASS_MATERIAL|TYPE_MAT_SLIDE)
#define WOOD                (CLASS_MATERIAL|TYPE_MAT_WOOD)
#define COOK_TORRANCE       (CLASS_MATERIAL|TYPE_MAT_COOK_TORRANCE)
#define GRANITE             (CLASS_MATERIAL|TYPE_MAT_GRANITE)
#define OAKPLANK            (CLASS_MATERIAL|TYPE_MAT_OAKPLANK)
#define CAR_PAINT           (CLASS_MATERIAL|TYPE_MAT_CAR_PAINT)
#define THIN_FILM           (CLASS_MATERIAL|TYPE_MAT_THIN_FILM)

struct b3_material
{
	b3Color     m_Diffuse;
	b3Color     m_Ambient;
	b3Color     m_Specular;
	b3_f64      m_Reflection;
	b3_f64      m_Refraction;
	b3_f64      m_Ior;
	b3_f64      m_SpecularExp;
};

struct b3_surface : public b3_material
{
	b3Color      m_AmbientSum;
	b3Color      m_DiffuseSum;
	b3Color      m_SpecularSum;
	b3_ray      *m_Incoming;
	b3_ray       m_ReflRay;
	b3_ray       m_RefrRay;
	b3_bool      m_Transparent;
	b3_f64       m_Fresnel;
	b3_f64       m_IorComputed;
	b3_f64       m_CosAlpha;
};

/*************************************************************************
**                                                                      **
**                        Base class for all materials                  **
**                                                                      **
*************************************************************************/
 
struct b3_light_info;

class B3_PLUGIN b3Material : public b3Item
{
protected:
	B3_ITEM_BASE(b3Material);

public:
	B3_ITEM_INIT(b3Material);
	B3_ITEM_LOAD(b3Material);

	static  void    b3Register();
	
	virtual inline b3_bool b3Prepare()
	{
		return true;
	}

	/********************** Shading pipeline ********************************/

	// Retrieve surface values and decide to terminate value retrieval.
	virtual inline b3_bool b3GetSurfaceValues(b3_surface *surface)
	{
		surface->m_Diffuse     = B3_LIGHT_BLUE;
		surface->m_Ambient     = surface->m_Diffuse * 0.2;
		surface->m_Specular    = B3_GREY;
		surface->m_Reflection  =      0.0;
		surface->m_Refraction  =      0.0;
		surface->m_Ior         =      1.5;
		surface->m_SpecularExp = 100000.0;

		return false;
	}

	// Make lighting: Compute ambient, diffuse and specular components from
	// already retrieved surface values. If the configured shader should
	// compute this return simply false.
	virtual inline b3_bool b3Illuminate(b3_surface *surface,b3_light_info *jit)
	{
		return false;
	}

	// Static entry point for component mixing (ambient, diffuse, specular terms plus refracted and
	// refleted parts.
	static inline b3_bool b3MixComponents(b3_surface *surface, b3_f64 reflection, b3_f64 refraction)
	{
		b3Material *material = surface->m_Incoming->material;

		return material != null ? material->b3ShadeComponents(surface,reflection,refraction) : false;
	}

protected:
	// Mix color components. If the configured shader should do this job return simply false.
	virtual inline b3_bool b3ShadeComponents(b3_surface *surface, b3_f64 reflection, b3_f64 refraction)
	{
		return false;
	}

	/********************** Shading pipeline ********************************/

	static inline void b3Mix(
		      b3_surface  *surface,
		const b3_material *a,
		const b3_material *b,
		const b3_f64       mix)
	{
		b3Color b_mixer;
		b3Color a_mixer;
		b3_f64  a_mix = 1.0 - mix;

		b_mixer.b3InitFactor(mix);
		a_mixer.b3InitFactor(a_mix);
		surface->m_Ambient     = a_mixer * a->m_Ambient  + b_mixer * b->m_Ambient;
		surface->m_Diffuse     = a_mixer * a->m_Diffuse  + b_mixer * b->m_Diffuse;
		surface->m_Specular    = a_mixer * a->m_Specular + b_mixer * b->m_Specular;

		surface->m_Reflection  =  a->m_Reflection  * a_mix + b->m_Reflection  * mix;
		surface->m_Refraction  =  a->m_Refraction  * a_mix + b->m_Refraction  * mix;
		surface->m_Ior         =  a->m_Ior         * a_mix + b->m_Ior         * mix;
		surface->m_SpecularExp =  a->m_SpecularExp * a_mix + b->m_SpecularExp * mix;
	}
};

/*************************************************************************
**                                                                      **
**                        Standard material                             **
**                                                                      **
*************************************************************************/

class B3_PLUGIN b3MatNormal : public b3Material, public b3_material
{
public:
	b3_u32            m_Flags;

protected:
	     b3MatNormal(b3_size class_size,b3_u32 class_type);

public:
	B3_ITEM_INIT(b3MatNormal);
	B3_ITEM_LOAD(b3MatNormal);

	        void    b3Write();
	virtual b3_bool b3GetSurfaceValues(b3_surface *surface);

private:
	        void    b3Init();
};

/*************************************************************************
**                                                                      **
**                        Chess material                                **
**                                                                      **
*************************************************************************/

class B3_PLUGIN b3MatChess : public b3Material 
{
public:
	b3_material m_Material[2];
	b3_s32      m_Flags;
	b3_s32      m_xTimes;
	b3_s32      m_yTimes;

	enum b3_chess
	{
		BLACK = 0,
		WHITE = 1
	};

public:
	B3_ITEM_INIT(b3MatChess);
	B3_ITEM_LOAD(b3MatChess);

	void    b3Write();
	b3_bool b3GetSurfaceValues(b3_surface *surface);
};

/*************************************************************************
**                                                                      **
**                        Texture material                              **
**                                                                      **
*************************************************************************/

class B3_PLUGIN b3MatTexture : public b3Material 
{
public:
	b3_f32            m_Reflection;
	b3_f32            m_Refraction;
	b3_f32            m_Ior;
	b3_f32            m_SpecularExp;
	b3_f32            m_xStart,m_yStart;    // surface coordinate start
	b3_f32            m_xScale,m_yScale;    // texture scale
	b3_s32            m_xTimes,m_yTimes;    // repetition in x- y-direction
	b3_s32            m_Flags;
	b3Path            m_Name;
	b3Tx             *m_Texture;

public:
	B3_ITEM_INIT(b3MatTexture);
	B3_ITEM_LOAD(b3MatTexture);

	void    b3Write();
	b3_bool b3Prepare();
	void    b3SetTexture(const char *name);
	b3_bool b3GetSurfaceValues(b3_surface *surface);
};

/*************************************************************************
**                                                                      **
**                        Wrapping texture material                     **
**                                                                      **
*************************************************************************/

class B3_PLUGIN b3MatWrapTexture : public b3Material 
{
public:
	b3_f32            m_Reflection;
	b3_f32            m_Refraction;
	b3_f32            m_Ior;
	b3_f32            m_SpecularExp;
	b3_f32            m_xStart,m_yStart;  // surface coordinate start
	b3_f32            m_xEnd,m_yEnd;      // surface coordinate end
	b3_s32            m_Flags;
	b3Path            m_Name;
	b3Tx             *m_Texture;          // only one texture (compat. Dali)

public:
	B3_ITEM_INIT(b3MatWrapTexture);
	B3_ITEM_LOAD(b3MatWrapTexture);

	void    b3Write();
	b3_bool b3Prepare();
	void    b3SetTexture(const char *name);
	b3_bool b3GetSurfaceValues(b3_surface *surface);
};

/*************************************************************************
**                                                                      **
**                        Color sliding material                        **
**                                                                      **
*************************************************************************/

class B3_PLUGIN b3MatSlide : public b3Material 
{
public:
	b3_material       m_Material[2];
	b3_f32            m_From,m_To;           // rel. polar values of start, end
	b3_s32            m_ModeFlag;            // direction and cut flags, see below

public:
	B3_ITEM_INIT(b3MatSlide);
	B3_ITEM_LOAD(b3MatSlide);

	void    b3Write();
	b3_bool b3GetSurfaceValues(b3_surface *surface);
};

#define DIR_SLIDE_BIT       0
#define CUT_SLIDE_BIT       1
#define XINVERT_BIT         2
#define YINVERT_BIT         3
#define MAT_INSIDE_BIT      4

#define SLIDE_CUT           (1 << CUT_SLIDE_BIT)
#define MAT_XINVERT			(1 << XINVERT_BIT)
#define MAT_YINVERT         (1 << YINVERT_BIT)
#define MAT_INSIDE          (1 << MAT_INSIDE_BIT)

#define XSLIDE              0x00000000
#define YSLIDE              0x00000001
#define XSLIDE_CUT          (XSLIDE|SLIDE_CUT)
#define YSLIDE_CUT          (YSLIDE|SLIDE_CUT)

/*************************************************************************
**                                                                      **
**                        Marble material                               **
**                                                                      **
*************************************************************************/

class B3_PLUGIN b3MatMarble : public b3Material, public b3Scaling
{
	b3_s32            m_xTimes,m_yTimes;
public:
	b3_material       m_DarkMaterial;
	b3_material       m_LightMaterial;

public:
	B3_ITEM_INIT(b3MatMarble);
	B3_ITEM_LOAD(b3MatMarble);

	void    b3Write();
	b3_bool b3Prepare();
	b3_bool b3GetSurfaceValues(b3_surface *surface);
};

/*************************************************************************
**                                                                      **
**                        Base class for wooden materials               **
**                                                                      **
*************************************************************************/

class B3_PLUGIN b3MaterialWooden : public b3Material, public b3Scaling
{
public:
	b3_material       m_DarkMaterial;
	b3_material       m_LightMaterial;

protected:
	B3_ITEM_BASE(b3MaterialWooden);

public:
	B3_ITEM_INIT(b3MaterialWooden);
	B3_ITEM_LOAD(b3MaterialWooden);

protected:
	void     b3Init();
};

/*************************************************************************
**                                                                      **
**                        Wooden material                               **
**                                                                      **
*************************************************************************/

class B3_PLUGIN b3MatWood : public b3MaterialWooden, public b3Wood
{
public:
	b3_u32            m_xTimes,m_yTimes; // not used

public:
	B3_ITEM_INIT(b3MatWood);
	B3_ITEM_LOAD(b3MatWood);

	void    b3Write();
	b3_bool b3Prepare();
	b3_bool b3GetSurfaceValues(b3_surface *surface);

private:
	void    b3Init();
};

/*************************************************************************
**                                                                      **
**                        Oak planks                                    **
**                                                                      **
*************************************************************************/

class B3_PLUGIN b3MatOakPlank : public b3MaterialWooden, public b3OakPlank
{
	b3_material     *m_LightMaterials;
	b3_material     *m_DarkMaterials;

public:
	B3_ITEM_INIT(b3MatOakPlank);
	B3_ITEM_LOAD(b3MatOakPlank);

	virtual ~b3MatOakPlank();

	void     b3Write();
	b3_bool  b3Prepare();
	b3_bool  b3GetSurfaceValues(b3_surface *surface);

private:
	void     b3Init();
};

/*************************************************************************
**                                                                      **
**                        Cook and Torrance reflection model            **
**                                                                      **
*************************************************************************/

class B3_PLUGIN b3MatCookTorrance : public b3MatNormal
{
	b3Color     m_Ra;
	b3Color     m_Rd;
	b3Color     m_Mu;

public:
	b3_f32      m_ka;
	b3_f32      m_ks;
	b3_f32      m_kd;
	b3_f32      m_m;

public:
	B3_ITEM_INIT(b3MatCookTorrance);
	B3_ITEM_LOAD(b3MatCookTorrance);

	void    b3Write();
	b3_bool b3Prepare();
	b3_bool b3Illuminate(b3_surface *surface,b3_light_info *jit);

	inline b3_bool b3GetSurfaceValues(b3_surface *surface)
	{
		return true;
	}
};

/*************************************************************************
**                                                                      **
**                        Granite material                              **
**                                                                      **
*************************************************************************/

class B3_PLUGIN b3MatGranite : public b3Material, public b3Scaling
{
public:
	b3_material       m_DarkMaterial;
	b3_material       m_LightMaterial;
	b3_count          m_Octaves;

public:
	B3_ITEM_INIT(b3MatGranite);
	B3_ITEM_LOAD(b3MatGranite);

	void    b3Write();
	b3_bool b3Prepare();
	b3_bool b3GetSurfaceValues(b3_surface *surface);
};

/*************************************************************************
**                                                                      **
**                        Car paint material                            **
**                                                                      **
*************************************************************************/

class B3_PLUGIN b3MatCarPaint : public b3Material
{
	b3_f64            m_MetallicScaleHalf;

public:
	b3_material       m_Parallel;
	b3_material       m_Perpendicular;
	b3_u32            m_Flags;
	b3_f64            m_MetallicScale;

public:
	B3_ITEM_INIT(b3MatCarPaint);
	B3_ITEM_LOAD(b3MatCarPaint);

	void    b3Write();
	b3_bool b3Prepare();
	b3_bool b3GetSurfaceValues(b3_surface *surface);
	b3_bool b3Illuminate(b3_surface *surface,b3_light_info *jit);

protected:
	b3_bool b3ShadeComponents(b3_surface *surface, b3_f64 reflection, b3_f64 refraction);
};

#define B3_MAT_CP_METALLIC 1

/*************************************************************************
**                                                                      **
**                        Thin film material                            **
**                                                                      **
*************************************************************************/

class B3_PLUGIN b3MatThinFilm : public b3Material, public b3_material, public b3Scaling
{
	static b3Color   m_WaveLength;
	static b3_vector m_ScaleTime;

public:
	b3_s32           m_Flags;
	b3_f32           m_Thickness; // in micro meter
	b3Color          m_Intensity;

public:
	B3_ITEM_INIT(b3MatThinFilm);
	B3_ITEM_LOAD(b3MatThinFilm);

	void    b3Write();
	b3_bool b3Prepare();
	b3_bool b3GetSurfaceValues(b3_surface *surface);
};

#endif
