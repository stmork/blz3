/*
**
**      $Filename:      b3Special.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing special features
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

#include "blz3/raytrace/b3Raytrace.h"
#include "blz3/base/b3Aux.h"
#include "blz3/base/b3Matrix.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.42  2002/02/05 20:04:12  sm
**      - Added legend to print hard copy.
**
**      Revision 1.41  2002/02/04 17:18:01  sm
**      - Added Measurement to modeller info.
**
**      Revision 1.40  2002/02/03 21:42:30  sm
**      - Added measurement printing. The measure itself is missing yet.
**        The support is done in b3RenderView and CAppRenderView.
**      - Added support for units in b3ModellerInfo
**      - Cleaned up some accelerators. Now arrow keys are working
**        again. The del key is working correctly inside edit controls again.
**
**      Revision 1.39  2002/02/01 15:04:09  sm
**      - Prepared shapes for icon conversion
**      - Added to save selected/first visible item in
**        hierarchy dialog.
**      - Some print cleanups done.
**      - Fixed activation of b3SuperSample.
**
**      Revision 1.38  2002/01/16 16:17:13  sm
**      - Introducing object edit painting and acting.
**
**      Revision 1.37  2002/01/08 15:45:50  sm
**      - Added support for repeating CButtons for button movement/rotation mode.
**
**      Revision 1.36  2002/01/06 21:38:18  sm
**      - Nasty Un CR/LF
**      - b3Dir fix. Not tested, yet!
**      - make count/make count_all update
**
**      Revision 1.35  2002/01/06 16:30:47  sm
**      - Added Load/Save/Replace object
**      - Enhanced "New world"
**      - Added some non static methods to b3Dir (Un*x untested, yet!)
**      - Fixed missing sphere/ellipsoid south pole triangles
**      - Fixed Spline cylinder/ring triangle indexing
**
**      Revision 1.34  2001/12/31 11:05:18  sm
**      - Added TestData for testing Blizzard data structures for reading
**        and writing.
**      - Fixed bugs found with previous mentioned tool:
**        o Some b3AnimElement errors found to be fixed under Windows.
**        o b3TriangleShape destructor handled unchecked m_GridList pointer
**      - Changed some output levels in b3Light, b3Scene and b3ShadeXXX from
**        B3LOG_NORMAL to B3LOG_DEBUG.
**
**      Revision 1.33  2001/12/30 22:52:35  sm
**      - Made b3Scene::b3SetCamera() compatible to earlier versions.
**
**      Revision 1.32  2001/12/30 18:24:35  sm
**      - Added missing b3AnimControl class
**      - Some minor bug fixes done:
**        o Missed some SetModifiedFlag()
**        o b3Scene::b3SetCamera() calls added which now puts the
**          selected camera in front of the b3Special list so that Lines III
**          select it when reloading.
**
**      Revision 1.31  2001/12/30 14:16:58  sm
**      - Abstracted b3File to b3FileAbstract to implement b3FileMem (not done yet).
**      - b3Item writing implemented and updated all raytracing classes
**        to work properly.
**      - Cleaned up spline shapes and CSG shapes.
**      - Added b3Caustic class for compatibility reasons.
**
**      Revision 1.30  2001/12/28 18:55:09  sm
**      - Nasty un cr/lf
**
**      Revision 1.29  2001/12/25 18:52:39  sm
**      - Introduced CB3Dialogbar for dialogs opened any time.
**      - Fulcrum fixed with snap to grid
**
**      Revision 1.28  2001/12/23 08:57:22  sm
**      - Fixed recursive calling bug in b3IsObscured(...)
**      - Minor intersection optimazations done.
**
**      Revision 1.27  2001/12/21 16:46:16  sm
**      - New dialog for camera properties
**      - Done some bugfixes concerning CB3FloatEdit
**
**      Revision 1.26  2001/12/08 21:37:38  sm
**      - Added "No Gfx" support
**
**      Revision 1.25  2001/12/08 19:17:07  sm
**      - Added interactive focal length
**      - Added interactive twirl
**      - Added camera move left/right on right mouse button
**
**      Revision 1.24  2001/12/02 17:38:17  sm
**      - Removing nasty CR/LF
**      - Added b3ExtractExt()
**      - Added stricmp() for Un*x
**      - Fixed some defines
**
**      Revision 1.23  2001/12/02 15:43:49  sm
**      - Creation/Deletion/Editing of lights
**      - Creation/Deletion of cameras
**      - New toolbars introduced.
**
**      Revision 1.22  2001/11/28 16:54:55  sm
**      - Dialog for modeller info.
**
**      Revision 1.21  2001/11/26 17:16:37  sm
**      - Linux b3TimeSpan fix
**
**      Revision 1.20  2001/11/25 19:20:32  sm
**      - Added new acting methods:
**        o Camera move
**        o Camera turn around itself
**        o Camera rotate around fulcrum
**
**      Revision 1.19  2001/11/25 12:25:31  sm
**      - Completing some dialogs:
**        o super sampling
**        o distributed raytracing
**
**      Revision 1.18  2001/11/12 16:50:29  sm
**      - Scene properties dialog coding
**
**      Revision 1.17  2001/11/08 19:31:33  sm
**      - Nasty CR/LF removal!
**      - Added TGA/RGB8/PostScript image saving.
**      - Hoping to win Peter H. for powerful MFC programming...
**
**      Revision 1.16  2001/11/05 16:57:39  sm
**      - Creating demo scenes.
**      - Initializing some b3Item derived objects
**
**      Revision 1.15  2001/11/03 16:24:16  sm
**      - Added scene property dialog
**      - Added raytrace view title
**      - Added raytrace abort on button press
**
**      Revision 1.14  2001/11/02 19:05:36  sm
**      - Introducing time mearuring.
**      - Fixed wrong lens flare handling.
**
**      Revision 1.13  2001/10/29 19:34:02  sm
**      - Added new define B3_DELETE_BASE.
**      - Added support to abort raytrace processing.
**      - Added search path to world loading.
**      - Fixed super sampling.
**      - Fixed memory leak in raytracing row processing.
**
**      Revision 1.12  2001/10/21 16:55:21  sm
**      - Introducing lens flares.
**      - Introducing different modes of background computation.
**      - Introducing different types of row sampling.
**
**      Revision 1.11  2001/10/05 20:30:46  sm
**      - Introducing Mork and Phong shading.
**      - Using light source when shading
**
**      Revision 1.10  2001/10/03 20:17:56  sm
**      - Minor bugfixes
**
**      Revision 1.9  2001/10/03 18:46:45  sm
**      - Adding illumination and recursive raytracing
**
**      Revision 1.8  2001/09/30 15:53:19  sm
**      - Removing nasty CR/LF
**
**      Revision 1.7  2001/09/04 15:15:57  sm
**      - Added rotating objects
**
**      Revision 1.6  2001/09/01 15:54:54  sm
**      - Tidy up Size confusion in b3Item/b3World and derived classes
**      - Made (de-)activation of objects
**
**      Revision 1.5  2001/08/18 15:38:27  sm
**      - New action toolbar
**      - Added comboboxes for camera and lights (but not filled in)
**      - Drawing Fulcrum and view volume (Clipping plane adaption is missing)
**      - Some RenderObject redesignes
**      - Color selecting bug fix in RenderObject
**
**      Revision 1.4  2001/08/11 16:29:07  sm
**      - Nasty UnCR done
**      - Compiling but not running OpenGL under Unix
**
**      Revision 1.3  2001/08/11 15:59:59  sm
**      - Rendering cleaned up
**      - CWinApp/CMainFrm derived from Blizzard III classes
**        supporting more effective GUI.
**
**      Revision 1.2  2001/08/09 15:27:34  sm
**      - Following shapes are newly supported now:
**        o disk
**        o cylinder
**        o cone
**        o ellipsoid
**        o torus
**        o triangles
**      - Done some makefile fixes
**      - Everything is Windozable
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

void b3InitSpecial::b3Init()
{
	b3PrintF (B3LOG_DEBUG,"Registering special features...\n");

	b3Item::b3Register(&b3SuperSample::b3Init,  &b3SuperSample::b3Init,  SUPERSAMPLE4 );
	b3Item::b3Register(&b3CameraPart::b3Init,   &b3CameraPart::b3Init,   CAMERA );
	b3Item::b3Register(&b3Nebular::b3Init,      &b3Nebular::b3Init,      NEBULAR );
	b3Item::b3Register(&b3ModellerInfo::b3Init, &b3ModellerInfo::b3Init, LINES_INFO );
	b3Item::b3Register(&b3Animation::b3Init,    &b3Animation::b3Init,    ANIMATION );
	b3Item::b3Register(&b3Distribute::b3Init,   &b3Distribute::b3Init,   DISTRIBUTE );
	b3Item::b3Register(&b3LensFlare::b3Init,    &b3LensFlare::b3Init,    LENSFLARE );
	b3Item::b3Register(&b3Caustic::b3Init,      &b3Caustic::b3Init,      CAUSTIC );
}

/*************************************************************************
**                                                                      **
**                        Special base class                            **
**                                                                      **
*************************************************************************/

