/*
**
**      $Filename:      b3Animation.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing Animation
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
#include "blz3/base/b3Matrix.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.10  2002/08/15 13:56:43  sm
**      - Introduced B3_THROW macro which supplies filename
**        and line number of source code.
**      - Fixed b3AllocTx when allocating a zero sized image.
**        This case is definitely an error!
**      - Added row refresh count into Lines
**
**      Revision 1.9  2002/08/09 13:20:19  sm
**      - b3Mem::b3Realloc was a mess! Now fixed to have the same
**        behaviour on all platforms. The Windows method ::GlobalReAlloc
**        seems to be broken:-(
**      - Introduced b3DirAbstract and b3PathAbstract classes
**
**      Revision 1.8  2002/03/03 21:22:22  sm
**      - Added support for creating surfaces using profile curves.
**      - Added simple creating of triangle fields.
**
**      Revision 1.7  2002/01/01 13:50:22  sm
**      - Fixed some memory leaks:
**        o concerning triangle shape and derived spline shapes
**        o concerning image pool handling. Images with windows
**          path weren't found inside the image pool requesting
**          further image load.
**
**      Revision 1.6  2001/12/31 12:15:55  sm
**      - Fixed obsolete b3AnimElement handling
**
**      Revision 1.5  2001/12/31 11:05:17  sm
**      - Added TestData for testing Blizzard data structures for reading
**        and writing.
**      - Fixed bugs found with previous mentioned tool:
**        o Some b3AnimElement errors found to be fixed under Windows.
**        o b3TriangleShape destructor handled unchecked m_GridList pointer
**      - Changed some output levels in b3Light, b3Scene and b3ShadeXXX from
**        B3LOG_NORMAL to B3LOG_DEBUG.
**
**      Revision 1.4  2001/12/30 22:52:35  sm
**      - Made b3Scene::b3SetCamera() compatible to earlier versions.
**
**      Revision 1.3  2001/12/30 18:24:35  sm
**      - Added missing b3AnimControl class
**      - Some minor bug fixes done:
**        o Missed some SetModifiedFlag()
**        o b3Scene::b3SetCamera() calls added which now puts the
**          selected camera in front of the b3Special list so that Lines III
**          select it when reloading.
**
**      Revision 1.2  2001/12/30 14:16:57  sm
**      - Abstracted b3File to b3FileAbstract to implement b3FileMem (not done yet).
**      - b3Item writing implemented and updated all raytracing classes
**        to work properly.
**      - Cleaned up spline shapes and CSG shapes.
**      - Added b3Caustic class for compatibility reasons.
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

void b3InitAnimation::b3Init()
{
	b3PrintF (B3LOG_DEBUG,"Registering animation elements...\n");
	b3Item::b3Register(&b3AnimElement::b3StaticInit,&b3AnimElement::b3StaticInit,ANIM_MOVE );
	b3Item::b3Register(&b3AnimElement::b3StaticInit,&b3AnimElement::b3StaticInit,ANIM_ROTATE );
	b3Item::b3Register(&b3AnimElement::b3StaticInit,&b3AnimElement::b3StaticInit,ANIM_SCALE );
	b3Item::b3Register(&b3AnimControl::b3StaticInit,&b3AnimControl::b3StaticInit,CPOINT_3D );
	b3Item::b3Register(&b3AnimControl::b3StaticInit,&b3AnimControl::b3StaticInit,CPOINT_4D );
}

/*************************************************************************
**                                                                      **
**                        b3AnimControl implementation                  **
**                                                                      **
*************************************************************************/

b3AnimControl::b3AnimControl(b3_u32 class_type) : b3Item(sizeof (b3AnimControl),CPOINT_4D)
{
	m_Dimension = 4;
	m_Used      = 0;
	m_Max       = 0;
	m_Controls  = null;
}

b3AnimControl::b3AnimControl(b3_u32 *src) : b3Item(src)
{
	b3_index i;

	m_Dimension = b3InitInt();
	m_Used      = b3InitInt();
	m_Max       = b3InitInt();

	m_Controls  = (b3_vector4D *)b3Alloc(m_Max * sizeof(b3_vector4D));
	if (m_Controls == null)
	{
		B3_THROW(b3WorldException,B3_WORLD_MEMORY);
	}
	for (i = 0;i < m_Max;i++)
	{
		m_Controls[i].x = b3InitFloat();
		m_Controls[i].y = b3InitFloat();
		m_Controls[i].z = b3InitFloat();
		m_Controls[i].w = (m_Dimension == 4 ? b3InitFloat() : 1.0f);
	}

	// Force homogenous coordinates
	ClassType   = CPOINT_4D;
	m_Dimension = 4;
}

void b3AnimControl::b3Write()
{
	b3_index i;

	b3StoreInt  (m_Dimension);
	b3StoreCount(m_Used);
	b3StoreCount(m_Max);

	for (i = 0;i < m_Max;i++)
	{
		b3StoreVector4D(&m_Controls[i]);
	}
}

/*************************************************************************
**                                                                      **
**                        b3AnimElement implementation                  **
**                                                                      **
*************************************************************************/

b3AnimElement::b3AnimElement(b3_u32 class_type) : b3Item(sizeof (b3AnimElement),class_type)
{
	m_Heads[0].b3InitBase(CLASS_VERTEX);
}

b3AnimElement::b3AnimElement(b3_u32 *src) : b3Item(src)
{
	b3InitNOP();
	if (m_ItemSize > 0x1ec)
	{
		b3InitVector(&center);
		b3InitMatrix(&actual);
		b3InitMatrix(&neutralInverse);
	}
	else
	{
		b3Vector::b3Init(&center);
		b3InitMatrix(&actual);
		b3MatrixUnit(&neutralInverse);
	}
	ratio      = b3InitFloat();
	empty      = b3InitInt();
	start      = b3InitFloat();
	end        = b3InitFloat();
	flags      = b3InitInt();
	trackIndex = b3InitInt();
	curveUse   = (m_ItemSize > 0x1ec ? b3InitInt() : 0);

	// Init nurbs
	b3InitNurbs(&param,null,knots);
	for (b3_index i = 0;i < B3_MAX_KNOTS;i++)
	{
		knots[i] = b3InitFloat();
	}

	b3InitString(name,  B3_ANIMSTRINGLEN);
	b3InitString(object,B3_BOXSTRINGLEN);
}

void b3AnimElement::b3Write()
{
	b3StorePtr   (null);
	b3StoreVector(&center);
	b3StoreMatrix(&actual);
	b3StoreMatrix(&neutralInverse);
	b3StoreFloat(ratio);
	b3StoreInt  (empty);
	b3StoreFloat(start);
	b3StoreFloat(end);
	b3StoreInt(flags);
	b3StoreInt(trackIndex);
	b3StoreInt(curveUse);

	// Init nurbs
	b3StoreNurbs(&param);
	for (b3_index i = 0;i < B3_MAX_KNOTS;i++)
	{
		b3StoreFloat(knots[i]);
	}

	b3StoreString(name,  B3_ANIMSTRINGLEN);
	b3StoreString(object,B3_BOXSTRINGLEN);
}
