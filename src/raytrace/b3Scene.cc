/*
**
**      $Filename:      b3Scene.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing root scene
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

#include "blz3/raytrace/b3Scene.h"
#include "b3ReadTGF.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.62  2004/05/20 19:10:30  sm
**	- Separated shader from scene. this is easier
**	  to handle.
**
**	Revision 1.61  2004/05/19 15:35:03  sm
**	- Hope of having fixed ticket no. 13.
**	
**	Revision 1.60  2004/05/15 14:37:46  sm
**	- Added resolution combo box to scene dialog.
**	- Fixed bug no. 3
**	
**	Revision 1.59  2004/05/13 16:17:26  sm
**	- Added background clouds as special item.
**	
**	Revision 1.58  2004/05/12 16:28:16  sm
**	- Beautified bump icons
**	- Missing return type for b3Material::b3Mix added
**	- Fixed bug 23 concerning camera title cropping.
**	
**	Revision 1.57  2004/05/05 16:32:26  sm
**	- Fixing following bugs:
**	  o #19 because of variable shadowing
**	  o #20 is no bug due to a KILLFOCUS event. This
**	    is still a perfomrmance problem.
**	  o #17 fixed. Now we use b3Scene::b3GetName() in
**	    combination with the b3Scene filename.
**	
**	Revision 1.56  2004/05/05 15:41:17  sm
**	- Working on bug 17.
**	
**	Revision 1.55  2004/04/17 09:40:55  sm
**	- Splitting b3Raytrace.h into their components for
**	  better oversightment.
**	
**	Revision 1.54  2004/03/02 09:07:17  sm
**	- Added read/write support for Cook/Torrance material.
**	- Added test module for Cook/Torrance reflection model.
**	- Fixed camera name handling if camera name is empty.
**	
**	Revision 1.53  2004/01/18 13:51:57  sm
**	- Done further security issues.
**	
**	Revision 1.52  2003/08/31 10:44:07  sm
**	- Further buffer overflow avoidments.
**	
**	Revision 1.51  2003/07/26 14:03:14  sm
**	- Fixed ICC version: The b3Vector classes computed a wrong value
**	  in b3Length() because of using the uninitialized fourth vector
**	  component.
**	
**	Revision 1.50  2003/07/12 17:44:47  sm
**	- Cleaned up raytracing b3Item registration
**	
**	Revision 1.49  2003/07/09 10:09:38  sm
**	- Changed brt3's default image file format to JPEG
**	- Increased default quality of JPEG images from 75 to 85
**	- Added house creating support ;-)
**	
**	Revision 1.48  2003/03/04 20:37:39  sm
**	- Introducing new b3Color which brings some
**	  performance!
**	
**	Revision 1.47  2003/02/17 16:57:46  sm
**	- Inlining head pointer computation.
**	
**	Revision 1.46  2003/02/05 18:42:32  sm
**	- Changed TGF to scene/bbox import
**	- Resorted some menus
**	- Added TGF import to Un*x makefile
**	
**	Revision 1.45  2003/02/02 14:22:32  sm
**	- Added TGF import facility.
**	
**	Revision 1.44  2002/08/23 15:34:28  sm
**	- Added time support to water animation.
**	- Added multiple file format types to brt3.
**	
**	Revision 1.43  2002/08/23 12:37:11  sm
**	- Optimized motion blur raytracing using their own thread methods...
**	
**	Revision 1.42  2002/08/23 11:35:23  sm
**	- Added motion blur raytracing. The image creation looks very
**	  nice! The algorithm is not as efficient as it could be.
**	
**	Revision 1.41  2002/08/21 10:16:40  sm
**	- Made some changes to the Un*x OpenGL renderer:
**	  o Added animations
**	  o Added camera switching
**	  o Corrected far clipping plane computation.
**	- Configure script tidied up.
**	
**	Revision 1.40  2002/08/19 16:50:39  sm
**	- Now having animation running, running, running...
**	- Activation handling modified to reflect animation
**	  and user transformation actions.
**	- Made some architectual redesigns.
**	
**	Revision 1.39  2002/08/18 13:05:17  sm
**	- First try to animate. We have to relink the control points which
**	  are stored in separate Blizzard classes to the b3AnimElement
**	  class.
**	
**	Revision 1.38  2002/08/17 17:31:23  sm
**	- Introduced animation support (Puh!)
**	
**	Revision 1.37  2002/07/26 09:13:33  sm
**	- Found alpha problem: the Linux OpenGL renderer didn't use the
**	  b3RenderContext::b3Init() method! Now everything function very well:-)
**	- The Un*x OpenGL renderer has got a key press interface now.
**	- Corrected spot lights
**	- Textures needn't to be square any more (some less memory usage)
**	
**	Revision 1.36  2002/07/25 13:22:32  sm
**	- Introducing spot light
**	- Optimized light settings when drawing
**	- Further try of stencil maps
**	
**	Revision 1.35  2002/07/21 17:02:36  sm
**	- Finished advanced color mix support (correct Phong/Mork shading)
**	- Added first texture mapping support. Further development on
**	  Windows now...
**	
**	Revision 1.34  2002/03/03 21:22:22  sm
**	- Added support for creating surfaces using profile curves.
**	- Added simple creating of triangle fields.
**	
**	Revision 1.33  2002/02/20 20:23:57  sm
**	- Some type cleanups done.
**	
**	Revision 1.32  2002/01/02 15:48:37  sm
**	- Added automated expand/collapse to hierarchy tree.
**	
**	Revision 1.31  2001/12/31 11:05:18  sm
**	- Added TestData for testing Blizzard data structures for reading
**	  and writing.
**	- Fixed bugs found with previous mentioned tool:
**	  o Some b3AnimElement errors found to be fixed under Windows.
**	  o b3TriangleShape destructor handled unchecked m_GridList pointer
**	- Changed some output levels in b3Light, b3Scene and b3ShadeXXX from
**	  B3LOG_NORMAL to B3LOG_DEBUG.
**	
**	Revision 1.30  2001/12/30 22:52:35  sm
**	- Made b3Scene::b3SetCamera() compatible to earlier versions.
**	
**	Revision 1.29  2001/12/30 18:24:35  sm
**	- Added missing b3AnimControl class
**	- Some minor bug fixes done:
**	  o Missed some SetModifiedFlag()
**	  o b3Scene::b3SetCamera() calls added which now puts the
**	    selected camera in front of the b3Special list so that Lines III
**	    select it when reloading.
**	
**	Revision 1.28  2001/12/30 14:16:58  sm
**	- Abstracted b3File to b3FileAbstract to implement b3FileMem (not done yet).
**	- b3Item writing implemented and updated all raytracing classes
**	  to work properly.
**	- Cleaned up spline shapes and CSG shapes.
**	- Added b3Caustic class for compatibility reasons.
**	
**	Revision 1.27  2001/12/02 17:38:17  sm
**	- Removing nasty CR/LF
**	- Added b3ExtractExt()
**	- Added stricmp() for Un*x
**	- Fixed some defines
**	
**	Revision 1.26  2001/12/02 15:43:49  sm
**	- Creation/Deletion/Editing of lights
**	- Creation/Deletion of cameras
**	- New toolbars introduced.
**	
**	Revision 1.25  2001/11/26 17:16:37  sm
**	- Linux b3TimeSpan fix
**	
**	Revision 1.24  2001/11/25 12:25:31  sm
**	- Completing some dialogs:
**	  o super sampling
**	  o distributed raytracing
**	
**	Revision 1.23  2001/11/12 16:50:29  sm
**	- Scene properties dialog coding
**	
**	Revision 1.22  2001/11/08 19:31:33  sm
**	- Nasty CR/LF removal!
**	- Added TGA/RGB8/PostScript image saving.
**	- Hoping to win Peter H. for powerful MFC programming...
**	
**	Revision 1.21  2001/11/05 16:57:39  sm
**	- Creating demo scenes.
**	- Initializing some b3Item derived objects
**	
**	Revision 1.20  2001/11/03 16:24:16  sm
**	- Added scene property dialog
**	- Added raytrace view title
**	- Added raytrace abort on button press
**	
**	Revision 1.19  2001/11/01 09:43:11  sm
**	- Some image logging cleanups.
**	- Texture preparing now in b3Prepare().
**	- Done some minor fixes.
**	
**	Revision 1.18  2001/10/21 16:55:20  sm
**	- Introducing lens flares.
**	- Introducing different modes of background computation.
**	- Introducing different types of row sampling.
**	
**	Revision 1.17  2001/10/19 14:46:57  sm
**	- Rotation spline shape bug found.
**	- Major optimizations done.
**	- Cleanups
**	
**	Revision 1.16  2001/10/17 14:46:02  sm
**	- Adding triangle support.
**	- Renaming b3TriangleShape into b3Triangles and introducing
**	  new b3TriangleShape as base class. This results in
**	  source file renaming, too.
**	- Fixing soft shadow bug.
**	- Only scene loading background image when activated.
**	- Fixing LDC spline initialization.
**	- Converting Windows paths into right paths on Un*x
**	
**	Revision 1.15  2001/10/09 20:47:01  sm
**	- some further texture handling.
**	
**	Revision 1.14  2001/10/06 19:24:17  sm
**	- New torus intersection routines and support routines
**	- Added further shading support from materials
**	- Added stencil checking
**	- Changed support for basis transformation for shapes with
**	  at least three direction vectors.
**	
**	Revision 1.13  2001/10/05 20:30:46  sm
**	- Introducing Mork and Phong shading.
**	- Using light source when shading
**	
**	Revision 1.12  2001/10/03 18:46:45  sm
**	- Adding illumination and recursive raytracing
**	
**	Revision 1.11  2001/09/23 14:11:18  sm
**	- A new raytrace is born! But it isn't raytracing yet.
**	
**	Revision 1.10  2001/09/02 18:54:56  sm
**	- Moving objects
**	- BBox size recomputing fixed. Further cleanups in b3RenderObject
**	  are necessary.
**	- It's really nice to see!
**
**	Revision 1.9  2001/08/20 14:16:48  sm
**	- Putting data into cmaera and light combobox.
**	- Selecting camera and light.
**
**	Revision 1.8  2001/08/18 15:38:27  sm
**	- New action toolbar
**	- Added comboboxes for camera and lights (but not filled in)
**	- Drawing Fulcrum and view volume (Clipping plane adaption is missing)
**	- Some RenderObject redesignes
**	- Color selecting bug fix in RenderObject
**
**	Revision 1.7  2001/08/11 19:59:16  sm
**	- Added orthogonal projection
**
**	Revision 1.6  2001/08/11 15:59:59  sm
**	- Rendering cleaned up
**	- CWinApp/CMainFrm derived from Blizzard III classes
**	  supporting more effective GUI.
**
**	Revision 1.5  2001/08/05 09:23:22  sm
**	- Introducing vectors, matrices, Splines and NURBS
**
**	Revision 1.4  2001/08/03 15:54:09  sm
**	- Compilation of OpenGL under Windows NT
**
**	Revision 1.3  2001/08/02 15:37:17  sm
**	- Now we are able to draw Blizzard Scenes with OpenGL.
**
**	Revision 1.2  2001/08/02 15:21:54  sm
**	- Some minor changes
**
**	Revision 1.1.1.1  2001/07/01 12:24:59  sm
**	Blizzard III is born
**
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

void b3Scene::b3Register()
{
	b3PrintF(B3LOG_DEBUG,"Registering scene classes...\n");
	b3Item::b3Register(&b3Scene::b3StaticInit, &b3Scene::b3StaticInit, TRACEANGLE_MORK);
	b3Item::b3Register(&b3Scene::b3StaticInit, &b3Scene::b3StaticInit, TRACEPHOTO_MORK);
	b3Item::b3Register(&b3Scene::b3StaticInit, &b3Scene::b3StaticInit, TRACEANGLE_PHONG);
	b3Item::b3Register(&b3Scene::b3StaticInit, &b3Scene::b3StaticInit, TRACEPHOTO_PHONG);
	b3Item::b3Register(&b3Scene::b3StaticInit, &b3Scene::b3StaticInit, TRACEPHOTO_ALBRECHT);
	b3Item::b3Register(&b3Scene::b3StaticInit, &b3Scene::b3StaticInit, GLOBAL_ILLUM);
}

b3Scene::b3Scene(b3_u32 class_type) : b3Item(sizeof(b3Scene),class_type)
{
	b3PrintF(B3LOG_DEBUG,"Blizzard III scene init.\n");

	b3AllocHeads(3);
	m_Heads[0].b3InitBase(CLASS_BBOX);
	m_Heads[1].b3InitBase(CLASS_LIGHT);
	m_Heads[2].b3InitBase(CLASS_SPECIAL);

	m_TopColor.b3Init   (0.5f,0.0f,1.0f);
	m_BottomColor.b3Init(0.0f,0.0f,1.0f);

	m_xAngle           = 0;
	m_yAngle           = 0;
	m_BBoxOverSize     = 0.02f;
	m_BackgroundType   = TP_NOTHING;
	m_TraceDepth       = 5;
	m_Flags            = TP_SIZEVALID;
	m_ShadowBrightness = 0.5f;
	m_Epsilon          = 0.001f;
	m_xSize            = 200;
	m_ySize            = 150;
	m_BackTexture      = null;
	m_ActualCamera     = null;
	m_Shader           = null;
	m_Filename.b3Empty();

	b3ReallocateShader();
}

b3Scene::b3Scene(b3_u32 *buffer) : b3Item(buffer)
{
	b3PrintF(B3LOG_DEBUG,"Blizzard III scene load.\n");

	// Background color
	b3InitColor(m_TopColor);
	b3InitColor(m_BottomColor);

	// Camera
	b3InitVector(&m_EyePoint);
	b3InitVector(&m_ViewPoint);
	b3InitVector(&m_Width);
	b3InitVector(&m_Height);

	// Some other values
	b3InitNull();
	m_xAngle           = b3InitFloat();
	m_yAngle           = b3InitFloat();
	m_BBoxOverSize     = b3InitFloat();
	m_BackgroundType   = (b3_bg_type)b3InitInt();
	m_TraceDepth       = b3InitInt();
	m_Flags            = b3InitInt();
	m_ShadowBrightness = b3InitFloat();
	m_Epsilon          = b3InitFloat();
	m_xSize            = b3InitInt();
	m_ySize            = b3InitInt();
	b3InitString(m_TextureName,B3_TEXSTRINGLEN);
	m_BackTexture      = null;
	m_ActualCamera     = null;
	m_Nebular          = null;
	m_LensFlare        = null;
	m_SuperSample      = null;
	m_Shader           = null;
	m_Filename.b3Empty();

	b3ReallocateShader();
}

void b3Scene::b3Write()
{
	b3PrintF(B3LOG_DEBUG,"Blizzard III scene storage.\n");

	// Background color
	b3StoreColor(m_TopColor);
	b3StoreColor(m_BottomColor);

	// Camera
	b3StoreVector(&m_EyePoint);
	b3StoreVector(&m_ViewPoint);
	b3StoreVector(&m_Width);
	b3StoreVector(&m_Height);

	// Some other values
	b3StoreNull  ();
	b3StoreFloat (m_xAngle);
	b3StoreFloat (m_yAngle);
	b3StoreFloat (m_BBoxOverSize);
	b3StoreInt   (m_BackgroundType);
	b3StoreCount (m_TraceDepth);
	b3StoreInt   (m_Flags);
	b3StoreFloat (m_ShadowBrightness);
	b3StoreFloat (m_Epsilon);
	b3StoreRes   (m_xSize);
	b3StoreRes   (m_ySize);
	b3StoreString(m_TextureName,B3_TEXSTRINGLEN);
}

b3Scene::~b3Scene()
{
	if (m_Shader != null)
	{
		delete m_Shader;
	}
}

void b3Scene::b3SetShading(b3_u32 class_type)
{
	ClassType = class_type;
	b3ReallocateShader();
}

void b3Scene::b3ReallocateShader()
{
	if (m_Shader != null)
	{
		delete m_Shader;
	}
	switch (b3GetClassType())
	{
	case TRACEPHOTO_MORK:
	case TRACEANGLE_MORK:
		m_Shader = new b3ShaderMork(this);
		break;

	case TRACEPHOTO_PHONG:
	case TRACEANGLE_PHONG:
		m_Shader = new b3ShaderPhong(this);
		break;

	default:
		m_Shader = null;
		break;
	}
}

b3_bool b3Scene::b3GetDisplaySize(b3_res &xSize,b3_res &ySize)
{
	xSize = this->m_xSize;
	ySize = this->m_ySize;
	return (m_Flags & TP_SIZEVALID) != 0;
}

/*************************************************************************
**                                                                      **
**                        Filename handling                             **
**                                                                      **
*************************************************************************/