b3Special::b3Special(b3_size class_size,b3_u32 class_type) : b3Item(class_size,class_type)
{
}

b3Special::b3Special(b3_u32 class_type) : b3Item(sizeof(b3Special),class_type)
{
}

b3Special::b3Special(b3_u32 *src) : b3Item(src)
{
}


/*************************************************************************
**                                                                      **
**                        Super sampling                                **
**                                                                      **
*************************************************************************/

b3SuperSample::b3SuperSample(b3_u32 class_type) :
	b3Special(sizeof(b3SuperSample),class_type)
{
	m_Limit.a = 0;
	m_Limit.r = 0.2f;
	m_Limit.g = 0.2f;
	m_Limit.b = 0.2f;
	m_Active  = true;
}

b3SuperSample::b3SuperSample(b3_u32 *src) :
	b3Special(src)
{
	b3_color limit;

	b3InitColor(&limit);
	m_Limit.a = fabs(limit.a);
	m_Limit.r = fabs(limit.r);
	m_Limit.g = fabs(limit.g);
	m_Limit.b = fabs(limit.b);

	// This looks a little bit ugly but is for compatibility reasons.
	// This instantiation uses an easy way to determine activation.
	b3Activate((limit.r >= 0) && (limit.g >= 0) && (limit.b >= 0));
}

