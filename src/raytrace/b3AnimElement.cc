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
**      Revision 1.2  2002/08/18 13:05:17  sm
**      - First try to animate. We have to relink the control points which
**        are stored in separate Blizzard classes to the b3AnimElement
**        class.
**
**      Revision 1.1  2002/08/17 17:31:22  sm
**      - Introduced animation support (Puh!)
**
**
*/

/*************************************************************************
**                                                                      **
**                        b3AnimElement implementation                  **
**                                                                      **
*************************************************************************/

b3AnimElement::b3AnimElement(b3_u32 class_type) : b3Item(sizeof (b3AnimElement),class_type)
{
	b3AllocHeads(1);
	m_Heads[0].b3InitBase(CLASS_VERTEX);
}

b3AnimElement::b3AnimElement(b3_u32 *src) : b3Item(src)
{
	b3AllocHeads(1);
	m_Heads[0].b3InitBase(CLASS_VERTEX);

	b3InitNOP();
	if (m_ItemSize > 0x1ec)
	{
		b3InitVector(&m_Center);
		b3InitMatrix(&m_Actual);
		b3InitMatrix(&m_NeutralInverse);
	}
	else
	{
		b3Vector::b3Init(&m_Center);
		b3InitMatrix(&m_Actual);
		b3MatrixUnit(&m_NeutralInverse);
	}
	m_Ratio      = b3InitFloat();
	m_Empty      = b3InitInt();
	m_Start      = b3InitFloat();
	m_End        = b3InitFloat();
	m_Flags      = b3InitInt();
	m_TrackIndex = b3InitInt();
	m_CurveUse   = (m_ItemSize > 0x1ec ? b3InitInt() : 0);

	// Init nurbs
	b3InitNurbs(&m_Param,null,m_Knots);
	for (b3_index i = 0;i < m_Param.knot_max;i++)
	{
		m_Knots[i] = b3InitFloat();
	}

	b3InitString(m_Name,  B3_ANIMSTRINGLEN);
	b3InitString(m_Object,B3_BOXSTRINGLEN);
}

void b3AnimElement::b3Write()
{
	b3StorePtr   (null);
	b3StoreVector(&m_Center);
	b3StoreMatrix(&m_Actual);
	b3StoreMatrix(&m_NeutralInverse);
	b3StoreFloat(m_Ratio);
	b3StoreInt  (m_Empty);
	b3StoreFloat(m_Start);
	b3StoreFloat(m_End);
	b3StoreInt  (m_Flags);
	b3StoreIndex(m_TrackIndex);
	b3StoreBool (m_CurveUse);

	// Init nurbs
	b3StoreNurbs(&m_Param);
	for (b3_index i = 0;i < m_Param.knot_max;i++)
	{
		b3StoreFloat(m_Knots[i]);
	}

	b3StoreString(m_Name,  B3_ANIMSTRINGLEN);
	b3StoreString(m_Object,B3_BOXSTRINGLEN);
}

/* This routine computes the point on a curve at a specified time point. */
/* The time point must be valid to compute the curve point at the time */
/* point. So use ClipValue() to clip at the time bounds. */
/* --------------------------------------------------------------------- */
/* Element:  where the NURBS curve is specified */
/* position: where to store the curve point */
/* t:        the time point of the curve point */

void b3AnimElement::b3GetPosition(b3_vector32_4D *position,b3_f64 t)
{
	b3_f64      q,qStart,ratio;
	b3_index    pos;
	b3_f64      coeffs[B3_MAX_DEGREE+1];

		/* compute predefined values */
	qStart = m_Knots[m_Param.closed ? 0 : m_Param.degree];
	q      = m_Knots[m_Param.control_num] - qStart;

		/* compute position at time t */
	ratio = (t - m_Start) / (m_End - m_Start);
	pos = m_Param.b3Mansfield (coeffs,qStart + q * ratio);
	m_Param.b3MansfieldVector(position,coeffs,pos,0L);
}

void b3AnimElement::b3GetPosition(b3_vector *position,b3_f64 t)
{
	b3_vector32_4D dummy;

	b3GetPosition(&dummy,t);
	position->x = dummy.x;
	position->y = dummy.y;
	position->z = dummy.z;
}
