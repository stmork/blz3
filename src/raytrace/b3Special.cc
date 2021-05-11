/*
**
**	$Filename:	b3Special.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing special features
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/base/b3Aux.h"
#include "blz3/raytrace/b3Animation.h"
#include "blz3/raytrace/b3Special.h"

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

void b3Special::b3Register()
{
	b3PrintF(B3LOG_DEBUG, "Registering special features...\n");

	b3Item::b3Register(&b3SuperSample::b3StaticInit,     &b3SuperSample::b3StaticInit,     SUPERSAMPLE4);
	b3Item::b3Register(&b3CameraPart::b3StaticInit,      &b3CameraPart::b3StaticInit,      CAMERA);
	b3Item::b3Register(&b3Nebular::b3StaticInit,         &b3Nebular::b3StaticInit,         NEBULAR);
	b3Item::b3Register(&b3ModellerInfo::b3StaticInit,    &b3ModellerInfo::b3StaticInit,    LINES_INFO);
	b3Item::b3Register(&b3Animation::b3StaticInit,       &b3Animation::b3StaticInit,       ANIMATION);
	b3Item::b3Register(&b3Distribute::b3StaticInit,      &b3Distribute::b3StaticInit,      DISTRIBUTE);
	b3Item::b3Register(&b3LensFlare::b3StaticInit,       &b3LensFlare::b3StaticInit,       LENSFLARE);
	b3Item::b3Register(&b3Caustic::b3StaticInit,         &b3Caustic::b3StaticInit,         CAUSTIC);
	b3Item::b3Register(&b3CloudBackground::b3StaticInit, &b3CloudBackground::b3StaticInit, CLOUDS);
}

/*************************************************************************
**                                                                      **
**                        Special base class                            **
**                                                                      **
*************************************************************************/

b3Special::b3Special(b3_size class_size, b3_u32 class_type) : b3Item(class_size, class_type)
{
}

b3Special::b3Special(b3_u32 class_type) : b3Item(sizeof(b3Special), class_type)
{
}

b3Special::b3Special(b3_u32 * src) : b3Item(src)
{
}

/*************************************************************************
**                                                                      **
**                        Super sampling                                **
**                                                                      **
*************************************************************************/

b3SuperSample::b3SuperSample(b3_u32 class_type) :
	b3Special(sizeof(b3SuperSample), class_type)
{
	m_Limit.b3Init(0.2f, 0.2f, 0.2f);
	m_Active  = true;
}

b3SuperSample::b3SuperSample(b3_u32 * src) :
	b3Special(src)
{
	b3_color limit;

	b3InitColor(&limit);
	m_Limit.b3Init(
		fabs(limit.r),
		fabs(limit.g),
		fabs(limit.b),
		fabs(limit.a));

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
		limit.a =  fabs(m_Limit[b3Color::A]);
		limit.r =  fabs(m_Limit[b3Color::R]);
		limit.g =  fabs(m_Limit[b3Color::G]);
		limit.b =  fabs(m_Limit[b3Color::B]);
	}
	else
	{
		limit.a = -fabs(m_Limit[b3Color::A]);
		limit.r = -fabs(m_Limit[b3Color::R]);
		limit.g = -fabs(m_Limit[b3Color::G]);
		limit.b = -fabs(m_Limit[b3Color::B]);
	}
	b3StoreColor(&limit);
}

bool b3SuperSample::b3IsActive() const
{
	return m_Active;
}

void b3SuperSample::b3Activate(bool flag)
{
	m_Active = flag;
}

/*************************************************************************
**                                                                      **
**                        Camera description                            **
**                                                                      **
*************************************************************************/

b3CameraPart::b3CameraPart(b3_u32 class_type) :
	b3Special(sizeof(b3CameraPart), class_type)
{
	b3Vector::b3Init(&m_EyePoint, 0, -200,  0);
	b3Vector::b3Init(&m_ViewPoint, 0, -100,  0);
	b3Vector::b3Init(&m_Width,   50,   0,  0);
	b3Vector::b3Init(&m_Height,   0,  37.5, 0);
	m_Flags = CAMERA_ACTIVE;
	m_CameraName[0] = 0;
}