void b3SuperSample::b3Write()
{
	b3_color limit;

	// This looks a little bit ugly but is for compatibility reasons.
	// This instantiation uses an easy way to determine activation.
	if (m_Active)
	{
		limit.a =  fabs(m_Limit.a);
		limit.r =  fabs(m_Limit.r);
		limit.g =  fabs(m_Limit.g);
		limit.b =  fabs(m_Limit.b);
	}
	else
	{
		limit.a = -fabs(m_Limit.a);
		limit.r = -fabs(m_Limit.r);
		limit.g = -fabs(m_Limit.g);
		limit.b = -fabs(m_Limit.b);
	}
	b3StoreColor(&limit);
}

b3_bool b3SuperSample::b3IsActive()
{
	return m_Active;
}

void b3SuperSample::b3Activate(b3_bool flag)
{
	m_Active = flag;
}

/*************************************************************************
**                                                                      **
**                        Camera description                            **
**                                                                      **
*************************************************************************/

b3CameraPart::b3CameraPart(b3_u32 class_type) :
	b3Special(sizeof(b3CameraPart),class_type)
{
	b3Vector::b3Init(&m_EyePoint, 0,-200,  0);
	b3Vector::b3Init(&m_ViewPoint,0,-100,  0);
	b3Vector::b3Init(&m_Width,   50,   0,  0);
	b3Vector::b3Init(&m_Height,   0,  37.5,0);
	m_Flags = CAMERA_ACTIVE;
	m_CameraName[0] = 0;
}

