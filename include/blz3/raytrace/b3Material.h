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

/**
 * This structure provides basic material properties.
 */
struct b3_material
{
	b3Color     m_Diffuse;     //!< Diffuse surface color.
	b3Color     m_Ambient;     //!< Ambient surface color.
	b3Color     m_Specular;    //!< Specular surface color.
	b3_f64      m_Reflection;  //!< The surface reflection.
	b3_f64      m_Refraction;  //!< The surface refractance.
	b3_f64      m_Ior;         //!< The index of refraction.
	b3_f64      m_SpecularExp; //!< The specular exponent in range [1..100000]
};

/**
 * This structure holds information about what's going on at an intersection point.
 */
struct b3_surface : public b3_material
{
	b3Color      m_AmbientSum;       //!< The ambient sum integrated over all light sources.
	b3Color      m_DiffuseSum;       //!< The diffuse sum integrated over all light sources.
	b3Color      m_SpecularSum;      //!< The specular sum integrated over all light sources.
	b3_ray      *m_Incoming;         //!< The incoming ray
	b3_ray       m_ReflRay;          //!< The outgoing reflection ray.
	b3_ray       m_RefrRay;          //!< The outgoing refraction ray.
	b3_bool      m_Transparent;      //!< A flag indicating a transparent surface.
	b3_f64       m_Fresnel;          //!< The Fresnel term depending on the angle of incidence and the index of refraction.
	b3_f64       m_IorComputed;      //!< The real index of refraction depending if the incoming ray is inside or outside.
	b3_f64       m_CosAlpha;         //!< The angle of incidence as cosine value.
};

/*************************************************************************
**                                                                      **
**                        Base class for all materials                  **
**                                                                      **
*************************************************************************/
 
struct b3_light_info;

/**
 * This class defines the surface properties at a given intersection point.
 *
 * \dotfile material.dot "class hierarchy of materials"
 */
class B3_PLUGIN b3Material : public b3Item
{
protected:
	B3_ITEM_BASE(b3Material); //!< This is a base class deserialization constructor.

public:
	B3_ITEM_INIT(b3Material); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3Material); //!< This constructor handles deserialization.

	/**
	 * This method registers the default material implementations to the b3World item registry.
	 */
	static  void    b3Register();
	
	virtual inline b3_bool b3Prepare()
	{
		return true;
	}

	/********************** Shading pipeline ********************************/

	/**
	 * Retrieve surface values and decide to terminate value retrieval.
	 * The default implementation returns a simple material.
	 *
	 * @param surface The surface structure to fill in the material properties.
	 * @return True if the material traversal should terminate.
	 */
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

	/**
	 * Make lighting: Compute ambient, diffuse and specular components from
	 * already retrieved surface values. If the configured shader should
	 * compute this return simply false.
	 *
	 * @param surface The surface values.
	 * @param lit The lighting values.
	 * @return True on material computation, false on shader computation.
	 */
	virtual inline b3_bool b3Illuminate(b3_surface *surface,b3_light_info *lit)
	{
		return false;
	}

	/**
	 * This method is the static entry point for component mixing (ambient, diffuse, specular terms plus refracted and
	 * reflected parts.
	 *
	 * @param surface 
	 * @param reflection The reflectance.
	 * @param refraction The refractance.
	 * @return True if mixed via material properties. 
	 */ 
	static inline b3_bool b3MixComponents(b3_surface *surface, b3_f64 reflection, b3_f64 refraction)
	{
		b3Material *material = surface->m_Incoming->material;

		return material != null ? material->b3ShadeComponents(surface,reflection,refraction) : false;
	}

