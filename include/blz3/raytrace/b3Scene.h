/*
**
**	$Filename:	b3Scene.h $ 
*	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing class definitions for root scene
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifndef B3_RAYTRACE_SCENE_H
#define B3_RAYTRACE_SCENE_H

#include "blz3/raytrace/b3Base.h"
#include "blz3/raytrace/b3Light.h"
#include "blz3/raytrace/b3PrepareInfo.h"
#include "blz3/base/b3Render.h"
#include "blz3/image/b3Tx.h"
#include "blz3/image/b3TxPool.h"
#include "blz3/system/b3Display.h"

/*************************************************************************
**                                                                      **
**                        global rendering informations                 **
**                                                                      **
*************************************************************************/

#define CLASS_GLOBAL            0x7ff00000
#define TYPE_TRACEANGLE         0x00000001      // raytracing (normal projection)
#define TYPE_TRACEPHOTO         0x00000002      // raytracing (obsolete)
#define TYPE_RADIOSITY          0x00000003      // radiosity
#define TYPE_REFLECTION         0x00000004      // reflection mapping
#define TYPE_GOROUD             0x00000005      // Gouraud shading
#define TYPE_GLOBAL_ILLUM       0x00000006      // global illumination

#define	MORK                    0x00001000      // Mork's illumination
#define	PHONG_ILLUM             0x00002000      // Phong's illumination
#define ALBRECHT                0x00003000      // Jens' coherence algo

#define TRACEANGLE_MORK         (CLASS_GLOBAL|TYPE_TRACEANGLE|MORK)
#define TRACEPHOTO_MORK         (CLASS_GLOBAL|TYPE_TRACEPHOTO|MORK)
#define TRACEANGLE_PHONG        (CLASS_GLOBAL|TYPE_TRACEANGLE|PHONG_ILLUM)
#define TRACEPHOTO_PHONG        (CLASS_GLOBAL|TYPE_TRACEPHOTO|PHONG_ILLUM)
#define TRACEPHOTO_ALBRECHT     (CLASS_GLOBAL|TYPE_TRACEPHOTO|ALBRECHT)
#define GLOBAL_ILLUM            (CLASS_GLOBAL|TYPE_GLOBAL_ILLUM|MORK)

#define TRACEPHOTO              TRACEPHOTO_MORK	// obsolete

// m_BackgroundType
enum b3_bg_type
{
	TP_NOTHING    = 0,            // Lightning background
	TP_TEXTURE    = 1,            // Background image
	TP_SLIDE      = 2,            // Background slide
	TP_SKY_N_HELL = 3             // Sky & hell
};

class b3Animation;
class b3ModellerInfo;
class b3Distribute;
class b3Nebular;
class b3SuperSample;
class b3LensFlare;
class b3CameraPart;

class b3RayRow;

class B3_PLUGIN b3Scene : public b3Item
{
	b3Base<b3Row>    m_RowPool;
	b3Base<b3Row>    m_TrashPool;
	b3PrepareInfo    m_PrepareInfo;
	b3_vector        m_NormWidth;
	b3_vector        m_NormHeight;
	b3Path           m_SceneName;

protected:
	b3Path           m_Filename;
	b3Mutex          m_PoolMutex;
	b3Mutex          m_TrashMutex;
	b3Mutex          m_SamplingMutex;
	b3Distribute    *m_Distributed;
	b3Nebular       *m_Nebular;
	b3SuperSample   *m_SuperSample;
	b3LensFlare     *m_LensFlare;
	b3CameraPart    *m_ActualCamera;
	b3_f64           m_ShadowFactor;        // Schattenhelligkeit
	b3Color          m_AvrgColor;
	b3Color          m_DiffColor;
	b3_vector64      m_xHalfDir;
	b3_vector64      m_yHalfDir;
	b3_vector64      m_xStepDir;
	b3_count         m_LightCount;

public:
	// Camera
	b3_vector        m_EyePoint;
	b3_vector        m_ViewPoint;
	b3_vector        m_Width;               // Bildschirmvektor X
	b3_vector        m_Height;              // Bildschirmvektor Y
	b3_res           m_xSize,m_ySize;       // Rechenaufloesung
	b3_f32           m_xAngle,m_yAngle;     // Blickwinkel

	// Background
	b3Color          m_TopColor;
	b3Color          m_BottomColor;
	b3Tx            *m_BackTexture;         //
	b3_bg_type       m_BackgroundType;      // Hintergrund: Farbe/Datei/...

	b3_count         m_TraceDepth;          // Rekursionstiefe
	b3_u32           m_Flags;               // beschreibt, welche Werte gueltig sind
	b3_f32           m_ShadowBrightness;

	// Some limits
	b3_f32           m_BBoxOverSize;        // BBox-Ueberziehung
	b3_f32           m_Epsilon;             // Schwellenwert
	char             m_TextureName[B3_TEXSTRINGLEN]; // Name des Hintergrundbildes