b3CameraPart::b3CameraPart(b3_u32 *src) :
	b3Special(src)
{
	b3InitVector(&m_Width);
	b3InitVector(&m_Height);
	b3InitVector(&m_EyePoint);
	b3InitVector(&m_ViewPoint);
	m_Flags = b3InitInt();
	b3InitString(m_CameraName,B3_CAMERANAMELEN);
}

void b3CameraPart::b3Write()
{
	b3StoreVector(&m_Width);
	b3StoreVector(&m_Height);
	b3StoreVector(&m_EyePoint);
	b3StoreVector(&m_ViewPoint);
	b3StoreInt(m_Flags);
	b3StoreString(m_CameraName,B3_CAMERANAMELEN);
}

b3_bool b3CameraPart::b3IsActive()
{
	return (m_Flags & CAMERA_ACTIVE) != 0;
}

void b3CameraPart::b3Activate(b3_bool activate)
{
	if (activate)
	{
		m_Flags |= CAMERA_ACTIVE;
	}
	else
	{
		m_Flags &= (~CAMERA_ACTIVE);
	}
}

void b3CameraPart::b3ComputeFocalLength(b3_f64 focal_length)
{
	b3_vector  dir;

	b3Vector::b3Sub(&m_ViewPoint,&m_EyePoint,&dir);
	b3Vector::b3Normalize(&dir,focal_length);
	b3Vector::b3Add(&m_EyePoint,&dir,&m_ViewPoint);
}

void b3CameraPart::b3Orientate(
	b3_vector *eye,
	b3_vector *view,
	b3_f64     focal_length,
	b3_f64     width,
	b3_f64     height)
{
	b3_vector  up;
	b3_vector  dir;

	m_EyePoint = *eye;
	b3Vector::b3Sub(view,eye,&dir);
	b3Vector::b3Normalize(&dir,focal_length);
	b3Vector::b3Add(eye,&dir,&m_ViewPoint);
	b3Vector::b3Init(&up,0,0,1);
	b3Vector::b3CrossProduct(&dir,&up,&m_Width);
	b3Vector::b3CrossProduct(&m_Width,&dir,&m_Height);
	b3Vector::b3Normalize(&m_Width,width);
	b3Vector::b3Normalize(&m_Height,height);
}

void b3CameraPart::b3Overview(
	b3_vector *center,
	b3_vector *size,
	b3_f64     xAngle,
	b3_f64     yAngle)
{
	b3_vector eye;
	b3_f64    rad;

	rad    = 0.4 * b3Vector::b3Length(size);
	eye.x  = center->x + 8.0 * rad * cos(xAngle) * cos(yAngle);
	eye.y  = center->y + 8.0 * rad * sin(xAngle) * cos(yAngle);
	eye.z  = center->z + 8.0 * rad * sin(yAngle);

	b3Orientate(&eye,center,7.0 * rad,rad,rad);
}

void b3CameraPart::b3Transform(b3_matrix *transformation)
{
	b3MatrixVMul(transformation,&m_EyePoint, &m_EyePoint, true);
	b3MatrixVMul(transformation,&m_ViewPoint,&m_ViewPoint,true);
	b3MatrixVMul(transformation,&m_Width,    &m_Width,    false);
	b3MatrixVMul(transformation,&m_Height,   &m_Height,   false);
}

char *b3CameraPart::b3GetName()
{
	return m_CameraName;
}

/*************************************************************************
**                                                                      **
**                        Some infos for Lines III                      **
**                                                                      **
*************************************************************************/

static const b3_f64 unit_scale[B3_UNIT_MAX] =
{
	1.0,   // B3_UNIT_MM
	10.0,  // B3_UNIT_CM
	25.4,  // B3_UNIT_IN
	100.0, // B3_UNIT_DM
	308.4, // B3_UNIT_FT
	1000.0 // B3_UNIT_M
};

static const char *unit_descr[] =
{
	"mm","cm","in","dm","ft","m"
};

