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
#include "blz3/raytrace/b3Activation.h"
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

/**
 * This enumeration lists the possible background types.
 */
enum b3_bg_type
{
	TP_NOTHING    = 0,            //!< Lightning background
	TP_TEXTURE    = 1,            //!< Background image
	TP_SLIDE      = 2,            //!< Background slide
	TP_SKY_N_HELL = 3             //!< Sky clouds.
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

/**
 * This class encapsulates a whole Blizzard III geometry.
 */
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
	b3_vector        m_EyePoint;            //!< Actual eye point.
	b3_vector        m_ViewPoint;           //!< Actual view point.
	b3_vector        m_Width;               //!< Horizontal projection plane vector.
	b3_vector        m_Height;              //!< Vertical projection plane vector.
	b3_res           m_xSize,m_ySize;       //!< Image resolution.
	b3_f32           m_xAngle,m_yAngle;     //!< Horizontal and vertical view angle.

	// Background
	b3Color          m_TopColor;            //!< Top background color.
	b3Color          m_BottomColor;         //!< Bottom background color.
	b3Tx            *m_BackTexture;         //!< The background texture.
	b3_bg_type       m_BackgroundType;      //!< The background type.

	b3_count         m_TraceDepth;          //!< The ray trace depth.
	b3_u32           m_Flags;               //!< 
	b3_f32           m_ShadowBrightness;    //!< Ambient term of Mork shading.

	// Some limits
	b3_f32           m_BBoxOverSize;        //!< Bounding box dimension tolarence
	b3_f32           m_Epsilon;             //!< Epsilon to use.
	char             m_TextureName[B3_TEXSTRINGLEN]; //!< Name of background image.