b3CameraPart::b3CameraPart(b3_u32 * src) :
	b3Special(src)
{
	b3InitVector(&m_Width);
	b3InitVector(&m_Height);
	b3InitVector(&m_EyePoint);
	b3InitVector(&m_ViewPoint);
	m_Flags = b3InitInt();
	b3InitString(m_CameraName, B3_CAMERANAMELEN);
}

void b3CameraPart::b3Write()
{
	b3StoreVector(&m_Width);
	b3StoreVector(&m_Height);
	b3StoreVector(&m_EyePoint);
	b3StoreVector(&m_ViewPoint);
	b3StoreInt(m_Flags);
	b3StoreString(m_CameraName, B3_CAMERANAMELEN);
}

bool b3CameraPart::b3IsActive() const
{
	return (m_Flags & CAMERA_ACTIVE) != 0;
}

void b3CameraPart::b3Activate(bool activate)
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

	b3Vector::b3Sub(&m_ViewPoint, &m_EyePoint, &dir);
	b3Vector::b3Normalize(&dir, focal_length);
	b3Vector::b3Add(&m_EyePoint, &dir, &m_ViewPoint);
}

void b3CameraPart::b3ScaleFocalLength(b3_f64 factor)
{
	b3_vector  dir;

	b3Vector::b3Sub(&m_ViewPoint, &m_EyePoint, &dir);
	b3Vector::b3Scale(&dir, factor);
	b3Vector::b3Add(&m_EyePoint, &dir, &m_ViewPoint);
}

void b3CameraPart::b3Orientate(
	b3_vector * eye,
	b3_vector * view,
	b3_f64     focal_length,
	b3_f64     width,
	b3_f64     height)
{
	b3_vector  up;
	b3_vector  dir;

	m_EyePoint = *eye;
	b3Vector::b3Sub(view, eye, &dir);
	b3Vector::b3Normalize(&dir, focal_length);
	b3Vector::b3Add(eye, &dir, &m_ViewPoint);
	if ((dir.x == 0) && (dir.y == 0))
	{
		b3Vector::b3Init(&up, 1, 0, 0);
	}
	else
	{
		b3Vector::b3Init(&up, 0, 0, 1);
	}
	b3Vector::b3CrossProduct(&dir,     &up,  &m_Width);
	b3Vector::b3CrossProduct(&m_Width, &dir, &m_Height);
	b3Vector::b3Normalize(&m_Width,  width);
	b3Vector::b3Normalize(&m_Height, height);
}

void b3CameraPart::b3ComputeAngles(b3_f64 & xAngle, b3_f64 & yAngle) const
{
	xAngle = atan2(
			m_EyePoint.y - m_ViewPoint.y,
			m_EyePoint.x - m_ViewPoint.x);

	b3_f64 x = m_EyePoint.x - m_ViewPoint.x;
	b3_f64 y = m_EyePoint.y - m_ViewPoint.y;
	b3_f64 z = m_EyePoint.z - m_ViewPoint.z;
	yAngle = atan2(z, sqrt(x * x + y * y));
}

void b3CameraPart::b3Overview(
	b3_vector * center,
	b3_vector * size,
	b3_f64      xAngle,
	b3_f64      yAngle)
{
	b3_vector eye;
	b3_f64    rad;

	rad    = 0.4 * b3Vector::b3Length(size);
	eye.x  = center->x + 8.0 * rad * cos(xAngle) * cos(yAngle);
	eye.y  = center->y + 8.0 * rad * sin(xAngle) * cos(yAngle);
	eye.z  = center->z + 8.0 * rad * sin(yAngle);

	b3Orientate(&eye, center, 7.0 * rad, rad, rad);
}