static const b3_u32 measure[B3_MEASURE_MAX - 1] =
{
	1,10,20,50,100,200,500,1000
};

b3ModellerInfo::b3ModellerInfo(b3_u32 class_type) :
	b3Special(sizeof(b3ModellerInfo),class_type)
{
	m_Center.x     = 0;
	m_Center.y     = 0;
	m_Center.z     = 0;
	m_StepMove.x   =
	m_StepMove.y   =
	m_StepMove.z   = 10;
	m_StepRotate.x =
	m_StepRotate.y =
	m_StepRotate.z = 15;
	b3SetUnit(B3_UNIT_CM);
	b3SetMeasure(100);
	b3SetMeasure(B3_MEASURE_100);
	m_Unit         = b3ScaleUnitToMM();
	m_GridMove     = 10;
	m_GridRot      = 15;
	m_GridActive   = true;
	m_AngleActive  = true;
}

b3ModellerInfo::b3ModellerInfo(b3_u32 *src) :
	b3Special(src)
{
	b3InitVector(&m_Center);
	m_GridMove     = b3InitFloat();
	m_GridRot      = b3InitFloat();
	m_ResizeFlag   = b3InitBool();
	m_BBoxTitles   = b3InitBool();
	m_GridActive   =
	m_AngleActive  = b3InitBool();
	m_CameraActive = b3InitBool();
	m_Flags        = B3_UNIT_CM;
	m_StepMove.x   =
	m_StepMove.y   =
	m_StepMove.z   = 10;
	m_StepRotate.x =
	m_StepRotate.y =
	m_StepRotate.z = 15;
	if (B3_PARSE_INDEX_VALID)
	{
		m_Flags    = b3InitInt();
		m_Unit     = b3InitFloat();	
		if (B3_PARSE_INDEX_VALID)
		{
			b3InitVector(&m_StepMove);
			b3InitVector(&m_StepRotate);
			m_AngleActive  = b3InitBool();
		}
	}
	if (b3GetMeasure() == 0)
	{
		b3SetMeasure(100);
		b3SetMeasure(B3_MEASURE_100);
	}
	m_Unit = b3ScaleUnitToMM();
}

void b3ModellerInfo::b3Write()
{
	m_Unit = b3ScaleUnitToMM();
	b3StoreVector(&m_Center);
	b3StoreFloat(m_GridMove  );
	b3StoreFloat(m_GridRot   );
	b3StoreBool (m_ResizeFlag);
	b3StoreBool (m_BBoxTitles);
	b3StoreBool (m_GridActive);
	b3StoreBool (m_CameraActive);
	b3StoreInt  (m_Flags);
	b3StoreFloat(m_Unit);
	b3StoreVector(&m_StepMove);
	b3StoreVector(&m_StepRotate);
	b3StoreBool(m_AngleActive);
}

void b3ModellerInfo::b3SnapToGrid(b3_vector *vector)
{
	if (m_GridActive)
	{
		vector->x = floor(vector->x / m_GridMove + 0.5) * m_GridMove;
		vector->y = floor(vector->y / m_GridMove + 0.5) * m_GridMove;
		vector->z = floor(vector->z / m_GridMove + 0.5) * m_GridMove;
	}
}

void b3ModellerInfo::b3SnapToAngle(b3_f64 &angle)
{
	if (m_AngleActive)
	{
		// Convert to radians
		b3_f64 GridStep = m_GridRot * M_PI / 180.0;

		angle = floor(angle / GridStep + 0.5) * GridStep;
	}
}

b3_f64 b3ModellerInfo::b3ScaleUnitToMM()
{
	return unit_scale[m_Flags & B3_UNIT_MASK];
}

const char *b3ModellerInfo::b3GetUnitDescr()
{
	return unit_descr[b3GetUnit()];
}

b3_unit b3ModellerInfo::b3GetUnit()
{
	return (b3_unit)(m_Flags & B3_UNIT_MASK);
}