	static b3TxPool  m_TexturePool;                  //!< The image pool.
	static b3_f64    epsilon;                        //!< The near zero value.
	static b3_s32    m_RenderPriority;               //!< The render thread priority to use for raytracing.

public:
	B3_ITEM_INIT(b3Scene); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3Scene); //!< This constructor handles deserialization.

	/**
	 * This destructor deinitializes this scene instance.
	 */
	virtual ~b3Scene();

	/**
	 * Method for registering the shapes into the item registry.
	 */
	static  void             b3Register();
	        void             b3Write();

	/**
	 * This method returns the selected render image resolution.
	 *
	 * @param xSize Image width.
	 * @param ySize Image height.
	 * @return True on success.
	 */
	b3_bool          b3GetDisplaySize(b3_res &xSize,b3_res &ySize);

	//////////////////////////
	////////// Render handling
	//////////////////////////
	/** 
	 * This method iterates through all objects and shapes to setup
	 * vertex memory.
	 *
	 * @param context The render context to use.
	 * @see b3RenderObject::b3SetupVertexMemory().
	 */
	void             b3SetupVertexMemory(b3RenderContext *context);

	/**
	 * This method iterates through all objects and shapes to free
	 * the allocated vertex and index memory.
	 *
	 * @see b3RenderObject::b3FreeVertices().
	 */
	void             b3FreeVertices();

	/** 
	 * This method iterates through all objects and shapes to draw
	 * all render objects in its configured state.
	 *
	 * @param context The render context to use.
	 * @see b3RenderObject::b3Draw().
	 */
	void             b3Draw(b3RenderContext *context);

	/**
	 * This method iterates through all objects and shapes to update
	 * the render object data in case the data is invalidated through
	 * the b3RenderObject::b3Recompute() method.
	 *
	 * @see b3RenderObject::b3Update().
	 */
	void             b3Update();

	/**
	 * This method iterates through all objects and shapes and marks all
	 * material data as invalid for recomputing through the b3UpdateMaterial()
	 * method
	 *
	 * @see b3RenderObject::b3RecomputeMaterial().
	 */
	void             b3RecomputeMaterial();

	/**
	 * This method iterates through all objects and shapes to update
	 * the render object material in case the material is invalidated through
	 * the b3RenderObject::b3RecomputeMaterial() method.
	 *
	 * @see b3RenderObject::b3UpdateMaterial().
	 */
	void             b3UpdateMaterial();

	////////////////////////////
	////////// Filename handling
	////////////////////////////
	/**
	 * This method returns the scene name. The scene name is computed from
	 * the file name. The extension and the directory part is removed.
	 *
	 * @return The scene name.
	 */
	char            *b3GetName();

	/**
	 * This method returns the file name of this scene.
	 *
	 * @return The used file name of this scene.
	 */
	char            *b3GetFilename();

	/**
	 * This method sets a new file name of the scene.
	 *
	 * @param filename The new file name.
	 */
	void             b3SetFilename(const char *filename);

	/**
	 * This method sets a new background image file name.
	 *
	 * @param name The new file name of the background image.
	 */
	void             b3SetTexture(const char *name);

	/////////////////////////
	////////// Scene handling
	/////////////////////////
	/**
	 * This method prepares the scene for raytracing into an image of the
	 * given resolution,
	 *
	 * @param xSize The image width.
	 * @param ySize The image height.
	 * @throws b3PrepareException
	 * @return True on success.
	 */
	b3_bool          b3PrepareScene(b3_res xSize,b3_res ySize) throw(b3PrepareException);

	/**
	 * This method simply raytraces the scene onto the given display.
	 *
	 * @param display The display to raytrace into.
	 */
	void             b3Raytrace(b3Display *display);

	/**
	 * This method aborts an active raytrace process.
	 */
	void             b3AbortRaytrace();

	/**
	 * This method computes the nearest intersection point to the rays base
	 * position.
	 *
	 * @param ray The ray for intersection testing.
	 * @param checkVisibility If true use camera visibility optimization.
	 * @param max The maximum length of the ray to test.
	 * @return True when any intersection point was found.
	 */
	inline b3_bool   b3Intersect(b3_ray *ray,b3_bool checkVisibility,b3_f64 max = DBL_MAX)
	{
		ray->Q     = max;
		ray->shape = b3Intersect(b3GetFirstBBox(),ray,checkVisibility);

		return ray->shape != null;
	}
	/**
	 * This method checks whether any intersection point is available on the
	 * given ray. This is for shadow testing purposes which don't need any
	 * surface values.
	 *
	 * @param ray The ray to test the occluding.
	 * @param max The maximum length of the ray to test.
	 * @return True when any intersection point was found.
	 */
	b3_bool          b3IsObscured(b3_ray *ray,b3_f64 max = DBL_MAX)
	{
		ray->Q     = max;
		ray->shape = b3IsObscured(b3GetFirstBBox(),ray);
		return ray->shape != null;
	}
	
	//////////////////
	////////// Shading
	//////////////////
	/**
	 * This method returns the configured shader instance.
	 *
	 * @return The configured shader instance.
	 */
	inline b3Shader *b3GetShader()
	{
		return m_Shader;
	}

	/**
	 * This method setup new shading. The shader is instanciated depending on
	 * the given class type.
	 *
	 * @param classType The class type which defines the shader.
	 */
	void             b3SetShading(b3_u32 classType);

	/**
	 * This method copmutes the infinite background color depending on
	 * the specified background type.
	 *
	 * @param ray The shooting ray which runs into infinity.
	 * @param fx The relative x coordinate of the result image in the range [-1..1].
	 * @param fy The relative y coordinate of the result image in the range [-1..1].
	 */
	void             b3GetBackgroundColor(b3_ray *ray,b3_f64 fx,b3_f64 fy);

	/**
	 * This method mixes additional lens flares into the result color if
	 * lens flares are configured.
	 *
	 * @param ray The shooting ray
	 * @see b3_ray
	 */
	void             b3MixLensFlare(b3_ray *ray);

	/**
	 * This method computes the infinite color in case the ray depth is not
	 * at top level. In such a case the image coordinates must be computed
	 * otherwise. This method calls the b3GetBackgroundColor() method.
	 *
	 * @param ray The infinite ray.
	 * @see b3GetBackgroundColor().
	 */
	void             b3GetInfiniteColor(b3_ray *ray);

	/**
	 * This method returns the list base of the top level objects.
	 *
	 * @return The list base of the top level objects.
	 */
	inline b3Base<b3Item> *b3GetBBoxHead()
	{
		return &m_Heads[0];
	}

	/**
	 * This method returns the first object.
	 *
	 * @return The first object in the scene.
	 */
	inline b3BBox  *b3GetFirstBBox()
	{
		return (b3BBox *)b3GetBBoxHead()->First;
	}

	/**
	 * This method recounts the object hierarchy depth of the scene
	 * hierarchy. Every object gets the hierarchy depth in its type
	 * part of the class type field.  This method call is necessary
	 * before serializing the scene so that the hierarchy can be
	 * reconstructed during deserialization.
	 *
	 * @see b3Reorg()
	 * @see b3World::b3Write()
	 */
	void            b3Recount();

	/**
	 * This method reorganizes the object hierarchy after deserialization.
	 *
	 * @see b3Recount()
	 * @see b3World::b3Read()
	 */
	void            b3Reorg();

	/**
	 * This method returns the object count.
	 *
	 * @return The object count in the scene.
	 */
	b3_count        b3GetBBoxCount();

	/**
	 * This method recursively recomputes the scenes bounding box.
	 *
	 * @param lower The resulting lower corner of the scene bounding box.
	 * @param upper The resulting upper corner of the scene bounding box.
	 * @return True on success.
	 */
	b3_bool         b3ComputeBounds(b3_vector *lower,b3_vector *upper);

	/**
	 * This method invalidates all objects beginning at the given object
	 * up in the hierarchy to the root object.
	 *
	 * @param search The leaf element to invalidate.
	 * @return True on success.
	 * @see b3RenderObject::b3Recompute()
	 * @warning This method may be slow. It is ment for GUI purposes only.
	 */
	b3_bool         b3BacktraceRecompute(b3BBox *search);

	/**
	 * This method searches recursively in the object hierarchy the list base
	 * of the given object.
	 *
	 * @param bbox The object to search for the object list base it belongs to.
	 * @return The belonging object list base.
	 * @warning This method may be slow. It is ment for GUI purposes only.
	 */

	b3Base<b3Item> *b3FindBBoxHead(b3BBox  *bbox);
	/**
	 * this method searches in the object hierarchy the hosting object of the
	 * given shape.
	 *
	 * @param shape The shape to search for the object it belongs to.
	 * @return The belonging object.
	 * @warning This method may be slow. It is ment for GUI purposes only.
	 */
	b3BBox         *b3FindParentBBox(b3Shape *shape);

	/**
	 * This method collects all objects which tangents the given ray.
	 *
	 * @param line The ray to shoot.
	 * @param array The array which gets the object pointers.
	 * @param max The maximum distance of the ray direction.
	 */
	void            b3CollectBBoxes(b3_line64 *line,b3Array<b3BBox *> *array,b3_f64 max = DBL_MAX);

	/**
	 * This method collects all objects in the given bounding box.
	 *
	 * @param lower The lower corner of the bounding box.
	 * @param upper The upper corner of the bounding box.
	 * @param array  The array which gets the object pointers.
	 */
	void            b3CollectBBoxes(b3_vector *lower,b3_vector *upper,b3Array<b3BBox *> *array);

	/**
	 * This method collects all objects which are in the given activation state.
	 *
	 * @param array The array which gets the object pointers.
	 * @param activation The activation state.
	 */
	void            b3CollectActiveBBoxes(b3Array<b3BBox *> *array,b3_bool activation);

	/**
	 * This method sets the hole hierarchy into the given state.
	 *
	 * @param activate The activation state.
	 */
	void            b3Activate(b3_bool activate=true);

	/**
	 * This method transforms objects in the hierarchy depending on the given states.
	 *
	 * @param transformation The transformation matrix.
	 * @param isAffine This flag signals if the transformation is affine. In this case the vertex
	 * normals can be transformed, too. Otherwise the normals must be recomputed.
	 * @param forceAction Force transformation and ignore activation state of object.
	 */
	void            b3Transform(b3_matrix *transformation,b3_bool isAffine = true,b3_bool forceAction = false);

	/**
	 * This method recomputes for all objects the visibility in conjunction with
	 * the actual camera.
	 */
	void            b3ComputeVisibility();

	/////////////////////////
	////////// camera methods
	/////////////////////////
	/**
	 * This method returns the first camera. If the given
	 * must active flag is set the method returns the first
	 * active camera if any.
	 *
	 * @param mustActive Force first camera to be active.
	 * @return The first camera.
	 */
	b3CameraPart   *b3GetFirstCamera(b3_bool mustActive = false);

	/**
	 * This method returns the actually selected camera.
	 *
	 * @return The actually selected camera.
	 * @see b3SetCamera().
	 */
	b3CameraPart   *b3GetActualCamera();

	/**
	 * This method returns the first camera which matches
	 * the given camera name. The name comparison is case insensitive.
	 *
	 * @param cameraName The camera name to search for.
	 * @return The found camera if any.
	 */
	b3CameraPart   *b3GetCameraByName(const char *cameraName);

	/**
	 * This method returns the next camera outgoing from the given camera.
	 *
	 * @param act The camera position to search from.
	 * @return The next camera.
	 */
	b3CameraPart   *b3GetNextCamera(b3CameraPart *act);

	/**
	 * This method copies the values from the actual set camera
	 * into the scene copies.
	 */
	b3CameraPart   *b3UpdateCamera();

	/**
	 * This method returns the actual camera title. If the actual camera
	 * is defined and its title has a length greater than zero this one
	 * is used. If no actual camera is defined the scene file name is
	 * returned.
	 *
	 * @param title The buffer which receives the camera name.
	 * @param size The buffer size of the retrieving camera name.
	 * @return True if the result name has a length greater than zero.
	 */
	b3_bool         b3GetTitle(char *title,size_t size);

	/**
	 * This method sets a new actual camera. An additional flag signals if
	 * the camera should in front of the special list. This may accelerate
	 * access via the b3GetCameraByName() method.
	 *
	 * @param camera The camera to set as actual camera.
	 * @param reorder A flag which signals if the camera should be put in front
	 * of all other cameras.
	 */
	void            b3SetCamera(b3CameraPart *camera,b3_bool reorder=false);
	
	////////////////////////
	////////// Light methods
	////////////////////////
	/**
	 * This method returns the list base of all light sources.
	 *
	 * @return The list base of the light sources.
	 */
	inline b3Base<b3Item> *b3GetLightHead()
	{
		return &m_Heads[1];
	}

	/**
	 * This method returns the first light source. If the given
	 * must active flag is set the method returns the first
	 * active light source if any.
	 *
	 * @param mustActive Force first light source to be active.
	 * @return The first light source.
	 */
	b3Light        *b3GetLight(b3_bool mustActive = false);

	/**
	 * This method returns the first light source which matches
	 * the given light name. The name comparison is case insensitive.
	 *
	 * @param lightName The light name to search for.
	 * @return The found light source if any.
	 */
	b3Light        *b3GetLightByName(const char *lightName);

	/**
	 * This method returns the light source count.
	 *
	 * @return The amount of light sources.
	 */
	b3_count        b3GetLightCount();

	/////////////////////////////////
	////////// Special access methods
	/////////////////////////////////
	/**
	 * This method returns the list base of global special effects.
	 *
	 * @return The list base of special effects.
	 */
	inline b3Base<b3Item> *b3GetSpecialHead()
	{
		return &m_Heads[2];
	}

	/**
	 * This method returns the animation definition.
	 * If there is no animation definition found and the given force flag is set
	 * a new one is returned. This animation class is the root for
	 * all animation elements.
	 *
	 * @param force If a animation instance is forced.
	 * @return The animation definition.
	 * @see b3AnimElement.
	 */
	b3Animation       *b3GetAnimation(b3_bool force = false);

	/**
	 * This method returns a modeller helper class instance. If no instance
	 * was found a new one is created.
	 *
	 * @return The modeller helper information instance.
	 */
	b3ModellerInfo    *b3GetModellerInfo();

	/**
	 * This method returns the distributed raytracing special effect definition.
	 * If there is no distributed raytracing definition found and the given force flag is set
	 * a new one is returned.
	 *
	 * @param force If a distributed raytracing instance is forced.
	 * @return The distributed raytracing definition.
	 */
	b3Distribute      *b3GetDistributed(b3_bool force = true);

	/**
	 * This method returns the nebular special effect definition.
	 * If there is no nebular definition found and the given force flag is set
	 * a new one is returned.
	 *
	 * @param force If a nebular instance is forced.
	 * @return The nebular definition.
	 */
	b3Nebular         *b3GetNebular    (b3_bool force = true);

	/**
	 * This method returns the super sampling definition.
	 * If there is no super sampling definition found and the given force flag is set
	 * a new one is returned.
	 *
	 * @param force If a super sampling instance is forced.
	 * @return The super sampling definition.
	 */
	b3SuperSample     *b3GetSuperSample(b3_bool force = true);

	/**
	 * This method returns the lens flare special effect definition.
	 * If there is no lens flare definition found and the given force flag is set
	 * a new one is returned.
	 *
	 * @param force If a lens flare instance is forced.
	 * @return The lens flare definition.
	 */
	b3LensFlare       *b3GetLensFlare  (b3_bool force = false);

	/**
	 * This method returns the cloud background special effect definition.
	 * If there is no cloud definition found and the given force flag is set
	 * a new one is returned.
	 *
	 * @param force If an cloud background instance is forced.
	 * @return The cloud background definition.
	 */
	b3CloudBackground *b3GetCloudBackground(b3_bool force = false);

	////////////////////
	////////// Animation
	////////////////////
	/**
	 * This method sets the global animation time point.
	 *
	 * @param t The new animation time point to move to.
	 */
	void            b3SetAnimation (b3_f64 t);

	/**
	 * This method returns the actual animation time point.
	 *
	 * @return The actual animation time point.
	 */
	b3_f64          b3GetTimePoint();

	/**
	 * This method resets the animation to the neutral position time point.
	 */
	void            b3ResetAnimation();

	/**
	 * This method animates recursively the scene.
	 *
	 * @param activation The activation state.
	 */
	void            b3Animate(b3Activation::b3_anim_activation activation);

	/**
	 * This method reads a TGF scene and converts it into a Blizzard III scene.
	 *
	 * @param filename The file name of the TGF file.
	 * @return The converted Blizzard III scene.
	 */
	static  b3Scene        *b3ReadTGF(const char *filename);

	/**
	 * This method checks the given image file name for existance in the
	 * image pool and setups the given pointer reference. This is for
	 * preparing surface properties for raytracing such as texture mapping
	 * or bump mapping.
	 *
	 * @param tx The pointer to a pointer which receives the image reference.
	 * @param name The image name to search for.
	 * @return True on success.
	 */
	static  b3_bool         b3CheckTexture(b3Tx **tx,const char *name);

	/**
	 * This method shortens the given full qualified file name to a short
	 * name which satifies the search path of the scene image pool.
	 *
	 * @param fullname  The full qualified image file name.
	 * @param shortname The shortened image file name.
	 * @return True on success.
	 * @see b3ImagePool::b3CutName().
	 */
	static  b3_bool         b3CutTextureName(const char *fullname,char *shortname);

	/**
	 * This is a multi threading call back function which simply prepares
	 * the given object.
	 *
	 * @param bbox The object to prepare.
	 * @param ptr An additional custom information pointer.
	 * @return The result of the b3BBox::b3Prepare() method.
	 */
	static  b3_u32          b3PrepareBBoxThread(b3BBox *bbox,void *ptr);