void b3CameraPart::b3Transform(b3_matrix * transformation)
{
	b3Matrix::b3VMul(transformation, &m_EyePoint,  &m_EyePoint, true);
	b3Matrix::b3VMul(transformation, &m_ViewPoint, &m_ViewPoint, true);
	b3Matrix::b3VMul(transformation, &m_Width,     &m_Width,    false);
	b3Matrix::b3VMul(transformation, &m_Height,    &m_Height,   false);
}

const char * b3CameraPart::b3GetName() const
{
	return m_CameraName;
}

void b3CameraPart::b3SetName(const char * name)
{
	b3Mem::b3StrCpy(m_CameraName, name, sizeof(m_CameraName));
}

bool b3CameraPart::b3Prepare(b3_preparation_info * prep_info B3_UNUSED)
{
	b3PrintF(B3LOG_DEBUG, "Camera %s, %sactive\n", b3GetName(), b3IsActive() ? "" : "not ");
	b3PrintF(B3LOG_DEBUG, " W: %3.5f\n", b3Vector::b3Length(&m_Width));
	b3PrintF(B3LOG_DEBUG, " H: %3.5f\n", b3Vector::b3Length(&m_Height));
	b3PrintF(B3LOG_DEBUG, " F: %3.5f\n", b3GetFocalLength());
	return true;
}

b3_f64 b3CameraPart::b3GetFocalLength() const
{
	return b3Vector::b3Distance(&m_ViewPoint, &m_EyePoint);
}

b3_f64 b3CameraPart::b3GetTwirl() const
{
	b3_vector ViewDir, Right, Vup;

	// compute view up vector
	ViewDir.x = m_ViewPoint.x - m_EyePoint.x;
	ViewDir.y = m_ViewPoint.y - m_EyePoint.y;
	ViewDir.z = m_ViewPoint.z - m_EyePoint.z;

	if ((ViewDir.x == 0) && (ViewDir.y == 0))
	{
		return 0;
	}

	Right.x   =  ViewDir.y;
	Right.y   = -ViewDir.x;
	Right.z   =  0;


	// this is real VUP
	b3Vector::b3CrossProduct(&Right, &ViewDir, &Vup);

	// compute twirl
	return asin((
				Vup.x * m_Width.x +
				Vup.y * m_Width.y +
				Vup.z * m_Width.z) /
			(b3Vector::b3Length(&m_Width) * b3Vector::b3Length(&Vup)));
}

void b3CameraPart::b3SetTwirl(b3_f64 twirl)
{
	b3_line   RotLine;
	b3_matrix RotMatrix;
	b3_f64    width  = b3Vector::b3Length(&m_Width);
	b3_f64    height = b3Vector::b3Length(&m_Height);
	b3_f64    focal  = b3GetFocalLength();
	b3_f64    factor;

	RotLine.pos   = m_EyePoint;
	RotLine.dir.x = m_ViewPoint.x - m_EyePoint.x;
	RotLine.dir.y = m_ViewPoint.y - m_EyePoint.y;
	RotLine.dir.z = m_ViewPoint.z - m_EyePoint.z;

	factor = focal / b3Vector::b3Length(&RotLine.dir);
	m_ViewPoint.x = m_EyePoint.x + factor * RotLine.dir.x;
	m_ViewPoint.y = m_EyePoint.y + factor * RotLine.dir.y;
	m_ViewPoint.z = m_EyePoint.z + factor * RotLine.dir.z;

	if ((RotLine.dir.x == 0) && (RotLine.dir.y == 0))
	{
		m_Width.x =  1;
		m_Width.y =  0;
		m_Width.z =  0;
	}
	else
	{
		m_Width.x =  RotLine.dir.y;
		m_Width.y = -RotLine.dir.x;
		m_Width.z =  0;
	}

	// now compute new projection plane with
	// specified parameters.
	b3Vector::b3CrossProduct(&m_Width, &RotLine.dir, &m_Height);
	b3Vector::b3Normalize(&m_Width, width);
	b3Vector::b3Normalize(&m_Height, height);

	// set to the old twirl
	if (twirl != 0)
	{
		b3Matrix::b3RotateVector(nullptr, &RotMatrix, &RotLine, -twirl);
		b3Matrix::b3VMul(&RotMatrix, &m_Width,  &m_Width, false);
		b3Matrix::b3VMul(&RotMatrix, &m_Height, &m_Height, false);
	}
}