void b3ModellerInfo::b3SetUnit(b3_unit unit)
{
	m_Flags &= (~B3_UNIT_MASK);
	m_Flags |= (unit << B3_UNIT_SHIFT);
}

b3_u32 b3ModellerInfo::b3GetMeasure(b3_bool force_custom_value)
{
	b3_measure type;

	type = b3GetMeasureType();
	return (((type == B3_MEASURE_CUSTOM) || (force_custom_value)) ?
		((m_Flags & B3_CUSTOM_MEASURE_MASK) >> B3_CUSTOM_MEASURE_SHIFT) :
		measure[type]);
}

b3_measure b3ModellerInfo::b3GetMeasureType()
{
	return (b3_measure)((m_Flags & B3_MEASURE_MASK) >> B3_MEASURE_SHIFT);
}

void b3ModellerInfo::b3SetMeasure(b3_measure measure)
{
	m_Flags &= (~B3_MEASURE_MASK);
	m_Flags |= (measure << B3_MEASURE_SHIFT);
}

void b3ModellerInfo::b3SetMeasure(b3_u32 measure)
{
	m_Flags &= (~(B3_MEASURE_MASK|B3_CUSTOM_MEASURE_MASK));
	m_Flags |= (
		(B3_MEASURE_CUSTOM << B3_MEASURE_SHIFT) |
		(measure << B3_CUSTOM_MEASURE_SHIFT));
}

/*************************************************************************
**                                                                      **
**                        Nebular                                       **
**                                                                      **
*************************************************************************/

b3Nebular::b3Nebular(b3_u32 class_type) :
	b3Special(sizeof(b3Nebular),class_type)
{
	m_NebularVal = -100.0;
	b3Color::b3Init(&m_NebularColor,0,0.8f,0.9f,1.0f);
}

b3Nebular::b3Nebular(b3_u32 *src) :
	b3Special(src)
{
	b3InitColor(&m_NebularColor);
	m_NebularVal = b3InitFloat();
}

void b3Nebular::b3Write()
{
	b3StoreColor(&m_NebularColor);
	b3StoreFloat(m_NebularVal);
}

b3_bool b3Nebular::b3Prepare()
{
	if (m_NebularVal > 0)
	{
		m_NebularDenom = log(2.0) / m_NebularVal;
	}
	return true;
}

b3_bool b3Nebular::b3IsActive()
{
	return m_NebularVal > 0;
}

void b3Nebular::b3Activate(b3_bool flag)
{
	m_NebularVal = (flag ? fabs(m_NebularVal) : -fabs(m_NebularVal));
}

void b3Nebular::b3GetNebularColor(b3_color *result)
{
	*result = m_NebularColor;
}

void b3Nebular::b3ComputeNebular(
	b3_color *input,
	b3_color *result,
	b3_f64    distance)
{
	b3_f64 NebularIndex = exp (-distance * m_NebularDenom);
	b3_f64 NebularDenom = 1 - NebularIndex;

	result->a = NebularIndex * input->a + NebularDenom * m_NebularColor.a;
	result->r = NebularIndex * input->r + NebularDenom * m_NebularColor.r;
	result->g = NebularIndex * input->g + NebularDenom * m_NebularColor.g;
	result->b = NebularIndex * input->b + NebularDenom * m_NebularColor.b;
}

/*************************************************************************
**                                                                      **
**                        Lens flares                                   **
**                                                                      **
*************************************************************************/

b3LensFlare::b3LensFlare(b3_u32 class_type) :
	b3Special(sizeof(b3LensFlare),class_type)
{
	b3Activate(false);
	b3Color::b3Init(&m_Color,0,1,1,1);
}

b3LensFlare::b3LensFlare(b3_u32 *src) :
	b3Special(src)
{
	m_Flags = b3InitInt();
	b3InitColor(&m_Color);
	m_Expon = b3InitFloat();
}

void b3LensFlare::b3Write()
{
	b3StoreInt(m_Flags);
	b3StoreColor(&m_Color);
	b3StoreFloat(m_Expon);
}

b3_bool b3LensFlare::b3IsActive()
{
	return (m_Flags & LENSFLARE_ACTIVE) != 0;
}

