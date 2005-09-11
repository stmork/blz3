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

#include "blz3/raytrace/b3Material.h"
#include "blz3/raytrace/b3Scene.h"
#include "blz3/base/b3Math.h"
#include "blz3/base/b3Procedure.h"

#define no_DEBUG_MATERIAL

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.103  2005/09/11 17:45:52  sm
**      - Fixed wrong ordered b3MatGranite read.
**
**      Revision 1.102  2004/12/30 16:27:39  sm
**      - Removed assertion problem when starting Lines III: The
**        image list were initialized twice due to double calling
**        OnInitDialog() of CDialogBar. The CDialogBar::Create()
**        calls OnInitDialog() automatically sinde MFC 7
**      - Removed many global references from raytrace and base lib
**      - Fixed ticket no. 29: The b3RenderObject::b3Recompute
**        method checks the vertex maintainer against a null pointer.
**
**      Revision 1.101  2004/10/09 06:56:07  sm
**      - Cook/Torrance delivers surface values again.
**
**      Revision 1.100  2004/10/07 12:09:43  sm
**      - Added new temporary variable for precomputation.
**
**      Revision 1.99  2004/10/07 10:33:08  sm
**      - Added some GIF tools and made them usable with Blizzard III.
**
**      Revision 1.98  2004/10/05 09:29:22  sm
**      - Donw some documentations.
**
**      Revision 1.97  2004/09/30 11:52:44  sm
**      - Adjusted edit field accuracy.
**      - Done some minor car paint tests.
**
**      Revision 1.96  2004/09/29 08:46:00  sm
**      - Added metallic effect on car paint.
**
**      Revision 1.95  2004/09/28 16:04:43  sm
**      - Fixed material ponter problem inside all shader.
**
**      Revision 1.94  2004/09/28 15:07:40  sm
**      - Support for car paint is complete.
**      - Made some optimizations concerning light.
**      - Added material dependend possibility for color
**        mixing instead of mixing inside shader.
**
**      Revision 1.93  2004/09/27 16:56:48  sm
**      - Minor changes
**
**      Revision 1.92  2004/09/27 15:24:23  sm
**      - Added car paint as material shader
**
**      Revision 1.91  2004/09/17 20:57:53  sm
**      - Material shader add their color components to jit.
**      - Grizzle fix to Mork 2 shader: The reflective and refractive color
**        is initialized when coefficents are zero.
**
**      Revision 1.90  2004/09/17 12:53:55  sm
**      - Changed chader signatures to sum on different color
**        channels (ambient, diffuse and specular). I wanted
**        to do this for a long time, puh!
**
**      Revision 1.89  2004/08/02 13:57:59  sm
**      - Changed thin film animation to closed spline computation.
**
**      Revision 1.88  2004/08/01 12:47:39  sm
**      - Animated thin film material.
**
**      Revision 1.87  2004/07/27 19:05:59  sm
**      - Some typo cleanups.
**
**      Revision 1.86  2004/07/27 17:33:46  sm
**      - Some thin film modifications
**
**      Revision 1.85  2004/07/27 16:33:50  sm
**      - Added thin film material rendering
**
**      Revision 1.84  2004/07/27 10:21:12  sm
**      - Added two new materials:
**        o car paint
**        o thin film interference
**
**      Revision 1.83  2004/06/22 12:35:42  sm
**      - Fixed ticket no. 25: Rounding problems at shadow edges forces
**        black borders on objects. Now safe implementations of asin/acos
**        added to b3Math.
**      - Fixed mail address in icc make script.
**
**      Revision 1.82  2004/06/18 12:03:12  sm
**      - Removed default values from some b3Color methods.
**
**      Revision 1.81  2004/05/29 13:38:11  sm
**      - Made shading model visible to material an bump dialogs.
**
**      Revision 1.80  2004/05/28 14:06:29  sm
**      - Minor optimizations in shader
**
**      Revision 1.79  2004/05/26 12:47:20  sm
**      - Optimized recursive shading
**      - Optimized pow to an integer version (b3Math::b3FastPow)
**
**      Revision 1.78  2004/05/23 20:52:34  sm
**      - Done some Fresnel formula experiments.
**
**      Revision 1.77  2004/05/22 14:17:31  sm
**      - Merging some basic raytracing structures and gave them some
**        self explaining names. Also cleaned up some parameter lists.
**
**      Revision 1.76  2004/05/12 14:13:28  sm
**      - Added bump dialogs:
**        o noise
**        o marble
**        o texture
**        o glossy
**        o groove
**        o water
**        o wave
**      - Setup all bump items with default values.
**      - Found bug 22 which show a camera deletion bug in
**        toolbar and camera property dialog.
**      - Added bump example bwd
**      - Recounted resource.h (full compile necessary)
**
**      Revision 1.75  2004/05/11 14:01:15  sm
**      - Added unified invert/revert for object editing.
**      - Added deletion of transform history in scene
**        editor (= transformed history) and object editor
**        (= original form)
**
**      Revision 1.74  2004/05/10 15:12:09  sm
**      - Unified condition legends for conditions and
**        texture materials.
**      - Added wrap texture material dialog.
**
**      Revision 1.73  2004/05/09 18:59:52  sm
**      - Fixed bump water.
**
**      Revision 1.72  2004/05/09 18:27:23  sm
**      - Fixed granite
**
**      Revision 1.71  2004/05/09 15:06:56  sm
**      - Added inverse transformation for mapping.
**      - Unified scale mapping source via b3Scaling.
**      - Moved b3Scaling in its own files.
**      - Added property pages for scaling and removed
**        scaling input fields from dialogs.
**
**      Revision 1.70  2004/05/08 17:36:39  sm
**      - Unified scaling for materials and bumps.
**
**      Revision 1.69  2004/05/07 16:30:33  sm
**      - Bug #13 fixed. The BBox hierarchy is recounted on every
**        object edit finish.
**      - Wooden materials contain a dark and a separate light
**        material. Changes were also made in Lines.
**      - Introduced shape property copy including all materials,
**        bumps and conditions. Multiple copy modes are possible.
**
**      Revision 1.68  2004/05/06 18:13:52  sm
**      - Added support for changed only b3Items for a
**        better preview performance.
**
**      Revision 1.67  2004/04/26 12:27:43  sm
**      - Added following dialogs:
**        o granite
**        o chess
**      - Added scaling to wood properties
**
**      Revision 1.66  2004/04/25 16:22:40  sm
**      - Added some colors for convenience.
**      - Added some initial values to materials.
**
**      Revision 1.65  2004/04/24 20:15:52  sm
**      - Further slide material dialog development
**
**      Revision 1.64  2004/04/24 15:40:12  sm
**      - Started slide material dialog implementation
**      - Added simple property sheet/preview dialog template
**
**      Revision 1.63  2004/04/23 11:09:04  sm
**      - Refectored b3Materials for better dialog use.
**
**      Revision 1.62  2004/04/18 16:58:14  sm
**      - Changed definitions for base classes of raytracing objects.
**      - Put wood material and wood bump dialogs into property
**        pages.
**
**      Revision 1.61  2004/04/18 09:17:34  sm
**      - Minor changes
**
**      Revision 1.60  2004/04/18 08:53:05  sm
**      - Put temporiraly some hardwired values into the oakplank.
**
**      Revision 1.59  2004/04/17 09:40:55  sm
**      - Splitting b3Raytrace.h into their components for
**        better oversightment.
**
**      Revision 1.58  2004/04/15 08:57:00  sm
**      - Added b3BumpAokPlank
**
**      Revision 1.57  2004/04/11 19:04:20  sm
**      - Renamed b3Material::b3GetColors into b3Material::b3GetSurfaceValues
**
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