/*************************************************************************
**                                                                      **
**                        Some infos for Lines III                      **
**                                                                      **
*************************************************************************/

const b3_f64 b3ModellerInfo::m_UnitScaleTable[B3_UNIT_MAX] =
{
	1.0,   // B3_UNIT_MM
	10.0,  // B3_UNIT_CM
	25.4,  // B3_UNIT_IN
	100.0, // B3_UNIT_DM
	308.4, // B3_UNIT_FT
	1000.0 // B3_UNIT_M
};

const char * b3ModellerInfo::m_UnitDescrTable[B3_UNIT_MAX] =
{
	"mm", "cm", "in", "dm", "ft", "m"
};

const b3_u32 b3ModellerInfo::m_MeasureTable[B3_MEASURE_MAX - 1] =
{
	1, 10, 20, 50, 100, 200, 500, 1000
};

b3ModellerInfo::b3ModellerInfo(b3_u32 class_type) :
	b3Special(sizeof(b3ModellerInfo), class_type)
{
	m_Center.x         = 0;
	m_Center.y         = 0;
	m_Center.z         = 0;
	m_StepMove.x       =
		m_StepMove.y       =
			m_StepMove.z       = 10;
	m_StepRotate.x     =
		m_StepRotate.y     =
			m_StepRotate.z     = 15;
	m_Unit             = B3_UNIT_CM;
	m_Measure          = B3_MEASURE_100;
	m_ResizeFlag       = true; // Not used in Blizzard III
	m_CustomMeasure    = 100;
	m_GridMove         = 10;
	m_GridRot          = 15;
	m_GridActive       = true;
	m_AngleGridCamera  = false;
	m_AngleGridObjects = true;
	m_AngleGrid        = (m_AngleGridCamera << B3_ANGLE_GRID_CAMERA_BIT) | (m_AngleGridObjects << B3_ANGLE_GRID_OBJECT_BIT);
	m_UseSceneLights   = false;
}

b3ModellerInfo::b3ModellerInfo(b3_u32 * src) :
	b3Special(src)
{
	b3InitVector(&m_Center);
	m_GridMove     = b3InitFloat();
	m_GridRot      = b3InitFloat();
	m_ResizeFlag   = b3InitBool();
	m_ResizeFlag   = true;
	m_BBoxTitles   = b3InitBool();
	m_GridActive   = b3InitBool();
	m_CameraActive = b3InitBool();
	m_AngleGrid    = B3_ANGLE_GRID_OBJECT;
	m_StepMove.x   =
		m_StepMove.y   =
			m_StepMove.z   = 10;
	m_StepRotate.x =
		m_StepRotate.y =
			m_StepRotate.z = 15;
	m_Flags        = 0;
	if (B3_PARSE_INDEX_VALID)
	{
		m_Flags    = b3InitInt();
		b3InitFloat();
		if (B3_PARSE_INDEX_VALID)
		{
			b3InitVector(&m_StepMove);
			b3InitVector(&m_StepRotate);
			m_AngleGrid        = b3InitInt();
		}
	}
	m_AngleGridObjects = (m_AngleGrid & B3_ANGLE_GRID_OBJECT) != 0;
	m_AngleGridCamera  = (m_AngleGrid & B3_ANGLE_GRID_CAMERA) != 0;
	m_Unit           = (b3_unit)((m_Flags & B3_UNIT_MASK)           >> B3_UNIT_SHIFT);
	m_Measure        = (b3_measure)((m_Flags & B3_MEASURE_MASK)        >> B3_MEASURE_SHIFT);
	m_CustomMeasure  = (m_Flags & B3_CUSTOM_MEASURE_MASK) >> B3_CUSTOM_MEASURE_SHIFT;
	m_UseSceneLights = (m_Flags & B3_USE_SCENE_LIGHTS) != 0;

	if (m_CustomMeasure == 0)
	{
		m_CustomMeasure = 100;
		m_Measure       = B3_MEASURE_100;
	}
}