char *b3Scene::b3GetName()
{
	m_Filename.b3SplitFileName(null,m_SceneName);
	m_SceneName.b3RemoveExt();
	return m_SceneName;
}

char *b3Scene::b3GetFilename()
{
	return m_Filename;
}

void b3Scene::b3SetFilename(const char *filename)
{
	m_Filename.b3Format("%s",filename);
}

void b3Scene::b3SetTexture(const char *name)
{
	b3Item::b3SetString(m_TextureName,sizeof(m_TextureName),name);
}

/*************************************************************************
**                                                                      **
**                        Special handling                              **
**                                                                      **
*************************************************************************/

b3Animation *b3Scene::b3GetAnimation(b3_bool force)
{
	b3Animation *animation;
	b3Item      *item;

	B3_FOR_BASE(b3GetSpecialHead(),item)
	{
		if (item->b3GetClassType() == ANIMATION)
		{
			return (b3Animation *)item;
		}
	}

	if (force)
	{
		animation = new b3Animation(ANIMATION);
		b3GetSpecialHead()->b3Append(animation);
	}
	else
	{
		animation = null;
	}
	return animation;
}

b3_f64 b3Scene::b3GetTimePoint()
{
	b3Animation *anim = b3GetAnimation();

	return anim != null ? anim->m_Time : 0;
}