	static b3TxPool  m_TexturePool;
	static b3_f64    epsilon;

protected:
	B3_ITEM_BASE(b3Scene);

public:
	B3_ITEM_INIT(b3Scene);
	B3_ITEM_LOAD(b3Scene);

	static  void            b3Register();
	        void            b3Write();
	        void            b3Reorg();
	        b3_bool         b3GetDisplaySize(b3_res &xSize,b3_res &ySize);
	        char           *b3GetName();

			// Drawing routines
		    void            b3AllocVertices(b3RenderContext *context);
		    void            b3FreeVertices();
	virtual void            b3SetLights(b3RenderContext *context);
	        void            b3Draw(b3RenderContext *context);

			char           *b3GetFilename();
			void            b3SetFilename(const char *filename);
			void            b3SetTexture(const char *name);
			void            b3Update();
		    b3_bool         b3ComputeBounds(b3_vector *lower,b3_vector *upper);
	        b3_bool         b3BacktraceRecompute(b3BBox *search);
		    b3Base<b3Item> *b3FindBBoxHead(b3BBox  *bbox);
		    b3BBox         *b3FindParentBBox(b3Shape *shape);
			b3Animation    *b3GetAnimation(b3_bool force = false);
			b3_f64          b3GetTimePoint();
		    b3ModellerInfo *b3GetModellerInfo();
			b3Distribute   *b3GetDistributed(b3_bool force = true);
		    b3Nebular      *b3GetNebular    (b3_bool force = true);
		    b3SuperSample  *b3GetSuperSample(b3_bool force = true);
		    b3LensFlare    *b3GetLensFlare  (b3_bool force = false);
		    b3CameraPart   *b3GetCamera(b3_bool must_active = false);
	        b3CameraPart   *b3GetActualCamera();
			b3CameraPart   *b3GetCameraByName(const char *camera_name);
		    b3CameraPart   *b3GetNextCamera(b3CameraPart *act);
		    b3CameraPart   *b3UpdateCamera();
			b3_bool         b3GetTitle(char *title,size_t size);
			void            b3SetCamera(b3CameraPart *camera,b3_bool reorder=false);
		    b3Light        *b3GetLight(b3_bool must_active = false);
			b3Light        *b3GetLightByName(const char *light_name);
		    b3BBox         *b3GetFirstBBox();
		    b3_count        b3GetBBoxCount();
		    void            b3Activate(b3_bool activate=true);
		    void            b3Transform(b3_matrix *transformation,b3_bool is_affine = true,b3_bool force_action = false);
		    b3_bool         b3PrepareScene(b3_res xSize,b3_res ySize);
		    void            b3Raytrace(b3Display *display);
		    void            b3AbortRaytrace();
		    b3_bool         b3Intersect(b3_ray_info *ray,b3_f64 max = DBL_MAX);
			b3_bool         b3IsObscured(b3_ray_info *ray,b3_f64 max = DBL_MAX);
	virtual b3_bool         b3Shade(b3_ray_info *ray,b3_count depth = 0);
	virtual void            b3Illuminate(b3Light *light,b3_light_info *jit,b3_ray_fork *surface,b3Color &result,b3Material *material);
	virtual b3_bool         b3FindObscurer(b3_ray_info *ray,b3_f64 max = DBL_MAX);
	        void            b3CollectBBoxes(b3_line64 *line,b3Array<b3BBox *> *array,b3_f64 max = DBL_MAX);
	        void            b3CollectBBoxes(b3_vector *lower,b3_vector *upper,b3Array<b3BBox *> *array);
		    void            b3GetBackgroundColor(b3_ray_info *ray,b3_f64 fx,b3_f64 fy);

			void            b3SetAnimation (b3_f64 t);
	        void            b3ResetAnimation();
			void            b3Animate(b3Activation::b3_anim_activation activation);

			void            b3CollectActiveBBoxes(b3Array<b3BBox *> *array,b3_bool activation);
	static  b3Scene        *b3ReadTGF(const char *filename);

	inline b3Base<b3Item> *b3GetBBoxHead()
	{
		return &m_Heads[0];
	}

	inline b3Base<b3Item> *b3GetLightHead()
	{
		return &m_Heads[1];
	}

	inline b3Base<b3Item> *b3GetSpecialHead()
	{
		return &m_Heads[2];
	}

	static b3_bool   b3CheckTexture(b3Tx **tx,const char *name);
	static b3_bool   b3CutTextureName(const char *full_name,char *short_name);

protected:
		    b3_bool         b3ComputeOutputRays(b3_ray_fork *surface);
		    b3_f64          b3ComputeSpotExponent(b3Light *light);
		    void            b3GetInfiniteColor(b3_ray_info *ray);

private:
	        void            b3DoRaytrace(b3Display *display,b3_count CPUs);
	        void            b3DoRaytraceMotionBlur(b3Display *display,b3_count CPUs);
	static  b3_u32          b3RaytraceThread(void *ptr);
	static  b3_u32          b3RaytraceMotionBlurThread(void *ptr);
	static  b3_u32          b3PrepareThread(b3BBox *bbox,void *ptr);
	static  b3_u32          b3UpdateThread( b3BBox *bbox,void *ptr);
		    b3Shape        *b3Intersect(    b3BBox *bbox,b3_ray_info *ray);
		    b3Shape        *b3IsObscured(   b3BBox *bbox,b3_ray_info *ray);
		    void            b3MixLensFlare(b3_ray_info *ray);