private:
	        b3_bool         b3FindObscurer(b3_ray *ray,b3_f64 max = DBL_MAX);
			void            b3ReallocateShader();
	        void            b3DoRaytrace(b3Display *display,b3_count CPUs) throw(b3PrepareException);
	        void            b3DoRaytraceMotionBlur(b3Display *display,b3_count CPUs) throw(b3PrepareException);
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

/**
 * This class represents one raytracing row to render.
 *
 * @see b3Row.
 * @see b3Display
 */
class B3_PLUGIN b3RayRow : public b3Row
{
protected:
	b3Display   *m_Display;      //!< The used b3Display.
	b3Scene     *m_Scene;        //!< The used b3Scene.
	b3Shader    *m_Shader;       //!< The used b3Shader.
	b3Color     *m_LastResult;   //!< The position of the previous color in row.
	b3Color     *m_ThisResult;   //!< The position of the actual color.
	b3_vector64  m_preDir;
	b3_coord     m_y;            //!< The row's y coordinate
	b3_res       m_xSize;        //!< The resulting image width.
	b3_res       m_ySize;        //!< The resulting image height.
	b3_f64       m_fxStep;       //!< The step width in camera projection plane vector values.
	b3_f64       m_fy;           //!< 
	b3_f64       m_t;            //!< The actual time point.

public:
	/**
	 * This constructor initializes this row.
	 *
	 * @param scene The scene to raytrace.
	 * @param display The display to render in.
	 * @param y The y coordinate in image coordinates.
	 * @param xSize The image width.
	 * @param ySize The image height.
	 */
	b3RayRow(b3Scene *scene,b3Display *display,b3_coord y,b3_res xSize,b3_res ySize);