b3ModellerInfo *b3Scene::b3GetModellerInfo()
{
	b3ModellerInfo *info;
	b3Item         *item;

	B3_FOR_BASE(b3GetSpecialHead(),item)
	{
		if (item->b3GetClassType() == LINES_INFO)
		{
			return (b3ModellerInfo *)item;
		}
	}

	info = new b3ModellerInfo(LINES_INFO);
	b3GetSpecialHead()->b3Append(info);
	return info;
}

b3Distribute *b3Scene::b3GetDistributed(b3_bool force)
{
	b3Distribute *distributed;
	b3Item       *item;

	B3_FOR_BASE(b3GetSpecialHead(),item)
	{
		if (item->b3GetClassType() == DISTRIBUTE)
		{
			return (b3Distribute *)item;
		}
	}

	if (force)
	{
		distributed = new b3Distribute(DISTRIBUTE);
		b3GetSpecialHead()->b3Append(distributed);
	}
	else
	{
		distributed = null;
	}
	return distributed;
}

b3Nebular *b3Scene::b3GetNebular(b3_bool force)
{
	b3Nebular *nebular;
	b3Item    *item;

	B3_FOR_BASE(b3GetSpecialHead(),item)
	{
		if (item->b3GetClassType() == NEBULAR)
		{
			return (b3Nebular *)item;
		}
	}

	if (force)
	{
		nebular = new b3Nebular(NEBULAR);
		b3GetSpecialHead()->b3Append(nebular);
	}
	else
	{
		nebular = null;
	}
	return nebular;
}

