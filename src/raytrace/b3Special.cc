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

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
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
}

b3Special::b3Special(b3_size class_size,b3_u32 class_type) : b3Item(class_size,class_type)
{
}

b3Special::b3Special(b3_u32 class_type) : b3Item(sizeof(b3Special),class_type)
{
}

b3Special::b3Special(b3_u32 *src) : b3Item(src)
{
}


b3SuperSample::b3SuperSample(b3_u32 class_type) :
	b3Special(sizeof(b3SuperSample),class_type)
{
}

b3SuperSample::b3SuperSample(b3_u32 *src) :
	b3Special(src)
{
	b3InitColor(&Limit);
}


b3CameraPart::b3CameraPart(b3_u32 class_type) :
	b3Special(sizeof(b3CameraPart),class_type)
{
}

b3CameraPart::b3CameraPart(b3_u32 *src) :
	b3Special(src)
{
	b3InitVector(&Width);
	b3InitVector(&Height);
	b3InitVector(&EyePoint);
	b3InitVector(&ViewPoint);
	Flags = b3InitInt();
	b3InitString(CameraName,B3_CAMERANAMELEN);
}


b3ModellerInfo::b3ModellerInfo(b3_u32 class_type) :
	b3Special(sizeof(b3ModellerInfo),class_type)
{
	m_Center.x = 0;
	m_Center.y = 0;
	m_Center.z = 0;
}

b3ModellerInfo::b3ModellerInfo(b3_u32 *src) :
	b3Special(src)
{
	b3InitVector(&m_Center);
	m_GridMove     = b3InitFloat();
	m_GridRot      = b3InitFloat();
	m_ResizeFlag   = b3InitBool();
	m_BBoxTitles   = b3InitBool();
	m_GridActive   = b3InitBool();
	m_CameraActive = b3InitBool();
	if (B3_PARSE_INDEX_VALID)
	{
		m_Flags    = b3InitInt();
		m_Unit     = b3InitFloat();	
	}
}

b3_vector *b3ModellerInfo::b3GetFulcrum()
{
	return &m_Center;
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
	if (m_GridActive)
	{
		// Convert to radians
		b3_f64 GridStep = m_GridRot * M_PI / 180.0;

		angle = floor(angle / GridStep + 0.5) * GridStep;
	}
}

b3Nebular::b3Nebular(b3_u32 class_type) :
	b3Special(sizeof(b3Nebular),class_type)
{
}

b3Nebular::b3Nebular(b3_u32 *src) :
	b3Special(src)
{
	b3InitColor(&NebularColor);
	NebularVal = b3InitFloat();
}


b3Animation::b3Animation(b3_u32 class_type) :
	b3Special(sizeof(b3Animation),class_type)
{
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

	// OK, the following values are only for "Lines"
	frames     = b3InitInt();;
	tracks     = b3InitInt();;
	trackIndex = b3InitInt();;
	frameIndex = b3InitInt();;
	WTracks    = b3InitInt();;
	WFrames    = b3InitInt();;
	Element    = (b3AnimElement *)b3InitNull();
}


b3Distribute::b3Distribute(b3_u32 class_type) :
	b3Special(sizeof(b3Distribute),class_type)
{
}

b3Distribute::b3Distribute(b3_u32 *src) :
	b3Special(src)
{
	Type            = b3InitInt();
	SamplesPerPixel = b3InitInt();
	SamplesPerFrame = b3InitInt();
	if (B3_PARSE_INDEX_VALID)
	{
		DepthOfField    = b3InitFloat();
		PixelAperture   = b3InitInt();
		FrameAperture   = b3InitInt();
	}
}


b3LensFlare::b3LensFlare(b3_u32 class_type) :
	b3Special(sizeof(b3LensFlare),class_type)
{
}

b3LensFlare::b3LensFlare(b3_u32 *src) :
	b3Special(src)
{
	Flags = b3InitInt();
	b3InitColor(&Color);
	Expon = b3InitFloat();
}