	/**
	 * This destructor deinitializes this row instance.
	 */
	virtual         ~b3RayRow() {}

	/**
	 * This method simply raytraces this row.
	 */
	virtual void     b3Raytrace();

protected:
	/**
	 * This method shoots one ray which results in a color.
	 *
	 * @param ray The ray to shoot.
	 * @param fx The projection plane x coordinate.
	 * @param fy The projection plane y coordinate.
	 * @return The raytraced color.
	 */
	b3Color &b3Shade(b3_ray *ray, b3_f64 fx, b3_f64 fy);
};

/**
 * This enumeration lists the row states for super sampling. This is
 * necessary because of multi threading in SMP environments where it
 * can happen, that the prevouis row is not earlier ready than the
 * actual one.
 */
enum b3_row_state
{
	B3_STATE_NOT_STARTED, //!< Nothing raytraced, yet.
	B3_STATE_COMPUTING,   //!< Actually raytracing.
	B3_STATE_CHECK,       //!< Row needs check.
	B3_STATE_REFINING,    //!< Actually refining.
	B3_STATE_READY        //!< Raytracing complete.
};

/**
 * This class handles super sampling depending on high contrast.
 */
class B3_PLUGIN b3SupersamplingRayRow : public b3RayRow
{
	b3SupersamplingRayRow *m_PrevRow;
	b3SupersamplingRayRow *m_SuccRow;
	b3Color                m_Limit;
	b3_row_state           m_RowState;
	b3_bool                m_Debug;

public:
	/**
	 * This constructor initializes this row instance.
	 *
	 * @param scene The scene to raytrace.
	 * @param display The display to render in.
	 * @param y The y coordinate in image coordinates.
	 * @param xSize The image width.
	 * @param ySize The image height.
	 * @param previous The super sampling row directly above this one.
	 */
	b3SupersamplingRayRow(b3Scene *scene,b3Display *display,b3_coord y,b3_res xSize,b3_res ySize,b3SupersamplingRayRow *previous);