b3SuperSample *b3Scene::b3GetSuperSample(b3_bool force)
{
	b3SuperSample *supersample;
	b3Item        *item;

	B3_FOR_BASE(b3GetSpecialHead(),item)
	{
		if (item->b3GetClassType() == SUPERSAMPLE4)
		{
			return (b3SuperSample *)item;
		}
	}
	
	if (force)
	{
		supersample = new b3SuperSample(SUPERSAMPLE4);
		b3GetSpecialHead()->b3Append(supersample);
	}
	else
	{
		supersample = null;
	}
	return supersample;
}

b3LensFlare *b3Scene::b3GetLensFlare(b3_bool force)
{
	b3LensFlare *lensflare;
	b3Item      *item;

	B3_FOR_BASE(b3GetSpecialHead(),item)
	{
		if (item->b3GetClassType() == LENSFLARE)
		{
			return (b3LensFlare *)item;
		}
	}

	if (force)
	{
		lensflare = new b3LensFlare(LENSFLARE);
		b3GetSpecialHead()->b3Append(lensflare);
	}
	else
	{
		lensflare = null;
	}

	return lensflare;
}

b3CloudBackground*b3Scene::b3GetCloudBackground(b3_bool force)
{
	b3CloudBackground *clouds;
	b3Item            *item;

	B3_FOR_BASE(b3GetSpecialHead(),item)
	{
		if (item->b3GetClassType() == CLOUDS)
		{
			return (b3CloudBackground *)item;
		}
	}

	if (force)
	{
		clouds = new b3CloudBackground(CLOUDS);
		b3GetSpecialHead()->b3Append(clouds);
	}
	else
	{
		clouds = null;
	}

	return clouds;
}

