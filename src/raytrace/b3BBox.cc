/*
**
**      $Filename:      b3BBox.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing bounding boxes
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

#include "blz3/raytrace/b3BBox.h"
#include "b3ReadCOB.h"
#include "b3ReadTGF.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.101  2004/09/25 08:41:18  sm
**	- Splitting a branch for VBO development.
**
**	Revision 1.100  2004/09/24 20:22:05  sm
**	- Some VBO adjustments.
**	
**	Revision 1.99  2004/09/24 13:45:36  sm
**	- Extracted OpenGL extension vector buffer objects into own files.
**	- Some cleanup for Lines.
**	
**	Revision 1.98  2004/09/24 11:42:14  sm
**	- First VBO run under Linux.
**	
**	Revision 1.97  2004/09/23 21:27:38  sm
**	- VBOs still don't work.
**	
**	Revision 1.96  2004/09/23 15:47:04  sm
**	- Splitted b3RenderContext into own file.
**	- Added vertex buffer object support which does not
**	  run yet.
**	
**	Revision 1.95  2004/08/20 08:09:27  sm
**	- Optimized animation a little bit.
**	
**	Revision 1.94  2004/07/18 08:44:20  sm
**	- Removed recomputation marker for transformations.
**	
**	Revision 1.93  2004/07/18 08:28:44  sm
**	- Added transformation optimazation: We don't need to recompute
**	  vertices on unit matrix transformation. This simple test makes
**	  animation a lot faster!
**	
**	Revision 1.92  2004/07/02 19:28:03  sm
**	- Hoping to have fixed ticket no. 21. But the texture initialization is still slow :-(
**	- Recoupled b3Scene include from CApp*Doc header files to allow
**	  faster compilation.
**	- Removed intersection counter completely because of a mysterious
**	  destruction problem of b3Mutex.
**	
**	Revision 1.91  2004/05/19 15:35:03  sm
**	- Hope of having fixed ticket no. 13.
**	
**	Revision 1.90  2004/05/17 13:00:33  sm
**	- Fixed inverse/reverse handling of object editing.
**	- Added diverse handling vor object loading/replacing.
**	
**	Revision 1.89  2004/05/11 14:01:14  sm
**	- Added unified invert/revert for object editing.
**	- Added deletion of transform history in scene
**	  editor (= transformed history) and object editor
**	  (= original form)
**	
**	Revision 1.88  2004/05/09 15:06:56  sm
**	- Added inverse transformation for mapping.
**	- Unified scale mapping source via b3Scaling.
**	- Moved b3Scaling in its own files.
**	- Added property pages for scaling and removed
**	  scaling input fields from dialogs.
**	
**	Revision 1.87  2004/04/21 20:44:56  sm
**	- Added bump sampler to their dialogs.
**	- Added bbox dimensions for bump sampler
**	
**	Revision 1.86  2004/04/17 09:40:55  sm
**	- Splitting b3Raytrace.h into their components for
**	  better oversightment.
**	
**	Revision 1.85  2004/04/11 14:05:11  sm
**	- Raytracer redesign:
**	  o The reflection/refraction/ior/specular exponent getter
**	    are removed. The values are copied via the b3GetColors()
**	    method.
**	  o The polar members are renamed.
**	  o The shape/bbox pointers moved into the ray structure
**	- Introduced wood bump mapping.
**	
**	Revision 1.84  2003/08/31 10:44:07  sm
**	- Further buffer overflow avoidments.
**	
**	Revision 1.83  2003/07/12 17:44:47  sm
**	- Cleaned up raytracing b3Item registration
**	
**	Revision 1.82  2003/03/04 20:37:37  sm
**	- Introducing new b3Color which brings some
**	  performance!
**	
**	Revision 1.81  2003/02/26 16:36:16  sm
**	- Sorted drawing colors and added configuration support
**	  to dialog.
**	
**	Revision 1.80  2003/02/22 17:21:34  sm
**	- Changed some global variables into static class members:
**	  o b3Scene::epsilon
**	  o b3Scene::m_TexturePool et. al.
**	  o b3SplineTemplate<class VECTOR>::bspline_errno
**	
**	Revision 1.79  2003/02/22 15:17:18  sm
**	- Added support for selected shapes in object modeller
**	- Glued b3Shape and b3ShapeRenderObject. There was no
**	  distinct access method in use.
**	- Made some b3Shape methods inline and/or static which
**	  saves some memory.
**	
**	Revision 1.78  2003/02/18 16:52:57  sm
**	- Fixed no name error on new scenes (ticket no. 4).
**	- Introduced new b3Matrix class and renamed methods.
**	
**	Revision 1.77  2003/02/17 16:57:46  sm
**	- Inlining head pointer computation.
**	
**	Revision 1.76  2003/02/05 18:42:32  sm
**	- Changed TGF to scene/bbox import
**	- Resorted some menus
**	- Added TGF import to Un*x makefile
**	
**	Revision 1.75  2003/01/26 11:53:26  sm
**	- Added support for Caligari object loading.
**	
**	Revision 1.74  2003/01/18 14:13:49  sm
**	- Added move/rotate stepper operations
**	- Cleaned up resource IDs
**	
**	Revision 1.73  2003/01/07 16:14:38  sm
**	- Lines III: object editing didn't prepared any more. Fixed.
**	- Some prepare optimizations.
**	
**	Revision 1.72  2003/01/06 19:16:03  sm
**	- Removed use of b3TriangleRef into an b3Array<b3_index>.
**	- Camera transformations are now matrix transformations.
**	
**	Revision 1.71  2003/01/05 16:13:24  sm
**	- First undo/redo implementations
**	
**	Revision 1.70  2002/12/31 15:11:03  sm
**	- Fixed bound checking.
**	- Added a vector test module.
**	
**	Revision 1.69  2002/12/22 11:52:22  sm
**	- Ensure minimum volume for bounding boxes even for plain areas.
**	
**	Revision 1.68  2002/08/24 13:22:02  sm
**	- Extensive debugging on threading code done!
**	  o Cleaned up POSIX threads
**	  o Made safe thread handling available in raytracing code
**	  o b3PrepareInfo instantiates threads only once.
**	- Added new thread options to gcc: "-D_REENTRAND -pthread"
**	  which I only can assume what they are doing;-)
**	- Time window in motion blur moved from [-0.5,0.5] to [0,1]
**	  and corrected upper time limit.
**	
**	Revision 1.67  2002/08/19 16:50:39  sm
**	- Now having animation running, running, running...
**	- Activation handling modified to reflect animation
**	  and user transformation actions.
**	- Made some architectual redesigns.
**	
**	Revision 1.66  2002/08/18 13:05:17  sm
**	- First try to animate. We have to relink the control points which
**	  are stored in separate Blizzard classes to the b3AnimElement
**	  class.
**	
**	Revision 1.65  2002/08/16 11:40:38  sm
**	- Changed vertex handling for use without OpenGL. Vertex computation
**	  is needed for bound computation which is needed for animation. There
**	  are still some problems so we have to work further on Windows for
**	  better debugging.
**	- b3ExtractExt searches from right instead from left.
**	
**	Revision 1.64  2002/08/10 14:36:31  sm
**	- Some shapes had cleared the vertex array whenever the
**	  b3AllocVertices() method were called. Without calling
**	  b3Recomute() the shapes disoccured.
**	- Some methods moved as static methods into the
**	  b3Mem class.
**	
**	Revision 1.63  2002/08/07 12:38:43  sm
**	- Modified exception definition. Exceptions are identified with
**	  a three character code to unify error codes. This is necessary
**	  to convert error codes into error messages inside applications.
**	- Added some additional b3Hash methods.
**	- Added -Wall compiler option to all C++ files.
**	- Removed some compiler warnings.
**	
**	Revision 1.62  2002/08/05 16:04:55  sm
**	- Found first texture init bug. This wasn't an OpenGL bug. This
**	  couldn't be because every implementation had got the same
**	  bug. The static aux image for creating textures wasn't initialized
**	  at the right time.
**	- Version handling introduced: The version number is extracted
**	  from the version resource now.
**	- The b3Tx::b3AllocTx() method uses b3Realloc() for better
**	  memory usage.
**	- Some b3World messages removed.
**	- The 0x7fff class is registered into the b3ItemRegister now. This
**	  prevents printing a warning when this class isn't found. Due to
**	  the fact that *every* Blizzard data contains this class every
**	  data read put out this warning.
**	
**	Revision 1.61  2002/08/04 13:24:56  sm
**	- Found transformation bug: Normals have to be treated as
**	  direction vectors, aren't them?
**	- b3PrepareInfo::m_PrepareProc initialized not only in
**	  debug mode.
**	
**	Revision 1.60  2002/08/03 18:05:10  sm
**	- Cleaning up BL3_USE_OPENGL for linux/m68k without OpenGL
**	- Moved b3PrepareInfo into b3Scene class as member. This
**	  saves memory allocation calls and is an investment into
**	  faster Lines III object transformation.
**	
**	Revision 1.59  2002/08/02 14:52:12  sm
**	- Vertex/normal computation is now multithreaded, too.
**	- Minor changes on b3PrepareInfo class.
**	- Last changes to Windows port.
**	
**	Revision 1.58  2002/08/02 11:59:25  sm
**	- b3Thread::b3Wait now returns thread result.
**	- b3Log_SetLevel returns old log level.
**	- Introduced b3PrepareInfo class for multithreaded initialization
**	  support. Should be used for b3AllocVertices and b3ComputeVertices:-)
**	- b3TxPool class is now thread safe.
**	
**	Revision 1.57  2002/08/01 15:02:56  sm
**	- Found texture missing bug when printing. There weren't any
**	  selected textures inside an other OpenGL rendering context.
**	  Now fixed!
**	
**	Revision 1.56  2002/07/27 18:51:31  sm
**	- Drawing changed to glInterleavedArrays(). This means that
**	  extra normal and texture arrays are omitted. This simplifies
**	  correct programming, too.
**	
**	Revision 1.55  2002/07/25 13:22:32  sm
**	- Introducing spot light
**	- Optimized light settings when drawing
**	- Further try of stencil maps
**	
**	Revision 1.54  2002/07/21 17:02:36  sm
**	- Finished advanced color mix support (correct Phong/Mork shading)
**	- Added first texture mapping support. Further development on
**	  Windows now...
**	
**	Revision 1.53  2002/03/13 19:01:59  sm
**	- Fixed some GCC warnings.
**	
**	Revision 1.52  2002/03/10 13:55:15  sm
**	- Added creation dialog for rotation shapes.
**	- Cleaned up derivation of b3SplineRotShape.
**	- Added support for foreign BLZ3_HOME directories.
**	
**	Revision 1.51  2002/03/05 20:38:25  sm
**	- Added first profile (beveled spline shape).
**	- Added some features to b3SplineTemplate class.
**	- Added simple control to display 2 dimensional spline.
**	- Fine tuned the profile dialogs.
**	
**	Revision 1.50  2002/03/03 21:22:22  sm
**	- Added support for creating surfaces using profile curves.
**	- Added simple creating of triangle fields.
**	
**	Revision 1.49  2002/03/02 19:52:39  sm
**	- Nasty UnCR
**	- Fixed some compile bugs due to incompatibilities to Visual C++
**	
**	Revision 1.48  2002/02/27 20:14:51  sm
**	- Added stencil creation for creating simple shapes.
**	- Fixed material creation.
**	- Cleaned up some files.
**	
**	Revision 1.47  2002/02/22 20:18:09  sm
**	- Added shape/bbox creation in object editor. So bigger
**	  icons (64x64) for shape selection are created.
**	- Created new class for image list maintainance.
**	
**	Revision 1.46  2002/02/19 16:26:49  sm
**	- Further CSG interval computing cleanup done.
**	
**	Revision 1.45  2002/02/18 17:50:31  sm
**	- Corrected some intersection problems concerning CSG
**	- Added CSG shape icons
**	- renamed classes appropriate.
**	
**	Revision 1.44  2002/02/17 21:25:06  sm
**	- Introduced CSG
**	  o Heavily reorganized shape inheritance
**	  o New file b3CSGShape added
**	
**	Revision 1.43  2002/01/21 16:56:46  sm
**	- Showing splash dialog only in release version.
**	- Prepared shape icons.
**	
**	Revision 1.42  2002/01/19 19:57:56  sm
**	- Further clean up of CAppRenderDoc derivates done. Especially:
**	  o Moved tree build from CDlgHierarchy into documents.
**	  o All views react on activating.
**	  o CAppObjectDoc creation cleaned up.
**	  o Fixed some ugly drawing dependencies during initialization.
**	     Note: If you don't need Windows -> You're fine!
**	
**	Revision 1.41  2002/01/18 16:49:35  sm
**	- Further development of the object edit from scene branch. This needs
**	  much more logics for handling scenes and open object edits properly.
**	
**	Revision 1.40  2002/01/14 16:13:02  sm
**	- Some further cleanups done.
**	- Icon reordering done.
**	
**	Revision 1.39  2002/01/11 16:14:39  sm
**	- Fixed damaged b3Transform() by correcting used parameter vor
**	  b3MatrixMMul and the b3BBox::m_Matrix meber.
**	- Fixed Preview selection dialog.
**	
**	Revision 1.38  2002/01/10 17:31:11  sm
**	- Some minor GUI updates.
**	- b3BBox::b3Transform() changes m_Matrix member.
**	- Added image selection with image preview.
**	
**	Revision 1.37  2002/01/09 17:47:54  sm
**	- Finished CB3ImageButton implementation.
**	- Finished CDlgObjectCopy
**	
**	Revision 1.36  2002/01/08 16:04:08  sm
**	- New copy dialog added
**	- Merge with daily work
**	
**	Revision 1.35  2002/01/07 16:18:51  sm
**	- Added b3Item clone
**	- Added Drag & Drop
**	
**	Revision 1.34  2002/01/06 21:38:18  sm
**	- Nasty Un CR/LF
**	- b3Dir fix. Not tested, yet!
**	- make count/make count_all update
**	
**	Revision 1.33  2002/01/05 22:17:47  sm
**	- Recomputing bounding boxes correctly
**	- Found key input bug: The accelerator are the problem
**	- Code cleanup
**	
**	Revision 1.32  2002/01/04 17:53:54  sm
**	- Added new/delete object.
**	- Added deactive rest of all scene objects.
**	- Changed icons to reflect object activation.
**	- Sub object insertion added.
**	- Fixed update routines to reflect correct state in hierarchy.
**	- Better hierarchy update coded.
**	
**	Revision 1.31  2002/01/03 19:07:27  sm
**	- Cleaned up cut/paste
**	
**	Revision 1.30  2002/01/03 15:50:14  sm
**	- Added cut/copy/paste
**	
**	Revision 1.29  2002/01/02 15:48:37  sm
**	- Added automated expand/collapse to hierarchy tree.
**	
**	Revision 1.28  2001/12/30 22:52:35  sm
**	- Made b3Scene::b3SetCamera() compatible to earlier versions.
**	
**	Revision 1.27  2001/12/30 16:54:15  sm
**	- Inserted safe b3Write() into Lines III
**	- Fixed b3World saving: b3StoreXXX() methods must ensure
**	  buffer before the buffer itself is used.
**	- Extended b3Shape format with shape activation flag. Nice: The
**	  new data structures don't confuse the old Lines II/Blizzard II and
**	  even stores these new values.
**	
**	Revision 1.26  2001/12/30 14:16:57  sm
**	- Abstracted b3File to b3FileAbstract to implement b3FileMem (not done yet).
**	- b3Item writing implemented and updated all raytracing classes
**	  to work properly.
**	- Cleaned up spline shapes and CSG shapes.
**	- Added b3Caustic class for compatibility reasons.
**	
**	Revision 1.25  2001/12/02 17:38:17  sm
**	- Removing nasty CR/LF
**	- Added b3ExtractExt()
**	- Added stricmp() for Un*x
**	- Fixed some defines
**	
**	Revision 1.24  2001/12/02 15:43:49  sm
**	- Creation/Deletion/Editing of lights
**	- Creation/Deletion of cameras
**	- New toolbars introduced.
**	
**	Revision 1.23  2001/11/09 16:15:35  sm
**	- Image file encoder
**	- Performance meter for triangles / second added.
**	- Corrected Windows b3TimeSpan computation
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
**	Revision 1.20  2001/10/21 16:55:20  sm
**	- Introducing lens flares.
**	- Introducing different modes of background computation.
**	- Introducing different types of row sampling.
**	
**	Revision 1.19  2001/10/19 14:46:57  sm
**	- Rotation spline shape bug found.
**	- Major optimizations done.
**	- Cleanups
**	
**	Revision 1.18  2001/09/23 14:11:18  sm
**	- A new raytrace is born! But it isn't raytracing yet.
**	
**	Revision 1.17  2001/09/02 18:54:56  sm
**	- Moving objects
**	- BBox size recomputing fixed. Further cleanups in b3RenderObject
**	  are necessary.
**	- It's really nice to see!
**
**	Revision 1.16  2001/09/01 15:54:54  sm
**	- Tidy up Size confusion in b3Item/b3World and derived classes
**	- Made (de-)activation of objects
**
**	Revision 1.15  2001/08/18 15:38:27  sm
**	- New action toolbar
**	- Added comboboxes for camera and lights (but not filled in)
**	- Drawing Fulcrum and view volume (Clipping plane adaption is missing)
**	- Some RenderObject redesignes
**	- Color selecting bug fix in RenderObject
**
**	Revision 1.14  2001/08/16 14:41:24  sm
**	- Some more shading shapes added (only BSPline shapes are missing)
**
**	Revision 1.13  2001/08/14 15:37:50  sm
**	- Made some cleanups when OpenGL isn't available.
**
**	Revision 1.12  2001/08/12 19:47:48  sm
**	- Now having correct orthogonal projection incl. aspect ratio
**
**	Revision 1.11  2001/08/11 20:17:22  sm
**	- Updated OpenGL on Un*x platform.
**	- This was a great day!
**
**	Revision 1.10  2001/08/11 19:59:16  sm
**	- Added orthogonal projection
**
**	Revision 1.9  2001/08/10 15:14:36  sm
**	- Now having all shapes implemented for drawing lines.
**
**	Revision 1.8  2001/08/08 20:12:59  sm
**	- Fixing some makefiles
**	- introducing check/BlzDump (BlzDump moved from tools)
**	- Some further line drawing added
**	- b3RenderContext and b3RenderObject introduced. Every b3Shape inherit from
**	  b3RenderObject.
**
**	Revision 1.7  2001/08/06 20:52:56  sm
**	- Some CR/LF removal
**
**	Revision 1.6  2001/08/06 19:58:58  sm
**	- Drawing area - the first shape we can see with OpenGL
**
**	Revision 1.5  2001/08/06 16:35:35  sm
**	- Drawing first area
**
**	Revision 1.4  2001/08/02 15:37:17  sm
**	- Now we are able to draw Blizzard Scenes with OpenGL.
**
**	Revision 1.3  2001/08/02 15:21:54  sm
**	- Some minor changes
**
**	Revision 1.2  2001/07/02 16:09:46  sm
**	- Added bounding box reorganization.
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

static b3_gl_line m_BBoxIndices[12 * 2] =
{
	{ 0,1 },
	{ 1,2 },
	{ 2,3 },
	{ 3,0 },
	{ 4,5 },
	{ 5,6 },
	{ 6,7 },
	{ 7,4 },
	{ 0,4 },
	{ 1,5 },
	{ 2,6 },
	{ 3,7 }
};

b3Color b3BBox::m_GridColor(0.4f,0.4f,0.4f,0.0f);
b3_bool b3BBox::m_GridVisible = true;

void b3BBox::b3Register()
{
	b3PrintF (B3LOG_DEBUG,"Registering bounding boxes...\n");
	b3Item::b3Register(&b3BBox::b3StaticInit,&b3BBox::b3StaticInit,CLASS_BBOX,true);
}
                                                                                                                     
b3BBox::b3BBox(b3_u32 class_type) : b3Item(sizeof(b3BBox),class_type)
{
	b3AllocHeads(2);
	m_Heads[0].b3InitBase(CLASS_SHAPE);
	m_Heads[1].b3InitBase(CLASS_BBOX);

	b3Matrix::b3Unit(&m_Matrix);
	m_Type       = 0;
	b3Vector::b3Init(&m_DimBase,-1.0,-1.0,-1.0);
	b3Vector::b3Init(&m_DimSize, 2.0, 2.0, 2.0);
	m_BoxName[0] = 0;
	m_BoxURL[0]  = 0;

	m_CSGIntersectionCount = 0;
	m_ShapeCount           = 0;
}

b3BBox::b3BBox(b3_u32 *src) : b3Item(src)
{
	b3_size diff;

	m_Type = b3InitInt();
	b3InitVector(&m_DimBase);
	b3InitVector(&m_DimSize);
	b3InitNOP();
	b3InitMatrix(&m_Matrix);

	diff = m_ItemSize - (b3_size)(m_ParseIndex << 2);
	if (diff < B3_BOXSTRINGLEN)
	{
		b3InitString(m_BoxName,diff);
		m_BoxURL[0] = 0;
	}
	else
	{
		b3InitString(m_BoxName,B3_BOXSTRINGLEN);
		b3InitString(m_BoxURL, B3_BOXSTRINGLEN);
	}

	m_CSGIntersectionCount = 0;
	m_ShapeCount           = 0;
}

void b3BBox::b3Write()
{
	b3StoreInt(m_Type);
	b3StoreVector(&m_DimBase);
	b3StoreVector(&m_DimSize);
	b3StoreNull(); // This is Custom
	b3StoreMatrix(&m_Matrix);

	b3StoreString(m_BoxName,B3_BOXSTRINGLEN);
	b3StoreString(m_BoxURL, B3_BOXSTRINGLEN);
}

void b3BBox::b3CollectBBoxes(b3Array<b3BBoxReference> &array)
{
	b3BBoxReference  reference(this);
	b3Item          *item;
	b3BBox          *bbox;
	
	array.b3Add(reference);
	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox = (b3BBox *)item;
		bbox->b3CollectBBoxes(array);
	}
}

b3_bool b3BBox::b3Prepare(b3_bool recursive)
{
	b3Item     *item;
	b3Shape    *shape;
	b3CSGShape *csgShape;
	b3BBox     *bbox;

	b3Matrix::b3Inverse(&m_Matrix,&m_Inverse);
	m_ShapeCount = 0;
	B3_FOR_BASE(b3GetShapeHead(),item)
	{
		shape = (b3Shape *)item;
		if (!shape->b3Prepare())
		{
			b3PrintF(B3LOG_NORMAL,
				"Shape not initialized [%s].\n",
				b3GetName());
			return false;
		}

		m_ShapeCount++;
	}

	m_CSGIntersectionCount = 0;
	if (b3GetShapeHead()->b3GetClass() == CLASS_CSG)
	{
		B3_FOR_BASE(b3GetShapeHead(),item)
		{
			csgShape = (b3CSGShape *)item;
			m_CSGIntersectionCount += csgShape->b3GetMaxIntersections();
		}
		if ((m_CSGIntersectionCount >= B3_MAX_CSG_INTERSECTIONS_PER_BBOX) ||
		    (m_ShapeCount >= B3_MAX_CSG_SHAPES_PER_BBOX))
		{
			b3PrintF(B3LOG_NORMAL,
				"Not enough static memory for CSG computation [%s].\n",
				b3GetName());
			b3PrintF(B3LOG_NORMAL,
				"Found %d shapes with %d max. possible intersections.\n",
				m_ShapeCount,m_CSGIntersectionCount);
    		return false;
    	}
    }

	if (recursive)
	{
		B3_FOR_BASE(b3GetBBoxHead(),item)
		{
			bbox = (b3BBox *)item;
			if (!bbox->b3Prepare(recursive))
			{
				return false;
			}
		}
	}

	return true;
}

char *b3BBox::b3GetName()
{
	return m_BoxName;
}

void b3BBox::b3Dump(b3_count level)
{
	b3Item *bbox;

	level = b3GetType();
	b3DumpSpace(level);
	b3PrintF(B3LOG_NORMAL,"Object %s (level %d)\n",m_BoxName,level);

	B3_FOR_BASE(b3GetBBoxHead(),bbox)
	{
		bbox->b3Dump(level);
	}
}

void b3BBox::b3Reorg(
	b3Base<b3Item> *depot,
	b3Base<b3Item> *base,
	b3_count        level,
	b3_count        rec,
	b3Item         *insert_after)
{
	b3_count        new_level;
	b3BBox         *bbox;
	b3Base<b3Item> *sub_base = null;

	while ((bbox = (b3BBox *)depot->First) != null)
	{
		new_level = bbox->b3GetType();
		if (new_level < level)
		{
			return;
		}
		if (new_level == level)
		{
			depot->b3Remove(bbox);
			insert_after != null ? base->b3Insert(insert_after,bbox) : base->b3Append(bbox);
			sub_base = bbox->b3GetBBoxHead();
		}
		else
		{
			B3_ASSERT(sub_base != null);
			b3Reorg(depot,sub_base,new_level,rec + 1);
		}
	}
}

void b3BBox::b3Recount(b3Base<b3Item> *base,b3_count level)
{
	b3Item *item;
	b3BBox *bbox;

	B3_FOR_BASE(base,item)
	{
		bbox = (b3BBox *)item;
		b3Recount(bbox->b3GetBBoxHead(),level + 1);
		bbox->ClassType &= 0xffff0000;
		bbox->ClassType |= level;
	}
}

void b3Scene::b3Reorg()
{
	b3Base<b3Item>  depot;
	b3Base<b3Item> *base;
	b3Link<b3Item> *first;
	b3_count        level;

	base  =  b3GetBBoxHead();
	depot = *base;
	base->b3InitBase(CLASS_BBOX);
	if ((first = depot.First) != null)
	{
		level = first->b3GetType();
		b3BBox::b3Reorg(&depot,base,level,1);
	}
	b3BBox::b3Recount(base);
}

void b3BBox::b3SetupVertexMemory(b3RenderContext *context)
{
	b3Item  *item;
	b3BBox  *bbox;
	b3Shape *shape;

	b3RenderObject::b3SetupVertexMemory(context);

	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox = (b3BBox *)item;
		bbox->b3SetupVertexMemory(context);
		bbox->b3AddCount(context);
	}
	B3_FOR_BASE(b3GetShapeHead(),item)
	{
		shape = (b3Shape *)item;
		shape->b3SetupVertexMemory(context);
		shape->b3AddCount(context);
	}
}

void b3Scene::b3AllocVertices(b3RenderContext *context)
{
	b3Item  *item;
	b3BBox  *bbox;

	context->glVertexCount = 0;
	context->glPolyCount   = 0;
	context->glGridCount   = 0;
	context->glTextureSize = 0;
	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox = (b3BBox *)item;
		bbox->b3SetupVertexMemory(context);
		bbox->b3AddCount(context);
	}
}

void b3BBox::b3GetCount(
	b3RenderContext *ctx,
	b3_count        &vertCount,
	b3_count        &gridCount,
	b3_count        &polyCount)
{
	vertCount = 8;
}

void b3BBox::b3AllocVertexMemory(b3RenderContext *context)
{
	if (b3HasVBO())
	{
		b3RenderObject::b3AllocVertexMemory(context);
	}
	else
	{
		glVertex = m_BBoxVertex;
		glVertexCount = 8;
	}
}

void b3BBox::b3FreeVertexMemory()
{
	b3Item  *item;
	b3BBox  *bbox;
	b3Shape *shape;

	B3_FOR_BASE(b3GetShapeHead(),item)
	{
		shape = (b3Shape *)item;
		shape->b3FreeVertexMemory();
	}
	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox = (b3BBox *)item;
		bbox->b3FreeVertexMemory();

	}

	b3RenderObject::b3FreeVertexMemory();
}

void b3BBox::b3ComputeVertices()
{
	glVertex[0].v.x = m_DimBase.x;
	glVertex[0].v.y = m_DimBase.y;
	glVertex[0].v.z = m_DimBase.z;

	glVertex[1].v.x = m_DimBase.x;
	glVertex[1].v.y = m_DimBase.y;
	glVertex[1].v.z = m_DimBase.z + m_DimSize.z;

	glVertex[2].v.x = m_DimBase.x + m_DimSize.x;
	glVertex[2].v.y = m_DimBase.y;
	glVertex[2].v.z = m_DimBase.z + m_DimSize.z;

	glVertex[3].v.x = m_DimBase.x + m_DimSize.x;
	glVertex[3].v.y = m_DimBase.y;
	glVertex[3].v.z = m_DimBase.z;

	glVertex[4].v.x = m_DimBase.x;
	glVertex[4].v.y = m_DimBase.y + m_DimSize.y;
	glVertex[4].v.z = m_DimBase.z;

	glVertex[5].v.x = m_DimBase.x;
	glVertex[5].v.y = m_DimBase.y + m_DimSize.y;
	glVertex[5].v.z = m_DimBase.z + m_DimSize.z;

	glVertex[6].v.x = m_DimBase.x + m_DimSize.x;
	glVertex[6].v.y = m_DimBase.y + m_DimSize.y;
	glVertex[6].v.z = m_DimBase.z + m_DimSize.z;

	glVertex[7].v.x = m_DimBase.x + m_DimSize.x;
	glVertex[7].v.y = m_DimBase.y + m_DimSize.y;
	glVertex[7].v.z = m_DimBase.z;

	glVertexCount   = 8;
}

void b3BBox::b3ComputeNormals(b3_bool normalize)
{
}

void b3BBox::b3ComputeIndices()
{
	glGridCount   = 12;
	glPolyCount   =  0;

	glGrids    = m_BBoxIndices;
	glPolygons = null;
}

void b3BBox::b3Draw(b3RenderContext *context)
{
	b3Item  *item;
	b3BBox  *bbox;
	b3Shape *shape;

	// Draw this
	b3RenderObject::b3Draw(context);

	// Draw our shapes
	B3_FOR_BASE(b3GetShapeHead(),item)
	{
		shape = (b3Shape *)item;
		shape->b3Draw(context);
	}

	// Draw subsequent BBoxes
	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox = (b3BBox *)item;
		bbox->b3Draw(context);
	}
}

void b3BBox::b3Update()
{
	b3Item  *item;
	b3Shape *shape;

	b3RenderObject::b3Update();
	B3_FOR_BASE(b3GetShapeHead(),item)
	{
		shape = (b3Shape *)item;
		shape->b3Update();
	}
}

b3_bool b3Scene::b3UpdateThread(b3BBox *bbox,void *ptr)
{
	bbox->b3Update();
	return true;
}

void b3Scene::b3Update()
{
	b3PrintF(B3LOG_FULL,"    Updating geometry...\n");
	m_PrepareInfo.b3CollectBBoxes(this);
	m_PrepareInfo.b3Prepare(b3UpdateThread);
}

b3_bool b3Scene::b3UpdateMaterialThread(b3BBox *bbox,void *ptr)
{
	b3Item  *item;
	b3Shape *shape;

	bbox->b3UpdateMaterial();
	B3_FOR_BASE(bbox->b3GetShapeHead(),item)
	{
		shape = (b3Shape *)item;
		shape->b3UpdateMaterial();
	}
	return true;
}

void b3Scene::b3UpdateMaterial()
{
	b3PrintF(B3LOG_FULL,"    Updating materials...\n");
	m_PrepareInfo.b3CollectBBoxes(this);
	m_PrepareInfo.b3Prepare(b3UpdateMaterialThread);
}

b3_bool b3Scene::b3RecomputeMaterialThread(b3BBox *bbox,void *ptr)
{
	b3Item  *item;
	b3Shape *shape;

	bbox->b3RecomputeMaterial();
	B3_FOR_BASE(bbox->b3GetShapeHead(),item)
	{
		shape = (b3Shape *)item;
		shape->b3RecomputeMaterial();
	}
	return true;
}

void b3Scene::b3RecomputeMaterial()
{
	b3PrintF(B3LOG_FULL,"    Invalidating materials...\n");
	m_PrepareInfo.b3CollectBBoxes(this);
	m_PrepareInfo.b3Prepare(b3RecomputeMaterialThread);
}

b3_bool b3BBox::b3ComputeBounds(b3_vector *lower,b3_vector *upper,b3_f64 tolerance)
{
	b3Item    *item;
	b3BBox    *bbox;
	b3Shape   *shape;
	b3_vector  subLower;
	b3_vector  subUpper;
	b3_bool    result = false;

	b3Vector::b3InitBound(&subLower,&subUpper);
	B3_FOR_BASE(b3GetShapeHead(),item)
	{
		shape   = (b3Shape *)item;
		result |= shape->b3ComputeBounds(&subLower,&subUpper);
	}

	if (result)
	{
		// Use fresh data
		b3Vector::b3Scale(b3Vector::b3Sub(&subUpper,&subLower,&m_DimSize),tolerance * 0.5);
		b3Vector::b3SetMinimum(&m_DimSize,b3Scene::epsilon);
		b3Vector::b3Sub(&m_DimSize,&subLower);
		b3Vector::b3Add(&m_DimSize,&subUpper);
	}

	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox    = (b3BBox *)item;
		result |= bbox->b3ComputeBounds(&subLower,&subUpper,tolerance);
	}

	// Check lower bound
	b3Vector::b3CheckLowerBound(lower,&subLower);

	// Check upper bound
	b3Vector::b3CheckUpperBound(upper,&subUpper);

	// Compute bounds of this BBox
	if (result)
	{
		b3Vector::b3Sub(&subUpper,&subLower,&m_DimSize);
		m_DimBase   = subLower;

		b3Recompute();
		b3RenderObject::b3Update();
	}

	return result;
}

b3_bool b3Scene::b3ComputeBounds(b3_vector *lower,b3_vector *upper)
{
	b3Item  *item;
	b3BBox  *bbox;
	b3_bool  result = false;

	b3Update();

	b3PrintF(B3LOG_FULL,"    Updating geometry dimensions...\n");
	b3Vector::b3InitBound(lower,upper);
	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox    = (b3BBox *)item;
		result |= bbox->b3ComputeBounds(lower,upper,m_BBoxOverSize);
	}
	return result;
}

void b3BBox::b3ComputeBoxPolar(b3_ray *ray)
{
	b3_f64 x = ray->ipoint.x;
	b3_f64 y = ray->ipoint.y;
	b3_f64 z = ray->ipoint.z;

	ray->polar.m_BoxPolar.x = (x - m_DimBase.x) / m_DimSize.x;
	ray->polar.m_BoxPolar.y = (y - m_DimBase.y) / m_DimSize.y;
	ray->polar.m_BoxPolar.z = (z - m_DimBase.z) / m_DimSize.z;

	ray->polar.m_BBoxOriginal.x = (b3_f32)(x * m_Inverse.m11 + y * m_Inverse.m12 + z * m_Inverse.m13 + m_Inverse.m14);
	ray->polar.m_BBoxOriginal.y = (b3_f32)(x * m_Inverse.m21 + y * m_Inverse.m22 + z * m_Inverse.m23 + m_Inverse.m24);
	ray->polar.m_BBoxOriginal.z = (b3_f32)(x * m_Inverse.m31 + y * m_Inverse.m32 + z * m_Inverse.m33 + m_Inverse.m34);
}

b3_count b3BBox::b3Count()
{
	b3_count count = 1;

	b3Item *item;
	b3BBox *bbox;

	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox   = (b3BBox *)item;
		count += bbox->b3Count();
	}
	return count;
}

void b3BBox::b3ResetTransformation()
{
	b3Item  *item;
	b3BBox  *bbox;

    b3Matrix::b3Unit(&m_Matrix);
	b3Matrix::b3Unit(&m_Inverse);
	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox = (b3BBox *)item;
		bbox->b3ResetTransformation();
	}
}

b3_bool b3BBox::b3Inverse(b3_matrix *original)
{
	b3_matrix inverse;
	b3_bool   success;

#ifdef _DEBUG
	b3PrintF(B3LOG_FULL,"--- b3BBox::b3Inverse() ---\n");
	b3Matrix::b3Dump(&m_Matrix, "original");
#endif

	success = b3Matrix::b3Inverse(&m_Matrix,&inverse) != null;
	if (success)
	{
		if (original != null)
		{
			*original = m_Matrix;
		}
		b3Transform(&inverse,true,true);

		// This is not necessary but it removes
		// round errors.
		b3ResetTransformation();
	}

#ifdef _DEBUG
	b3Matrix::b3Dump(&m_Matrix, "b3BBox::m_Matrix");
	b3Matrix::b3Dump(&m_Inverse,"b3BBox::m_Inverse");
#endif

	return success;
}

b3_bool b3BBox::b3Reverse(b3_matrix *original)
{
	b3_bool   success;

	success = b3Matrix::b3Inverse(original,&m_Inverse) != null;
	if (success)
	{
		b3Transform(original,true,true);
	}

#ifdef _DEBUG
	b3PrintF(B3LOG_FULL,"--- b3BBox::b3Reverse() ---\n");
	b3Matrix::b3Dump(original, "original");
	b3Matrix::b3Dump(&m_Matrix, "b3BBox::m_Matrix");
	b3Matrix::b3Dump(&m_Inverse,"b3BBox::m_Inverse");
#endif
	
	return success;
}

b3_bool b3BBox::b3Transform(
	b3_matrix *transformation,
	b3_bool    is_affine,
	b3_bool    force_action)
{
	b3Item  *item;
	b3Shape *shape;
	b3BBox  *bbox;
	b3_bool  transformed = false;

	B3_FOR_BASE(b3GetShapeHead(),item)
	{
		shape = (b3Shape *)item;
		if (force_action || shape->b3IsActive())
		{
			shape->b3Transform(transformation,is_affine);
			transformed = true;
		}
	}

	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox = (b3BBox *)item;
		if(bbox->b3Transform(transformation,is_affine,force_action))
		{
			transformed = true;
		}
	}

	if (force_action || b3IsActive())
	{
		b3Matrix::b3MMul(&m_Matrix,transformation,&m_Matrix);
		transformed = true;
	}

	return transformed;
}

void b3Scene::b3Transform(
	b3_matrix *transformation,
	b3_bool    is_affine,
	b3_bool    force_action)
{
	if (!b3Matrix::b3IsUnitMatrix(transformation))
	{
		b3Item *item;
		b3BBox *bbox;

		B3_FOR_BASE(b3GetBBoxHead(),item)
		{
			bbox = (b3BBox *)item;
			bbox->b3Transform(transformation,is_affine,force_action);
		}
	}
}

void b3Scene::b3Draw(b3RenderContext *context)
{
	b3Item *item;
	b3BBox *bbox;

	b3PrintT("OpenGL - Drawing start");
	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox = (b3BBox *)item;
		bbox->b3Draw(context);
	}
	b3PrintT("OpenGL - Drawing end");
}

void b3BBox::b3Activate(b3_bool activate,b3_bool recurse)
{
	b3Item  *item;
	b3Shape *shape;
	b3BBox  *bbox;

	if (activate) m_Type |=   BBF_ACTIVE;
	else          m_Type &= (~BBF_ACTIVE);

	B3_FOR_BASE(b3GetShapeHead(),item)
	{
		shape = (b3Shape *)item;
		shape->b3Activate(activate);
	}

	if (recurse)
	{
		B3_FOR_BASE(b3GetBBoxHead(),item)
		{
			bbox = (b3BBox *)item;
			bbox->b3Activate(activate);
		}
	}
}

void b3Scene::b3Activate(b3_bool activate)
{
	b3Item  *item;
	b3BBox  *bbox;

	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox = (b3BBox *)item;
		bbox->b3Activate(activate);
	}
}

b3_bool b3BBox::b3IsActive()
{
	return (m_Type & BBF_ACTIVE) != 0;
}

void b3BBox::b3Expand(b3_bool expand)
{
	if (expand) m_Type |=   BBF_EXPANDED;
	else        m_Type &= (~BBF_EXPANDED);
}

b3_bool b3BBox::b3IsExpanded()
{
	return (m_Type & BBF_EXPANDED) != 0;
}

b3_count b3Scene::b3GetBBoxCount()
{
	b3Item   *item;
	b3BBox   *bbox;
	b3_count  count = 0;

	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox = (b3BBox *)item;
		count += bbox->b3Count();
	}
	return count;
}

b3_bool b3BBox::b3FindBBox(b3Base<b3Item> *base,b3BBox *search)
{
	b3Item  *item;
	b3BBox  *bbox;

	B3_FOR_BASE(base,item)
	{
		bbox = (b3BBox *)item;
		if (bbox == search)
		{
			return true;
		}
		if (b3FindBBox(bbox->b3GetBBoxHead(),search))
		{
			return true;
		}
	}
	return false;
}

/*************************************************************************
**                                                                      **
**                        Find BBox where bbox belongs to               **
**                                                                      **
*************************************************************************/