protected:
	/**
	 * Mix color components. If the configured shader should do this job return simply false.
	 *
	 * @param surface The surface properties.
	 * @param reflection The reflectance.
	 * @param refraction The refractance.
	 * @return True on component mixing, false on shader intelligence.
	 */
	virtual inline b3_bool b3ShadeComponents(b3_surface *surface, b3_f64 reflection, b3_f64 refraction)
	{
		return false;
	}

	/********************** Shading pipeline ********************************/

	/**
	 * This method mixes to material property collection by a defined mixer value.
	 *
	 * @param surface The surface which receives the result.
	 * @param a The first material to mix.
	 * @param b The second material to mix.
	 * @param mix The mixing value.
	 * @see b3Color::b3Mix()
	 * @see b3Math::b3Mix()
	 */
	static inline void b3Mix(
		      b3_surface  *surface,
		const b3_material *a,
		const b3_material *b,
		const b3_f64       mix)
	{
		b3Color mixer;

		mixer.b3InitFactor(mix);

		surface->m_Ambient     = b3Color::b3Mix(a->m_Ambient,  b->m_Ambient,  mixer);
		surface->m_Diffuse     = b3Color::b3Mix(a->m_Diffuse,  b->m_Diffuse,  mixer);
		surface->m_Specular    = b3Color::b3Mix(a->m_Specular, b->m_Specular, mixer);

		surface->m_Reflection  = a->m_Reflection  + mix * (b->m_Reflection  - a->m_Reflection);
		surface->m_Refraction  = a->m_Refraction  + mix * (b->m_Refraction  - a->m_Refraction);
		surface->m_Ior         = a->m_Ior         + mix * (b->m_Ior         - a->m_Ior);
		surface->m_SpecularExp = a->m_SpecularExp + mix * (b->m_SpecularExp - a->m_SpecularExp);
	}
};

/*************************************************************************
**                                                                      **
**                        Standard material                             **
**                                                                      **
*************************************************************************/

/**
 * This class is the minimal material description possible in Blizzard III.
 * It reflects simply the b3_material structure.
 */
class B3_PLUGIN b3MatNormal : public b3Material, public b3_material
{
public:
	b3_u32            m_Flags; //!< An unused set of flags.

protected:
	B3_ITEM_BASE(b3MatNormal); //!< This is a base class deserialization constructor.

public:
	B3_ITEM_INIT(b3MatNormal); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3MatNormal); //!< This constructor handles deserialization.

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

/**
 * This class implements a chess board. Two materials are orientated like
 * a chess board. The materials must not be black or white but they are
 * named so for analogy.
 */
class B3_PLUGIN b3MatChess : public b3Material 
{
public:
	b3_material m_Material[2]; //!< The two used materials.
	b3_s32      m_Flags;       //!< Unused.
	b3_s32      m_xTimes;      //!< The horizontal repeatition count.
	b3_s32      m_yTimes;      //!< The vertical repeatition count.

	/**
	 * This enumeration lists the material indices.
	 */
	enum b3_chess
	{
		BLACK = 0, //!< Index for black material.
		WHITE = 1  //!< Index for white material.
	};

public:
	B3_ITEM_INIT(b3MatChess); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3MatChess); //!< This constructor handles deserialization.

	void    b3Write();
	b3_bool b3GetSurfaceValues(b3_surface *surface);
};

/*************************************************************************
**                                                                      **
**                        Texture material                              **
**                                                                      **
*************************************************************************/

/**
 * This class implements simple texture mapping. The 2D polar surface coordinates
 * are transformed into texture coordinate domain using start point (translation)
 * and scaling. The texture can be tiled.
 */
class B3_PLUGIN b3MatTexture : public b3Material 
{
public:
	b3_f32            m_Reflection;         //!< The reflectance.
	b3_f32            m_Refraction;         //!< The refractance.
	b3_f32            m_Ior;                //!< The index of refraction.
	b3_f32            m_SpecularExp;        //!< The specular exponent in range [1..100000].
	b3_f32            m_xStart,m_yStart;    //!< Surface coordinate start.
	b3_f32            m_xScale,m_yScale;    //!< Texture scale.
	b3_s32            m_xTimes,m_yTimes;    //!< Repeatition in x- y-direction.
	b3_s32            m_Flags;              //!< Unused.
	b3Path            m_Name;               //!< The texture file name.
	b3Tx             *m_Texture;            //!< The selected texture.

public:
	B3_ITEM_INIT(b3MatTexture); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3MatTexture); //!< This constructor handles deserialization.

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