/*************************************************************************
**                                                                      **
**                        Camera handling                               **
**                                                                      **
*************************************************************************/

b3CameraPart *b3Scene::b3GetFirstCamera(b3_bool must_active)
{
	b3CameraPart *camera,*first = null;
	b3Item       *item;

	B3_FOR_BASE(b3GetSpecialHead(),item)
	{
		if (item->b3GetClassType() == CAMERA)
		{
			camera = (b3CameraPart *)item;
			if (first == null)
			{
				first = camera;
			}
			if ((!must_active) || (camera->m_Flags & CAMERA_ACTIVE))
			{
				return camera;
			}
		}
	}

	if (first == null)
	{
		camera = new b3CameraPart(CAMERA);
		camera->m_EyePoint  = m_EyePoint;
		camera->m_ViewPoint = m_ViewPoint;
		camera->m_Width     = m_Width;
		camera->m_Height    = m_Height;
		camera->m_Flags     = CAMERA_ACTIVE;
		b3GetTitle(camera->m_CameraName,sizeof(camera->m_CameraName));
		b3GetSpecialHead()->b3Append(camera);
	}
	else
	{
		camera = first;
	}

	return camera;
}


b3CameraPart *b3Scene::b3GetCameraByName(const char *camera_name)
{
	b3CameraPart *camera;
	b3Item       *item;

	B3_FOR_BASE(b3GetSpecialHead(),item)
	{
		if (item->b3GetClassType() == CAMERA)
		{
			camera = (b3CameraPart *)item;
			if (stricmp(camera->b3GetName(),camera_name) == 0)
			{
				return camera;
			}
		}
	}
	return null;
}