b3Base<b3Item> *b3BBox::b3FindBBoxHead(b3BBox *bbox)
{
	b3Item         *item;
	b3BBox         *inc_bbox;
	b3Base<b3Item> *base,*result;

	base = b3GetBBoxHead();
	B3_FOR_BASE(base,item)
	{
		if (item == bbox)
		{
			return base;
		}

		inc_bbox = (b3BBox *)item;
		result   = inc_bbox->b3FindBBoxHead(bbox);
		if (result != null)
		{
			return result;
		}
	}
	return null;
}

b3Base<b3Item> *b3Scene::b3FindBBoxHead(b3BBox *bbox)
{
	b3Item         *item;
	b3BBox         *inc_bbox;
	b3Base<b3Item> *base,*result;

	base = b3GetBBoxHead();
	B3_FOR_BASE(base,item)
	{
		if (item == bbox)
		{
			return base;
		}

		inc_bbox = (b3BBox *)item;
		result   = inc_bbox->b3FindBBoxHead(bbox);
		if (result != null)
		{
			return result;
		}
	}
	return null;
}

/*************************************************************************
**                                                                      **
**                        Collect all (de-)activated BBoxes             **
**                                                                      **
*************************************************************************/

void b3BBox::b3CollectActiveBBoxes(b3Array<b3BBox *> *array,b3_bool activation)
{
	b3Item         *item;
	b3BBox         *bbox;
	b3Base<b3Item> *base;

	if (b3IsActive() == activation)
	{
		array->b3Add(this);
	}

	base = b3GetBBoxHead();
	B3_FOR_BASE(base,item)
	{
		bbox   = (b3BBox *)item;
		bbox->b3CollectActiveBBoxes(array,activation);
	}
}