	friend class b3RayRow;
	friend class b3SupersamplingRayRow;
	friend class b3DistributedRayRow;
	friend class b3MotionBlurRayRow;
};

class B3_PLUGIN b3ScenePhong : public b3Scene
{
public:
	B3_ITEM_INIT(b3ScenePhong);
	B3_ITEM_LOAD(b3ScenePhong);

	b3_bool b3Shade(b3_ray_info *ray,b3_count depth = 0);
	void    b3Illuminate(b3Light *light,b3_light_info *jit,b3_ray_fork *surface,b3Color &result,b3Material *material);
	b3_bool b3FindObscurer(b3_ray_info *ray,b3_f64 max = DBL_MAX);
};

class B3_PLUGIN b3SceneMork : public b3Scene
{
public:
	B3_ITEM_INIT(b3SceneMork);
	B3_ITEM_LOAD(b3SceneMork);

	virtual void     b3SetLights(b3RenderContext *context);
	        b3_bool  b3Shade(b3_ray_info *ray,b3_count depth = 0);
	        void     b3Illuminate(b3Light *light,b3_light_info *jit,b3_ray_fork *surface,b3Color &result,b3Material *material);

private:
	b3_bool b3IsPointLightBackground(b3Light *light,b3_ray_info *ray);
	void    b3LightFlare(b3_ray_info *ray);
};

class B3_PLUGIN b3RayRow : public b3Row
{
protected:
	b3Display   *m_Display;
	b3Scene     *m_Scene;
	b3Color     *m_LastResult;
	b3Color     *m_ThisResult;
	b3_vector64  m_preDir;
	b3_coord     m_y;
	b3_res       m_xSize;
	b3_res       m_ySize;
	b3_f64       m_fxStep;
	b3_f64       m_fy;
	b3_f64       m_t;

public:
	              b3RayRow(b3Scene *scene,b3Display *display,b3_coord y,b3_res xSize,b3_res ySize);
	virtual      ~b3RayRow() {}
	virtual void  b3Raytrace();
};

enum b3_row_state
{
	B3_STATE_NOT_STARTED,
	B3_STATE_COMPUTING,
	B3_STATE_CHECK,
	B3_STATE_REFINING,
	B3_STATE_READY
};

class B3_PLUGIN b3SupersamplingRayRow : public b3RayRow
{
	b3SupersamplingRayRow *m_PrevRow;
	b3SupersamplingRayRow *m_SuccRow;
	b3Color                m_Limit;
	b3_row_state           m_RowState;
public:
	              b3SupersamplingRayRow(b3Scene *scene,b3Display *display,b3_coord y,b3_res xSize,b3_res ySize,b3SupersamplingRayRow *previous);
	virtual      ~b3SupersamplingRayRow();
	virtual void  b3Raytrace();

private:
	b3_bool b3Test(b3_res x);
	void    b3Refine(b3_bool this_row);
	void    b3Convert();
};

class B3_PLUGIN b3DistributedRayRow : public b3RayRow
{
protected:
	b3Distribute *m_Distr;
	b3_count      m_SPP;
	b3_count      m_SPF;
	b3_vector64  *m_xHalfDir;
	b3_vector64  *m_yHalfDir;
	b3_f32       *m_Samples;

public:
	                b3DistributedRayRow(b3Scene *scene,b3Display *display,b3_coord y,b3_res xSize,b3_res ySize);
	virtual        ~b3DistributedRayRow();
	virtual void    b3Raytrace();
};

class B3_PLUGIN b3MotionBlurRayRow : public b3DistributedRayRow
{
	b3_index       *m_TimeIndex;
	b3_index        m_Index;
	b3_index        m_Modulo;
	b3_index        m_Start;
	b3Color        *m_Color;
	b3_vector64     m_BackupDir;

public:
	                b3MotionBlurRayRow(b3Scene *scene,b3Display *display,b3_coord y,b3_res xSize,b3_res ySize);
	virtual        ~b3MotionBlurRayRow();
	virtual void    b3Raytrace();
	        void    b3SetTimePoint(b3_f64 t);
};

// m_Flags
#define TP_SIZEVALID     2L            // SizeX, SizeY gueltig
#define TP_NO_GFX        4L            // no rgb output

#define TP_FRAME         0L            // use frame size
#define TP_FIELD_EVEN    8L            // even field
#define TP_FIELD_ODD    16L            // odd field
#define TP_FRAME_MASK   24L            // mask

#define RAY_INSIDE       1
#define RAY_OUTSIDE      0

class b3RaytracingItems
{
public:
	static void b3Register();
};

#endif