b3_bool b3MatTexture::b3Prepare()
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

	surface->m_Diffuse = b3Color(m_Texture->b3GetValue(x,y));
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

b3_bool b3MatWrapTexture::b3Prepare()
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

	surface->m_Diffuse = b3Color(m_Texture->b3GetValue(x,y));
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

b3_bool b3MatMarble::b3Prepare()
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
	m_LightMaterial.m_Ambient = m_LightMaterial.m_Diffuse * 0.2;
	m_LightMaterial.m_Specular.b3Init(0.8f,0.8f,0.8f);

	m_DarkMaterial.m_Diffuse = m_LightMaterial.m_Diffuse * 0.7;
	m_DarkMaterial.m_Ambient = m_DarkMaterial.m_Diffuse * 0.2;
	m_DarkMaterial.m_Specular.b3Init(0.8f,0.8f,0.8f);

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

b3_bool b3MatWood::b3Prepare()
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

b3_bool b3MatOakPlank::b3Prepare()
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

b3_bool b3MatCookTorrance::b3Prepare()
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

b3_bool b3MatGranite::b3Prepare()
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

b3_bool b3MatCarPaint::b3Prepare()
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

b3_bool b3MatThinFilm::b3Prepare()
{
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
	b3Noise::b3AnimThinFilm(surface->m_Incoming->t,&shift);
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