	/**
	 * This destructor deinitializes this row instance.
	 */
	virtual      ~b3SupersamplingRayRow();

	virtual void  b3Raytrace();

private:
	b3_bool b3Test(b3_res x);
	void    b3Refine(b3_bool this_row);
	void    b3Convert();
};

/**
 * This class provides the sampling for distributed raytracing.
 */
class B3_PLUGIN b3DistributedRayRow : public b3RayRow
{
protected:
	b3Distribute *m_Distr;     //!< The class which describes distributed raytracing.
	b3_count      m_SPP;       //!< Samples per pixel.
	b3_count      m_SPF;       //!< Samples per frame.
	b3_vector64  *m_xHalfDir;  //!< The half width of a pixel in world coordinates.
	b3_vector64  *m_yHalfDir;  //!< The half height of a pixel in world coordinates.
	b3_f32       *m_Samples;   //!< The sampling offsets provided by the b3Distribute::m_Samples member.

public:
	/**
	 * This constructor initializes this row instance for distributed raytracing.
	 *
	 * @param scene The scene to raytrace.
	 * @param display The display to render in.
	 * @param y The y coordinate in image coordinates.
	 * @param xSize The image width.
	 * @param ySize The image height.
	 */
	b3DistributedRayRow(b3Scene *scene,b3Display *display,b3_coord y,b3_res xSize,b3_res ySize);