void b3LensFlare::b3Activate(b3_bool flag)
{
	if (flag)
	{
		m_Flags |=   LENSFLARE_ACTIVE;
	}
	else
	{
		m_Flags &= (~LENSFLARE_ACTIVE);
	}
}

/*************************************************************************
**                                                                      **
**                        Distributed raytracing                        **
**                                                                      **
*************************************************************************/

b3Distribute::b3Distribute(b3_u32 class_type) :
	b3Special(sizeof(b3Distribute),class_type)
{
	SAMPLE_SET_FLAGS(this,0);
	SAMPLE_SET_TYPE(this,SAMPLE_REGULAR);
	m_PixelAperture   = FILTER_BOX;
	m_FrameAperture   = FILTER_GAUSS;
	m_SamplesPerPixel = 5;
	m_SamplesPerFrame = 5;
	m_DepthOfField    = 0;
}

b3Distribute::b3Distribute(b3_u32 *src) :
	b3Special(src)
{
	m_Type            = b3InitInt();
	m_SamplesPerPixel = b3InitInt();
	m_SamplesPerFrame = b3InitInt();
	if (B3_PARSE_INDEX_VALID)
	{
		m_DepthOfField    = b3InitFloat();
		m_PixelAperture   = (b3_filter)b3InitInt();
		m_FrameAperture   = (b3_filter)b3InitInt();
	}
}

void b3Distribute::b3Write()
{
	b3StoreInt(m_Type);
	b3StoreCount(m_SamplesPerPixel);
	b3StoreCount(m_SamplesPerFrame);
	b3StoreFloat(m_DepthOfField);
	b3StoreInt(m_PixelAperture);
	b3StoreInt(m_FrameAperture);
}

/*************************************************************************
**                                                                      **
**                        Animation root definition                     **
**                                                                      **
*************************************************************************/

b3Animation::b3Animation(b3_u32 class_type) :
	b3Special(sizeof(b3Animation),class_type)
{
	m_Heads[0].b3InitBase(CLASS_ANIMATION);
}

b3Animation::b3Animation(b3_u32 *src) :
	b3Special(src)
{
	start           = b3InitFloat();
	end             = b3InitFloat();
	time            = b3InitFloat();
	neutral         = b3InitFloat();
	framesPerSecond = b3InitInt();
	flags           = b3InitInt();;

	if (B3_PARSE_INDEX_VALID)
	{
		// OK, the following values are only for "Lines"
		frames     = b3InitInt();
		tracks     = b3InitInt();
		trackIndex = b3InitInt();
		frameIndex = b3InitInt();
		WTracks    = b3InitInt();
		WFrames    = b3InitInt();
		Element    = (b3AnimElement *)b3InitNull();
	}
}

void b3Animation::b3Write()
{
	b3StoreFloat(start);
	b3StoreFloat(end);
	b3StoreFloat(time);
	b3StoreFloat(neutral);
	b3StoreInt(framesPerSecond);
	b3StoreInt(flags);

	// OK, the following values are only for "Lines"
	b3StoreInt(frames);
	b3StoreInt(tracks);
	b3StoreInt(trackIndex);
	b3StoreInt(frameIndex);
	b3StoreInt(WTracks);
	b3StoreInt(WFrames);
	b3StorePtr(Element);
}

/*************************************************************************
**                                                                      **
**                        b3Caustic definition                          **
**                                                                      **
*************************************************************************/

b3Caustic::b3Caustic(b3_u32 class_type) :
	b3Special(sizeof(b3Caustic),class_type)
{
}

b3Caustic::b3Caustic(b3_u32 *src) :
	b3Special(src)
{
	m_Flags      = b3InitInt();
	m_NumPhotons = b3InitInt();
	m_TraceDepth = b3InitInt();
}

void b3Caustic::b3Write()
{
	b3StoreInt  (m_Flags);
	b3StoreCount(m_NumPhotons);
	b3StoreCount(m_TraceDepth);
}