void b3ModellerInfo::b3Write()
{
	m_Flags =
		(m_Unit << B3_UNIT_SHIFT) |
		(m_Measure << B3_MEASURE_SHIFT) |
		(m_CustomMeasure << B3_CUSTOM_MEASURE_SHIFT);
	if (m_UseSceneLights)
	{
		m_Flags |= B3_USE_SCENE_LIGHTS;
	}
	m_AngleGrid =
		(m_AngleGridCamera << B3_ANGLE_GRID_CAMERA_BIT) |
		(m_AngleGridObjects << B3_ANGLE_GRID_OBJECT_BIT);

	b3StoreVector(&m_Center);
	b3StoreFloat(m_GridMove);
	b3StoreFloat(m_GridRot);
	b3StoreBool(m_ResizeFlag);
	b3StoreBool(m_BBoxTitles);
	b3StoreBool(m_GridActive);
	b3StoreBool(m_CameraActive);
	b3StoreInt(m_Flags);
	b3StoreFloat(b3ScaleUnitToMM());
	b3StoreVector(&m_StepMove);
	b3StoreVector(&m_StepRotate);
	b3StoreInt(m_AngleGrid);
}

void b3ModellerInfo::b3SnapToGrid(b3_vector * vector) const
{
	if (m_GridActive)
	{
		vector->x = floor(vector->x / m_GridMove + 0.5) * m_GridMove;
		vector->y = floor(vector->y / m_GridMove + 0.5) * m_GridMove;
		vector->z = floor(vector->z / m_GridMove + 0.5) * m_GridMove;
	}
}

void b3ModellerInfo::b3SnapToCameraAngle(b3_f64 & angle) const
{
	b3Snap(angle, m_AngleGridCamera);
}

void b3ModellerInfo::b3SnapToObjectAngle(b3_f64 & angle) const
{
	b3Snap(angle, m_AngleGridObjects);
}

void b3ModellerInfo::b3Snap(b3_f64 & angle, bool activation) const
{
	if (activation)
	{
		// Convert to radians
		b3_f64 GridStep = m_GridRot * M_PI / 180.0;

		angle = floor(angle / GridStep + 0.5) * GridStep;
	}
}

b3_f64 b3ModellerInfo::b3ScaleUnitToMM() const
{
	return m_UnitScaleTable[m_Unit];
}

const char * b3ModellerInfo::b3GetUnitDescr() const
{
	return m_UnitDescrTable[m_Unit];
}

b3_u32 b3ModellerInfo::b3GetMeasure(bool force_custom_value) const
{
	return ((m_Measure == B3_MEASURE_CUSTOM) || (force_custom_value)) ?
		m_CustomMeasure : m_MeasureTable[m_Measure];
}

void b3ModellerInfo::b3SetMeasure(b3_measure measure)
{
	m_Measure = measure;
}

void b3ModellerInfo::b3SetMeasure(b3_u32 measure)
{
	m_CustomMeasure = measure;
	m_Measure       = B3_MEASURE_CUSTOM;
}

/*************************************************************************
**                                                                      **
**                        Nebular                                       **
**                                                                      **
*************************************************************************/

b3Nebular::b3Nebular(b3_u32 class_type) :
	b3Special(sizeof(b3Nebular), class_type)
{
	m_NebularVal = -100.0;
	m_NebularColor.b3Init(0.8f, 0.9f, 1.0f);
}