	/**
	 * This destructor deinitializes this row instance.
	 */
	virtual        ~b3DistributedRayRow();

	virtual void    b3Raytrace();
};

/**
 * This class provides the sampling for motion blur.
 */
class B3_PLUGIN b3MotionBlurRayRow : public b3DistributedRayRow
{
	b3_index       *m_TimeIndex;
	b3_index        m_Index;
	b3_index        m_Modulo;
	b3_index        m_Start;
	b3Color        *m_Color;
	b3_vector64     m_BackupDir;

public:
	/**
	 * This constructor initializes this row instance for motion blur.
	 *
	 * @param scene The scene to raytrace.
	 * @param display The display to render in.
	 * @param y The y coordinate in image coordinates.
	 * @param xSize The image width.
	 * @param ySize The image height.
	 * @throws b3WorldException
	 */
	b3MotionBlurRayRow(b3Scene *scene,b3Display *display,b3_coord y,b3_res xSize,b3_res ySize) throw(b3WorldException);

	/**
	 * This destructor deinitializes this row instance.
	 */
	virtual        ~b3MotionBlurRayRow();

	/**
	 * This method sets the actual time point for the animation which is needed
	 * for motion blur.
	 *
	 * @param t The actual time point.
	 */
	void    b3SetTimePoint(b3_f64 t);

	virtual void    b3Raytrace();
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

/**
 * This class provides the entry point for registering all raytracing b3Itmes.
 *
 * @see b3Item
 * @see b3World
 */
class b3RaytracingItems
{
public:
	/**
	 * This method registers all raytracing items for serialization.
	 */
	static void b3Register();
};

#endif