b3CameraPart *b3Scene::b3GetActualCamera()
{
	if (m_ActualCamera == null)
	{
		b3SetCamera(b3GetFirstCamera(false));
	}

	return m_ActualCamera;
}

b3CameraPart *b3Scene::b3GetNextCamera(b3CameraPart *camera)
{
	while ((camera = (b3CameraPart *)camera->Succ) != null)
	{
		if (camera->b3GetClassType() == CAMERA)
		{
			return camera;
		}
	}
	return null;
}

b3CameraPart *b3Scene::b3UpdateCamera()
{
	if (m_ActualCamera != null)
	{
		b3PrintF(B3LOG_DEBUG,"Using camera %s\n",m_ActualCamera->b3GetName());
		m_EyePoint 	= m_ActualCamera->m_EyePoint;
		m_ViewPoint = m_ActualCamera->m_ViewPoint;
		m_Width     = m_ActualCamera->m_Width;
		m_Height    = m_ActualCamera->m_Height;
	}
	return m_ActualCamera;
}

void b3Scene::b3SetCamera(b3CameraPart *camera,b3_bool reorder)
{
	m_ActualCamera = camera;
	b3UpdateCamera();
	if (camera != null)
	{
		if (reorder)
		{
			b3GetSpecialHead()->b3Remove(camera);
			b3GetSpecialHead()->b3First(camera);
		}

		if (strlen(camera->m_CameraName) == 0)
		{
			b3Dir::b3SplitFileName(m_Filename,null,camera->m_CameraName);
		}
	}
}

b3_bool b3Scene::b3GetTitle(char *title,size_t size)
{
	title[0] = 0;
	if (m_ActualCamera != null)
	{
		if (strlen(m_ActualCamera->m_CameraName) > 0)
		{
			strlcpy(title,m_ActualCamera->m_CameraName,size);
		}
	}
	else
	{
		b3Dir::b3SplitFileName(m_Filename,null,title);
	}
	return strlen(title) > 0;
}

/*************************************************************************
**                                                                      **
**                        Light handling                                **
**                                                                      **
*************************************************************************/

b3Light *b3Scene::b3GetLightByName(const char *light_name)
{
	b3Light *light;
	b3Item  *item;

	B3_FOR_BASE(b3GetLightHead(),item)
	{
		light = (b3Light *)item;
		if (stricmp(light->b3GetName(),light_name) == 0)
		{
			return light;
		}
	}
	return null;
}

b3_count b3Scene::b3GetLightCount()
{
	return m_LightCount;
}