void b3Scene::b3CollectActiveBBoxes(b3Array<b3BBox *> *array,b3_bool activation)
{
	b3Item         *item;
	b3BBox         *bbox;
	b3Base<b3Item> *base;

	array->b3Clear();

	base = b3GetBBoxHead();
	B3_FOR_BASE(base,item)
	{
		bbox   = (b3BBox *)item;
		bbox->b3CollectActiveBBoxes(array,activation);
	}
}

/*************************************************************************
**                                                                      **
**                        Find BBox where shape belongs to              **
**                                                                      **
*************************************************************************/

b3BBox *b3BBox::b3FindParentBBox(b3Shape *shape)
{
	b3Item         *item;
	b3BBox         *bbox,*result;
	b3Base<b3Item> *base;

	base = b3GetShapeHead();
	B3_FOR_BASE(base,item)
	{
		if (item == shape)
		{
			return this;
		}
	}

	base = b3GetBBoxHead();
	B3_FOR_BASE(base,item)
	{
		bbox   = (b3BBox *)item;
		result = bbox->b3FindParentBBox(shape);
		if (result != null)
		{
			return result;
		}
	}
	return null;
}

b3BBox *b3Scene::b3FindParentBBox(b3Shape *shape)
{
	b3Item         *item;
	b3BBox         *bbox,*result;

	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox   = (b3BBox *)item;
		result = bbox->b3FindParentBBox(shape);
		if (result != null)
		{
			return result;
		}
	}
	return null;
}

b3_bool b3BBox::b3BacktraceRecompute(b3BBox *search)
{
	b3Item  *item;
	b3BBox  *bbox;
	b3_bool  result;

	// Found?
	if (search == this)
	{
		b3Recompute();
		return true;
	}

	// Search children
	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox   = (b3BBox *)item;
		result = bbox->b3BacktraceRecompute(search);
		if (result)
		{
			bbox->b3Recompute();
			return result;
		}
	}
	return false;
}

b3_bool b3Scene::b3BacktraceRecompute(b3BBox *search)
{
	b3Item  *item;
	b3BBox  *bbox;
	b3_bool  result;

	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox = (b3BBox *)item;
		if (bbox == search)
		{
			bbox->b3Recompute();
			return true;
		}

		if ((result = bbox->b3BacktraceRecompute(search)) != null)
		{
			bbox->b3Recompute();
			return result;
		}
	}
	return false;
}

b3BBox *b3BBox::b3ReadCOB(const char *filename)
{
	return b3COBReader::b3ReadCOB(filename);
}

b3BBox *b3BBox::b3ReadTGF(const char *filename)
{
	return b3TGFReader::b3ReadTGFBBox(filename);
}
