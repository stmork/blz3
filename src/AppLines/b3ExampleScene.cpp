/*
**
**	$Filename:	b3ExampleScene.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - constructing simple scenes
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Lines III includes                            **
**                                                                      **
*************************************************************************/

#include "AppLines.h"
#include "blz3/raytrace/b3Factory.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.24  2005/01/07 12:38:51  smork
**	- Bump release.
**	- Added tool to create object/bbox thumb nails.
**	- Moved scene factory from Lines to raytrace lib.
**
**	Revision 1.23  2004/12/30 16:27:38  sm
**	- Removed assertion problem when starting Lines III: The
**	  image list were initialized twice due to double calling
**	  OnInitDialog() of CDialogBar. The CDialogBar::Create()
**	  calls OnInitDialog() automatically sinde MFC 7
**	- Removed many global references from raytrace and base lib
**	- Fixed ticket no. 29: The b3RenderObject::b3Recompute
**	  method checks the vertex maintainer against a null pointer.
**	
**	Revision 1.22  2004/05/30 20:25:00  sm
**	- Set paging size in supersampling dialog to 1 instead of 10.
**	- Added support for debugging super sampling.
**	- The object preview uses the shading model of its owning world.
**	- Fixed animation problem when using rotating elements on
**	  time bounds because of rounding problems. Now using
**	  b3_f32 for time points.
**	
**	Revision 1.21  2004/05/29 13:38:11  sm
**	- Made shading model visible to material an bump dialogs.
**	
**	Revision 1.20  2004/05/28 15:32:06  sm
**	- Corrected class wizard file
**	
**	Revision 1.19  2004/05/20 19:10:30  sm
**	- Separated shader from scene. this is easier
**	  to handle.
**	
**	Revision 1.18  2004/05/12 14:13:27  sm
**	- Added bump dialogs:
**	  o noise
**	  o marble
**	  o texture
**	  o glossy
**	  o groove
**	  o water
**	  o wave
**	- Setup all bump items with default values.
**	- Found bug 22 which show a camera deletion bug in
**	  toolbar and camera property dialog.
**	- Added bump example bwd
**	- Recounted resource.h (full compile necessary)
**	
**	Revision 1.17  2004/05/11 09:58:25  sm
**	- Added raytraced quick preview for bject editing.
**	
**	Revision 1.16  2004/05/06 08:38:33  sm
**	- Demerged raytracing includes of Lines
**	
**	Revision 1.15  2004/04/23 13:17:17  sm
**	- Added simple material page and renamed wood material page.
**	- Reflect material member renaming.
**	
**	Revision 1.14  2004/03/02 09:07:17  sm
**	- Added read/write support for Cook/Torrance material.
**	- Added test module for Cook/Torrance reflection model.
**	- Fixed camera name handling if camera name is empty.
**	
**	Revision 1.13  2003/08/31 10:44:07  sm
**	- Further buffer overflow avoidments.
**	
**	Revision 1.12  2003/02/18 16:52:57  sm
**	- Fixed no name error on new scenes (ticket no. 4).
**	- Introduced new b3Matrix class and renamed methods.
**	
**	Revision 1.11  2003/01/11 12:30:30  sm
**	- Some additional undo/redo actions
**	
**	Revision 1.10  2002/12/31 15:13:07  sm
**	- Some minor fixes.
**	
**	Revision 1.9  2002/08/04 13:24:55  sm
**	- Found transformation bug: Normals have to be treated as
**	  direction vectors, aren't them?
**	- b3PrepareInfo::m_PrepareProc initialized not only in
**	  debug mode.
**	
**	Revision 1.8  2002/02/28 16:58:45  sm
**	- Added torus dialogs.
**	- Fixed material and stencil handling when not activating
**	  sheet page.
**	- Further cleanup of edit dialogs done.
**	- Corrected shading of CSG cylinder and CSG cone (added
**	  shaded top and bottom plate).
**	
**	Revision 1.7  2002/02/27 20:14:51  sm
**	- Added stencil creation for creating simple shapes.
**	- Fixed material creation.
**	- Cleaned up some files.
**	
**	Revision 1.6  2002/01/16 16:17:13  sm
**	- Introducing object edit painting and acting.
**	
**	Revision 1.5  2002/01/10 17:31:11  sm
**	- Some minor GUI updates.
**	- b3BBox::b3Transform() changes m_Matrix member.
**	- Added image selection with image preview.
**	
**	Revision 1.4  2002/01/06 16:30:47  sm
**	- Added Load/Save/Replace object
**	- Enhanced "New world"
**	- Added some non static methods to b3Dir (Un*x untested, yet!)
**	- Fixed missing sphere/ellipsoid south pole triangles
**	- Fixed Spline cylinder/ring triangle indexing
**	
**	Revision 1.3  2001/11/12 16:50:29  sm
**	- Scene properties dialog coding
**	
**	Revision 1.2  2001/11/05 16:57:39  sm
**	- Creating demo scenes.
**	- Initializing some b3Item derived objects
**	
**	Revision 1.1  2001/11/04 21:12:14  sm
**	- New CB3ShowRaytrace control
**	
**
*/

/*************************************************************************
**                                                                      **
**                        b3ExampleScene implementation                 **
**                                                                      **
*************************************************************************/

inline void b3ExampleScene::b3SetCameraName(b3CameraPart *camera,int id)
{
	CString name;

	name.LoadString(id);
	camera->b3SetName(name);
}

inline void b3ExampleScene::b3SetLightName(b3Light *light,int id)
{
	CString name;

	name.LoadString(id);
	light->b3SetName(name);
}

inline void b3ExampleScene::b3SetObjectName(b3BBox *bbox,int id)
{
	CString name;

	name.LoadString(id);
	bbox->b3SetName(name);
}