b3Nebular::b3Nebular(b3_u32 * src) :
	b3Special(src)
{
	b3InitColor(m_NebularColor);
	m_NebularVal = b3InitFloat();
}

void b3Nebular::b3Write()
{
	b3StoreColor(m_NebularColor);
	b3StoreFloat(m_NebularVal);
}

bool b3Nebular::b3Prepare(b3_preparation_info * prep_info B3_UNUSED)
{
	if (m_NebularVal > 0)
	{
		m_NebularDenom = log(2.0) / m_NebularVal;
	}
	return true;
}

bool b3Nebular::b3IsActive() const
{
	return m_NebularVal > 0;
}

void b3Nebular::b3Activate(bool flag)
{
	m_NebularVal = (flag ? fabs(m_NebularVal) : -fabs(m_NebularVal));
}

void b3Nebular::b3GetNebularColor(b3Color & result) const
{
	result = m_NebularColor;
}

void b3Nebular::b3ComputeNebular(
	const b3Color & input, b3Color & result,
	const b3_f64    distance) const
{
	b3_f64 NebularIndex = exp(-distance * m_NebularDenom);

	result = b3Color::b3Mix(m_NebularColor, input, NebularIndex);
}

/*************************************************************************
**                                                                      **
**                        Lens flares                                   **
**                                                                      **
*************************************************************************/

b3LensFlare::b3LensFlare(b3_u32 class_type) :
	b3Special(sizeof(b3LensFlare), class_type)
{
	b3Activate(false);
	m_Color.b3Init(1.0, 1.0, 0.9f);
	m_Expon = 13;
}

b3LensFlare::b3LensFlare(b3_u32 * src) :
	b3Special(src)
{
	m_Flags = b3InitInt();
	b3InitColor(m_Color);
	m_Expon = b3InitFloat();
}

void b3LensFlare::b3Write()
{
	b3StoreInt(m_Flags);
	b3StoreColor(m_Color);
	b3StoreFloat(m_Expon);
}

bool b3LensFlare::b3IsActive()
{
	return (m_Flags & LENSFLARE_ACTIVE) != 0;
}

void b3LensFlare::b3Activate(bool flag)
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
	b3Special(sizeof(b3Distribute), class_type)
{
	SAMPLE_SET_FLAGS(this, 0);
	SAMPLE_SET_TYPE(this, SAMPLE_REGULAR);
	m_PixelAperture   = B3_FILTER_BOX;
	m_FrameAperture   = B3_FILTER_GAUSS;
	m_SamplesPerPixel =  5;
	m_SamplesPerFrame = 64;
	m_DepthOfField    =  0;
	m_FilterPixel     = nullptr;
	m_FilterFrame     = nullptr;
}

b3Distribute::b3Distribute(b3_u32 * src) :
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
	else
	{
		m_DepthOfField  = 0;
		m_PixelAperture = B3_FILTER_BOX;
		m_FrameAperture = B3_FILTER_GAUSS;
	}
	m_FilterPixel     = nullptr;
	m_FilterFrame     = nullptr;
}

