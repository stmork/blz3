/*
**
**	$Filename:	b3Scene.h $ 
**	$Release:	Dortmund 2004 $
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
#include "blz3/raytrace/b3CameraProjection.h"
#include "blz3/base/b3Procedure.h"
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
#define MORK2                   0x00004000      // Mork's enhanced illumination

#define TRACEANGLE_MORK         (CLASS_GLOBAL|TYPE_TRACEANGLE|MORK)
#define TRACEPHOTO_MORK         (CLASS_GLOBAL|TYPE_TRACEPHOTO|MORK)
#define TRACEPHOTO_MORK2        (CLASS_GLOBAL|TYPE_TRACEPHOTO|MORK2)
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
class b3CloudBackground;

class b3RayRow;

#define LENSFLARE_LOOP 6
#define LENSFLARE_RING 2

class B3_PLUGIN b3Scene : public b3Item
{
	static const b3_f64 m_Distances[LENSFLARE_LOOP];
	static const b3_f64 m_ResultWeights[LENSFLARE_LOOP];
	static const b3_f64 m_Exponents[LENSFLARE_LOOP];

	b3Base<b3Row>    m_RowPool;
	b3Base<b3Row>    m_TrashPool;
	b3PrepareInfo    m_PrepareInfo;
	b3_vector        m_NormWidth;
	b3_vector        m_NormHeight;
	b3_vector        m_ViewAxis;
	b3_f64           m_ViewAxisLen;
	b3Path           m_SceneName;
	b3Shader        *m_Shader;
	b3Path           m_Filename;
	b3Mutex          m_PoolMutex;
	b3Mutex          m_TrashMutex;
	b3Mutex          m_SamplingMutex;
	b3Distribute    *m_Distributed;
	b3Nebular       *m_Nebular;
	b3SuperSample   *m_SuperSample;
	b3LensFlare     *m_LensFlare;
	b3CameraPart    *m_ActualCamera;
	b3Clouds        *m_Clouds;
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
	static b3_s32    m_RenderPriority;

public:
	B3_ITEM_INIT(b3Scene);
	B3_ITEM_LOAD(b3Scene);

	virtual ~b3Scene();

	static  void             b3Register();
	        void             b3Write();
	        b3_bool          b3GetDisplaySize(b3_res &xSize,b3_res &ySize);

			// Drawing routines
		    void             b3SetupVertexMemory(b3RenderContext *context);
		    void             b3FreeVertices();
	        void             b3Draw(b3RenderContext *context);
			void             b3Update();
			void             b3RecomputeMaterial();
			void             b3UpdateMaterial();

			// Filename handling
	        char            *b3GetName();
			char            *b3GetFilename();
			void             b3SetFilename(const char *filename);
			void             b3SetTexture(const char *name);

			// Scene handling
		    b3_bool          b3PrepareScene(b3_res xSize,b3_res ySize);
		    void             b3Raytrace(b3Display *display);
		    void             b3AbortRaytrace();
		    inline b3_bool   b3Intersect(b3_ray *ray,b3_bool check_visibility,b3_f64 max = DBL_MAX)
			{
				ray->Q     = max;
				ray->shape = b3Intersect(b3GetFirstBBox(),ray,check_visibility);

				return ray->shape != null;
			}
			b3_bool          b3IsObscured(b3_ray *ray,b3_f64 max = DBL_MAX)
			{
				ray->Q     = max;
				ray->shape = b3IsObscured(b3GetFirstBBox(),ray);
				return ray->shape != null;
			}
	
			// shading
			inline b3Shader *b3GetShader()
			{
				return m_Shader;
			}
			void             b3SetShading(b3_u32 class_type);
		    void             b3GetBackgroundColor(b3_ray *ray,b3_f64 fx,b3_f64 fy);
		    void             b3MixLensFlare(b3_ray *ray);
		    void             b3GetInfiniteColor(b3_ray *ray);

			// object methods
			inline b3Base<b3Item> *b3GetBBoxHead()
			{
				return &m_Heads[0];
			}

			inline b3BBox  *b3GetFirstBBox()
			{
				return (b3BBox *)b3GetBBoxHead()->First;
			}

			void            b3Recount();
	        void            b3Reorg();
		    b3_count        b3GetBBoxCount();
		    b3_bool         b3ComputeBounds(b3_vector *lower,b3_vector *upper);
	        b3_bool         b3BacktraceRecompute(b3BBox *search);
		    b3Base<b3Item> *b3FindBBoxHead(b3BBox  *bbox);
		    b3BBox         *b3FindParentBBox(b3Shape *shape);
	        void            b3CollectBBoxes(b3_line64 *line,b3Array<b3BBox *> *array,b3_f64 max = DBL_MAX);
	        void            b3CollectBBoxes(b3_vector *lower,b3_vector *upper,b3Array<b3BBox *> *array);
			void            b3CollectActiveBBoxes(b3Array<b3BBox *> *array,b3_bool activation);
		    void            b3Activate(b3_bool activate=true);
		    void            b3Transform(b3_matrix *transformation,b3_bool is_affine = true,b3_bool force_action = false);
	        void            b3ComputeVisibility();

			// camera methods
		    b3CameraPart   *b3GetFirstCamera(b3_bool must_active = false);
	        b3CameraPart   *b3GetActualCamera();
			b3CameraPart   *b3GetCameraByName(const char *camera_name);
		    b3CameraPart   *b3GetNextCamera(b3CameraPart *act);
		    b3CameraPart   *b3UpdateCamera();
			b3_bool         b3GetTitle(char *title,size_t size);
			void            b3SetCamera(b3CameraPart *camera,b3_bool reorder=false);
	
			// light methods
			inline b3Base<b3Item> *b3GetLightHead()
			{
				return &m_Heads[1];
			}
			b3Light        *b3GetLight(b3_bool must_active = false);
			b3Light        *b3GetLightByName(const char *light_name);
			b3_count        b3GetLightCount();

			// Special access methods
			inline b3Base<b3Item> *b3GetSpecialHead()
			{
				return &m_Heads[2];
			}
			b3Animation       *b3GetAnimation(b3_bool force = false);
		    b3ModellerInfo    *b3GetModellerInfo();
			b3Distribute      *b3GetDistributed(b3_bool force = true);
		    b3Nebular         *b3GetNebular    (b3_bool force = true);
		    b3SuperSample     *b3GetSuperSample(b3_bool force = true);
		    b3LensFlare       *b3GetLensFlare  (b3_bool force = false);
			b3CloudBackground *b3GetCloudBackground(b3_bool force = false);

			// Animation
			void            b3SetAnimation (b3_f64 t);
			b3_f64          b3GetTimePoint();
	        void            b3ResetAnimation();
			void            b3Animate(b3Activation::b3_anim_activation activation);

	static  b3Scene        *b3ReadTGF(const char *filename);
	static  b3_bool         b3CheckTexture(b3Tx **tx,const char *name);
	static  b3_bool         b3CutTextureName(const char *full_name,char *short_name);
	static  b3_u32          b3PrepareBBoxThread(b3BBox *bbox,void *ptr);

private:
	        b3_bool         b3FindObscurer(b3_ray *ray,b3_f64 max = DBL_MAX);
			void            b3ReallocateShader();
	        void            b3DoRaytrace(b3Display *display,b3_count CPUs);
	        void            b3DoRaytraceMotionBlur(b3Display *display,b3_count CPUs);
	static  b3_u32          b3RaytraceThread(void *ptr);
	static  b3_u32          b3RaytraceMotionBlurThread(void *ptr);
	static  b3_u32          b3UpdateThread(           b3BBox *bbox,void *ptr);
	static  b3_u32          b3RecomputeMaterialThread(b3BBox *bbox,void *ptr);
		    b3Shape        *b3Intersect(    b3BBox *bbox,b3_ray *ray,b3_bool check_visibility);
	        b3Shape        *b3IsObscured(   b3BBox *bbox,b3_ray *ray);

	friend class b3Shader;
	friend class b3RayRow;
	friend class b3SupersamplingRayRow;
	friend class b3DistributedRayRow;
	friend class b3MotionBlurRayRow;
};

class B3_PLUGIN b3RayRow : public b3Row
{
protected:
	b3Display   *m_Display;
	b3Scene     *m_Scene;
	b3Shader    *m_Shader;
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
	virtual         ~b3RayRow() {}
	virtual void     b3Raytrace();

protected:
	        b3Color &b3Shade(b3_ray *ray, b3_f64 fx, b3_f64 fy);
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
	b3_bool                m_Debug;

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