/**
 * This class implements a wrapping texture. This fits into the specified 2D
 * polar surface coordinates.
 */
class B3_PLUGIN b3MatWrapTexture : public b3Material 
{
public:
	b3_f32            m_Reflection;         //!< The reflectance.
	b3_f32            m_Refraction;         //!< The refractance.
	b3_f32            m_Ior;                //!< The index of refraction.
	b3_f32            m_SpecularExp;        //!< The specular exponent in range [1..100000].
	b3_f32            m_xStart,m_yStart;    //!< Surface coordinate start
	b3_f32            m_xEnd,m_yEnd;        //!< Surface coordinate end
	b3_s32            m_Flags;              //!< Unused.
	b3Path            m_Name;               //!< The texture file name.
	b3Tx             *m_Texture;            //!< The selected texture.

public:
	B3_ITEM_INIT(b3MatWrapTexture); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3MatWrapTexture); //!< This constructor handles deserialization.

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

/**
 * This class forms a material slide. The slide can be orientated in horizontal
 * (XSLIDE) or vertical direction (YSLIDE). The slide can be clamped at the
 * borders. This influences the further material computation during the b3GetSurfaceValues()
 * method when this class occures inside a material list.
 */
class B3_PLUGIN b3MatSlide : public b3Material 
{
public:
	b3_material       m_Material[2];         //!< The border materials
	b3_f32            m_From,m_To;           //!< The relative polar values of start, end
	b3_s32            m_ModeFlag;            //!< The direction and cut flags, see below

public:
	B3_ITEM_INIT(b3MatSlide); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3MatSlide); //!< This constructor handles deserialization.

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

/**
 * This class simulates marble material. The marble surface is computed
 * using the b3Noise::b3Marble() method. The resulting scalar mixes
 * the two user defined materials.
 */
class B3_PLUGIN b3MatMarble : public b3Material, public b3Scaling
{
	b3_s32            m_xTimes,m_yTimes;

public:
	b3_material       m_DarkMaterial;  //!< The darkest material.
	b3_material       m_LightMaterial; //!< The lightest material.

public:
	B3_ITEM_INIT(b3MatMarble); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3MatMarble); //!< This constructor handles deserialization.

	void    b3Write();
	b3_bool b3Prepare();
	b3_bool b3GetSurfaceValues(b3_surface *surface);
};

/*************************************************************************
**                                                                      **
**                        Base class for wooden materials               **
**                                                                      **
*************************************************************************/

/**
 * This base class handles all wooden material implementation. It inherits
 * also the scale handling from the b3Scaling class.
 *
 * @see b3Scaling.
 */
class B3_PLUGIN b3MaterialWooden : public b3Material, public b3Scaling
{
public:
	b3_material       m_DarkMaterial;  //!< The dark colored wood material.
	b3_material       m_LightMaterial; //!< The light colored wood material.

protected:
	B3_ITEM_BASE(b3MaterialWooden); //!< This is a base class deserialization constructor.

public:
	B3_ITEM_INIT(b3MaterialWooden); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3MaterialWooden); //!< This constructor handles deserialization.

protected:
	void     b3Init();
};

/*************************************************************************
**                                                                      **
**                        Wooden material                               **
**                                                                      **
*************************************************************************/

/**
 * This class simulates wood.
 *
 * @see b3MaterialWooden
 * @see b3Wood
 */
class B3_PLUGIN b3MatWood : public b3MaterialWooden, public b3Wood
{
	b3_u32            m_xTimes,m_yTimes; // not used

public:
	B3_ITEM_INIT(b3MatWood); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3MatWood); //!< This constructor handles deserialization.

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

/**
 * This class implements wooden oak planks.
 *
 * @see b3MaterialWooden
 * @see b3Wood
 * @see b3OakPlank
 */
class B3_PLUGIN b3MatOakPlank : public b3MaterialWooden, public b3OakPlank
{
	b3_material     *m_LightMaterials;
	b3_material     *m_DarkMaterials;

public:
	B3_ITEM_INIT(b3MatOakPlank); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3MatOakPlank); //!< This constructor handles deserialization.

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