b3Distribute::~b3Distribute()
{
	if (m_FilterPixel != nullptr)
	{
		delete m_FilterPixel;
	}

	if (m_FilterFrame != nullptr)
	{
		delete m_FilterFrame;
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

bool b3Distribute::b3IsActive()
{
	return (m_Type & SAMPLE_SUPERSAMPLE) != 0;
}

bool b3Distribute::b3IsMotionBlur()
{
	return b3IsActive() && ((m_Type & SAMPLE_MOTION_BLUR) != 0);
}

void b3Distribute::b3PrepareAnimation(b3_res xSize, b3Animation * animation)
{
	b3_f32    *   samples;
	b3_f64        start, step;
	b3_coord      sx, sy;
	b3_coord      spp;
	b3_sample     type;

	if (m_FilterPixel != nullptr)
	{
		delete m_FilterPixel;
	}
	m_FilterPixel = b3Filter::b3New(m_PixelAperture);

	if (m_FilterFrame != nullptr)
	{
		delete m_FilterFrame;
	}
	m_FilterFrame = b3Filter::b3New(m_FrameAperture);

	type    = SAMPLE_GET_TYPE(this);
	spp     = m_SamplesPerPixel;
	m_SPP   = type == SAMPLE_SEPARATED ? spp : spp * spp;

	m_Samples  = b3TypedAlloc<b3_f32>(xSize * m_SPP * 2);
	if (m_Samples == nullptr)
	{
		B3_THROW(b3WorldException, B3_WORLD_MEMORY);
	}

	samples = m_Samples;
	start   = 1.0;
	step    = 2.0 / spp;
	for (b3_coord i = 0; i < xSize; i++)
	{
		switch (type)
		{
		case SAMPLE_REGULAR:
			start = 1.0 - step * 0.5;
			for (sy = 0; sy < spp; sy++)
			{
				for (sx = 0; sx < spp; sx++)
				{
					*samples++ = m_FilterPixel->b3InvIntegral(sx * step - start);
					*samples++ = m_FilterPixel->b3InvIntegral(sy * step - start);
				}
			}
			break;

		case SAMPLE_RANDOM:
			for (sy = 0; sy < spp; sy++)
			{
				for (sx = 0; sx < spp; sx++)
				{
					*samples++ = m_FilterPixel->b3InvIntegral(B3_FRAN(2.0) - 1.0);
					*samples++ = m_FilterPixel->b3InvIntegral(B3_FRAN(2.0) - 1.0);
				}
			}
			break;

		case SAMPLE_JITTER:
			for (sy = 0; sy < spp; sy++)
			{
				for (sx = 0; sx < spp; sx++)
				{
					*samples++ = m_FilterPixel->b3InvIntegral(B3_FRAN(step) - sx * step - start);
					*samples++ = m_FilterPixel->b3InvIntegral(B3_FRAN(step) - sy * step - start);
				}
			}
			break;

		case SAMPLE_SEMI_JITTER:
			start = 1.0 - step * 0.25;
			for (sy = 0; sy < spp; sy++)
			{
				for (sx = 0; sx < spp; sx++)
				{
					*samples++ = m_FilterPixel->b3InvIntegral(B3_FRAN(step * 0.5) - sx * step - start);
					*samples++ = m_FilterPixel->b3InvIntegral(B3_FRAN(step * 0.5) - sy * step - start);
				}
			}
			break;

		case SAMPLE_SEPARATED:
			for (sx = 0; sx < spp; sx++)
			{
				*samples++ = m_FilterPixel->b3InvIntegral(B3_FRAN(2.0) - B3_IRAN(spp) * step - start);
				*samples++ = m_FilterPixel->b3InvIntegral(B3_FRAN(2.0) - B3_IRAN(spp) * step - start);
			}
			break;
		}
	}

	m_MotionBlur.b3Clear();
	m_TimeIndex.b3Clear();
	if (animation != nullptr)
	{
		if (animation->b3IsActive() && b3IsMotionBlur())
		{
			b3_index i, max;
			b3_f64   t, factor;

			factor = 0.5 / animation->m_FramesPerSecond;
			for (i = 0; i <= m_SamplesPerFrame; i++)
			{
				t = (b3_f64)i * 2.0 / m_SamplesPerFrame - 1.0;
				m_MotionBlur.b3Add((m_FilterFrame->b3InvIntegral(t) + 1.0) * factor);
			}

			max = m_SPP * xSize;
			for (i = 0; i < max; i++)
			{
				m_TimeIndex.b3Add((b3_index)B3_IRAN(m_SamplesPerFrame));
			}
		}
	}
}

/*************************************************************************
**                                                                      **
**                        Animation root definition                     **
**                                                                      **
*************************************************************************/

b3Animation::b3Animation(b3_u32 class_type) :
	b3Special(sizeof(b3Animation), class_type)
{
	b3AllocHeads(1);
	m_Heads[0].b3InitBase(CLASS_ANIMATION);
}

b3Animation::b3Animation(b3_u32 * src) :
	b3Special(src)
{
	b3AllocHeads(1);
	m_Heads[0].b3InitBase(CLASS_ANIMATION);

	b3InitNOP();
	m_Start           = b3InitFloat();
	m_End             = b3InitFloat();
	m_Time            = b3InitFloat();
	m_Neutral         = b3InitFloat();
	m_FramesPerSecond = b3InitCount();
	m_Flags           = b3InitInt();;

	if (B3_PARSE_INDEX_VALID)
	{
		// OK, the following values are only for "Lines"
		m_Frames     = b3InitCount();
		m_Tracks     = b3InitCount();
		m_TrackIndex = b3InitIndex();
		m_FrameIndex = b3InitIndex();
		m_WTracks    = b3InitCount();
		m_WFrames    = b3InitCount();
		if (B3_PARSE_INDEX_VALID)
		{
			m_Element    = (b3AnimElement *)b3InitNull();
		}
	}
}

void b3Animation::b3Write()
{
	b3StorePtr(nullptr);
	b3StoreFloat(m_Start);
	b3StoreFloat(m_End);
	b3StoreFloat(m_Time);
	b3StoreFloat(m_Neutral);
	b3StoreCount(m_FramesPerSecond);
	b3StoreInt(m_Flags);

	// OK, the following values are only for "Lines"
	b3StoreCount(m_Frames);
	b3StoreCount(m_Tracks);
	b3StoreIndex(m_TrackIndex);
	b3StoreIndex(m_FrameIndex);
	b3StoreCount(m_WTracks);
	b3StoreCount(m_WFrames);
	b3StorePtr(m_Element);
}

bool b3Animation::b3IsActive() const
{
	return (m_Flags & ANIMF_ON) != 0;
}

void b3Animation::b3Activate(bool activate)
{
	if (activate)
	{
		m_Flags |= ANIMF_ON;
	}
	else
	{
		m_Flags ^= (~ANIMF_ON);
	}
}

/*************************************************************************
**                                                                      **
**                        b3Caustic definition                          **
**                                                                      **
*************************************************************************/

b3Caustic::b3Caustic(b3_u32 class_type) :
	b3Special(sizeof(b3Caustic), class_type)
{
}

b3Caustic::b3Caustic(b3_u32 * src) :
	b3Special(src)
{
	m_Flags      = b3InitInt();
	m_NumPhotons = b3InitInt();
	m_TraceDepth = b3InitInt();
}

void b3Caustic::b3Write()
{
	b3StoreInt(m_Flags);
	b3StoreCount(m_NumPhotons);
	b3StoreCount(m_TraceDepth);
}

/*************************************************************************
**                                                                      **
**                        b3Caustic definition                          **
**                                                                      **
*************************************************************************/

b3CloudBackground::b3CloudBackground(b3_u32 class_type) :
	b3Special(sizeof(b3Caustic), class_type)
{
}

b3CloudBackground::b3CloudBackground(b3_u32 * src) :
	b3Special(src)
{
	b3InitVector(&m_Anim);
	b3InitVector(&m_PosScale);
	m_Flags       = b3InitInt();
	m_EarthRadius = b3InitFloat();
	m_CloudHeight = b3InitFloat();
	m_Scaling     = b3InitFloat();
	m_Sharpness   = b3InitFloat();
}

void b3CloudBackground::b3Write()
{
	b3StoreVector(&m_Anim);
	b3StoreVector(&m_PosScale);
	b3StoreInt(m_Flags);
	b3StoreFloat(m_EarthRadius);
	b3StoreFloat(m_CloudHeight);
	b3StoreFloat(m_Scaling);
	b3StoreFloat(m_Sharpness);
}

bool b3CloudBackground::b3Prepare(b3_preparation_info * prep_info B3_UNUSED)
{
	b3PrepareClouds();
	return true;
}