b3Light *b3Scene::b3GetLight(b3_bool must_active)
{
	b3Light *light;
	b3Item  *item;

	B3_FOR_BASE(b3GetLightHead(),item)
	{
		light = (b3Light *)item;
		if ((!must_active) || ((light->m_Flags & LIGHT_OFF) == 0))
		{
			return light;
		}
	}

	if ((light = (b3Light *)b3GetLightHead()->First) == null)
	{
		light = new b3Light(SPOT_LIGHT);
		strlcpy(light->m_Name,"Light",sizeof(light->m_Name));
		b3GetLightHead()->b3Append(light);
	}

	return light;
}

void b3Scene::b3SetLights(b3RenderContext *context)
{
	b3Item  *item;
	b3Light *light;
	b3Color  ambient;

	ambient.b3Init(
		m_ShadowBrightness,
		m_ShadowBrightness,
		m_ShadowBrightness);

	context->b3LightNum();
	context->b3LightReset();
	B3_FOR_BASE(b3GetLightHead(),item)
	{
		light = (b3Light *)item;
		if (light->b3IsActive())
		{
			// Use the same color for diffuse and specular
			context->b3LightAdd(
				&light->m_Position,
				light->m_SpotActive ? &light->m_Direction : null,
				b3ComputeSpotExponent(light),
				&light->m_Color,
				&ambient,
				&light->m_Color);
		}
	}
}

/*************************************************************************
**                                                                      **
**                        Object handling                               **
**                                                                      **
*************************************************************************/

void b3Scene::b3Recount()
{
	b3BBox::b3Recount(b3GetBBoxHead());
}

b3Scene *b3Scene::b3ReadTGF(const char *filename)
{
	return b3TGFReader::b3ReadTGFScene(filename);
}

/*************************************************************************
**                                                                      **
**                        shading                                       **
**                                                                      **
*************************************************************************/

#define LENSFLARE_LOOP 6
#define LENSFLARE_RING 2

static b3_f64 LensFlare_Distance[LENSFLARE_LOOP] =
{
	0.55,0.0,0.0,0.25,0.45,0.55
};

static b3_f64 LensFlare_ResultWeight[LENSFLARE_LOOP] =
{
	0.9,0.95,0.95,0.6,0.6,0.6
};

static b3_f64 LensFlare_Expon[LENSFLARE_LOOP] =
{
	2.4,1.5,1.5,2.0,7.0,4.0
};

void b3Scene::b3MixLensFlare(b3_ray_info *ray)
{
	b3Item    *item;
	b3Light   *light;
	b3_vector  view,central,toLight,nLight,nView;
	b3Color    result;
	b3_f64     factor,distance,weight = 0.6;
	b3_count   i;

	view.x = m_ViewPoint.x - m_EyePoint.x;
	view.y = m_ViewPoint.y - m_EyePoint.y;
	view.z = m_ViewPoint.z - m_EyePoint.z;
	factor = b3Vector::b3Length (&view);
	nView.x = view.y * ray->dir.z - view.z * ray->dir.y;
	nView.y = view.z * ray->dir.x - view.x * ray->dir.z;
	nView.z = view.x * ray->dir.y - view.y * ray->dir.x;

	B3_FOR_BASE(b3GetLightHead(),item)
	{
		light = (b3Light *)item;

		toLight.x = light->m_Position.x - m_ViewPoint.x;
		toLight.y = light->m_Position.y - m_ViewPoint.y;
		toLight.z = light->m_Position.z - m_ViewPoint.z;
		distance  = b3Vector::b3Length (&toLight) / factor;
		central.x = distance * view.x;
		central.y = distance * view.y;
		central.z = distance * view.z;

		b3Vector::b3CrossProduct (&view,&toLight,&nLight);

		for (i = 0;i < LENSFLARE_LOOP;i++)
		{
			b3_f64    cWeight,lWeight,angle,reverse,beta;
#ifdef TX_DISTURB
			b3_vector diff;
#endif

			cWeight  = LensFlare_Distance[i];
			lWeight  = 1.0 - cWeight;
			nLight.x = cWeight * central.x + lWeight * toLight.x;
			nLight.y = cWeight * central.y + lWeight * toLight.y;
			nLight.z = cWeight * central.z + lWeight * toLight.z;

			beta     = (
				nLight.x * ray->dir.x +
				nLight.y * ray->dir.y +
				nLight.z * ray->dir.z) / b3Vector::b3Length(&nLight);
			angle    = pow(beta,m_LensFlare->m_Expon * LensFlare_Expon[i]);
			if (i < LENSFLARE_RING)
			{
				if ((angle > 0.5) && (angle < 0.55)) angle = 0.3;
				else angle = 0;
			}
			else
			{
				if (angle > 0.9)  angle  = 0.9;
				else
				{
					if (angle > 0.89) angle = 0.95;
					else angle *= LensFlare_ResultWeight[i];
				}
				angle *= weight;
			}

#ifdef TX_DISTURB
			// disturbe lens flare
			diff.y  = beta;
			diff.x  = sqrt (1.0 - beta * beta);
			diff.z  = 0;
			diff.x *= 20;
			diff.y *= 20;
			if (angle < 0.95)
			{
				angle += ((Turbulence(&diff) - 0.5) * 0.1);
			}
#endif

			reverse  = 1.0 - angle;
			if (i < LENSFLARE_RING)
			{
				result.b3Init(
					ray->color[b3Color::R] * reverse + angle * light->m_Color[b3Color::R] * m_LensFlare->m_Color[b3Color::R],
					ray->color[b3Color::G],
					ray->color[b3Color::B]);
			}
			else
			{
				result = light->m_Color * m_LensFlare->m_Color * angle + ray->color * reverse;
			}
			ray->color = result;
		}
	}
}