/**
 * This class implements the Cook & Torrance shading model.
 */
class B3_PLUGIN b3MatCookTorrance : public b3MatNormal
{
	b3Color     m_Ra;
	b3Color     m_Rd;
	b3Color     m_Mu;

public:
	b3_f32      m_ka; //!< The ambient scaling factor.
	b3_f32      m_ks; //!< The specular scaling factor.
	b3_f32      m_kd; //!< The diffuse scaling factor.
	b3_f32      m_m;  //!< Fresnel factor.

public:
	B3_ITEM_INIT(b3MatCookTorrance); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3MatCookTorrance); //!< This constructor handles deserialization.

	void    b3Write();
	b3_bool b3Prepare();
	b3_bool b3GetSurfaceValues(b3_surface *surface);
	b3_bool b3Illuminate(b3_surface *surface,b3_light_info *lit);
};

/*************************************************************************
**                                                                      **
**                        Granite material                              **
**                                                                      **
*************************************************************************/

/**
 * This class simulates granite material. The granite surface is computed
 * using the b3Noise::b3Granite() method. The resulting scalar mixes
 * the two user defined materials.
 */
class B3_PLUGIN b3MatGranite : public b3Material, public b3Scaling
{
public:
	b3_material       m_DarkMaterial;  //!< The lightest material.
	b3_material       m_LightMaterial; //!< The darkest material.
	b3_count          m_Octaves;       //!< The octave iterations used for turbulence.

public:
	B3_ITEM_INIT(b3MatGranite); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3MatGranite); //!< This constructor handles deserialization.

	void    b3Write();
	b3_bool b3Prepare();
	b3_bool b3GetSurfaceValues(b3_surface *surface);
};

/*************************************************************************
**                                                                      **
**                        Car paint material                            **
**                                                                      **
*************************************************************************/

/**
 * This class simulates car surface colors.
 */
class B3_PLUGIN b3MatCarPaint : public b3Material
{
	b3_f64            m_MetallicScaleHalf;

public:
	b3_material       m_Parallel;      //!< Parallel material.
	b3_material       m_Perpendicular; //!< Perpendicular material.
	b3_u32            m_Flags;         //!< Some flags
	b3_f64            m_MetallicScale; //!< Scaling the metallic effect.

public:
	B3_ITEM_INIT(b3MatCarPaint); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3MatCarPaint); //!< This constructor handles deserialization.

	void    b3Write();
	b3_bool b3Prepare();
	b3_bool b3GetSurfaceValues(b3_surface *surface);
	b3_bool b3Illuminate(b3_surface *surface,b3_light_info *lit);

protected:
	b3_bool b3ShadeComponents(b3_surface *surface, b3_f64 reflection, b3_f64 refraction);

private:
	static inline void b3Randomize(const b3_vector64 *src,b3_vector64 *dst,b3_f64 scale,b3_f64 half)
	{
		dst->x = src->x + B3_FRAN(scale) - half;
		dst->y = src->y + B3_FRAN(scale) - half;
		dst->z = src->z + B3_FRAN(scale) - half;
		b3Vector::b3Normalize(dst);
	}
};

#define B3_MAT_CP_METALLIC 1

/*************************************************************************
**                                                                      **
**                        Thin film material                            **
**                                                                      **
*************************************************************************/

/**
 * This class simulates thin oil film surfaces.
 */
class B3_PLUGIN b3MatThinFilm : public b3Material, public b3_material, public b3Scaling
{
	static b3Color   m_WaveLength;
	static b3_vector m_ScaleTime;

public:
	b3_s32           m_Flags;
	b3_f32           m_Thickness; //!< Thickness in micro meter.
	b3Color          m_Intensity; //!< Color intensity filter.

public:
	B3_ITEM_INIT(b3MatThinFilm); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3MatThinFilm); //!< This constructor handles deserialization.

	void    b3Write();
	b3_bool b3Prepare();
	b3_bool b3GetSurfaceValues(b3_surface *surface);
};

#endif