void b3Scene::b3GetBackgroundColor(
	b3_ray_info *ray,
	b3_f64       lx,
	b3_f64       ly)
{
	b3_coord  x,y;
	b3_f64    r,sight;

	switch (m_BackgroundType)
	{
		case TP_TEXTURE :
			x = (b3_coord)(((lx + 1) * 0.5 * (m_BackTexture->xSize - 1)));
			y = (b3_coord)(((1 - ly) * 0.5 * (m_BackTexture->ySize - 1)));
			if (x < 0)                     x = 0;
			if (x >= m_BackTexture->xSize) x = m_BackTexture->xSize - 1;
			if (y < 0)                     y = 0;
			if (y >= m_BackTexture->ySize) y = m_BackTexture->ySize - 1;

			ray->color = b3Color(m_BackTexture->b3GetValue(x,y));
			break;

		case TP_SKY_N_HELL :
 			sight      = m_Clouds->b3ComputeClouds(ray,r,b3GetTimePoint());
			ray->color = b3Color::b3Mix(m_BottomColor,b3Color(r,r,B3_MAX(r,m_TopColor[b3Color::B])),sight);

#ifdef SKY_SLIDE
			ly = ray->color[b3Color::R] * 2.0 - 1.0;
#else
			break;
#endif
		case TP_SLIDE :
			ray->color = m_AvrgColor + m_DiffColor * ly;
			break;

		default:
			ray->color.b3Init(m_ShadowBrightness,m_ShadowBrightness,m_ShadowBrightness);
			break;
	}

	if (m_LensFlare != null)
	{
		b3MixLensFlare (ray);
	}
}

void b3Scene::b3GetInfiniteColor(b3_ray_info *ray)
{
	b3_f64 ly,lx;

	ly =
		ray->dir.x * m_NormHeight.x +
		ray->dir.y * m_NormHeight.y +
		ray->dir.z * m_NormHeight.z;
	lx =
		ray->dir.x * m_NormWidth.x +
		ray->dir.y * m_NormWidth.y +
		ray->dir.z * m_NormWidth.z;

	b3GetBackgroundColor (ray,lx,ly);
}

b3_f64 b3Scene::b3ComputeSpotExponent(b3Light *light)
{
	b3_index i,max = 20;
	b3_f64   p     = 0,angle;
	b3_bool  loop  = true;

	if (light->m_SpotActive)
	{
		for (i = 0;(i < max) && loop;i++)
		{
			angle = (double)i / (double)max;
			loop  = light->b3GetSpotFactor(angle) > 0.25;
		}
		p = - 1.0 / log10(cos(angle * 0.5 * M_PI));
		b3PrintF(B3LOG_FULL,"b3Scene::b3ComputeSpotExponent(%s) = %3.2f at lambda: %2.2f\n",
			light->b3GetName(),
			p,angle);
	}
	return p;
}
