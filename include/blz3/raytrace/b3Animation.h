/*
**
**	$Filename:	b3Animation.h $ 
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing class definitions for animation elements
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifndef B3_RAYTRACE_ANIMATION_H
#define B3_RAYTRACE_ANIMATION_H

#include "blz3/raytrace/b3Base.h"
#include "blz3/base/b3Spline.h"

/*************************************************************************
**                                                                      **
**                        animations                                    **
**                                                                      **
*************************************************************************/

#define CLASS_ANIMATION         0x75000000 // animation class
#define CLASS_VERTEX            0x00010000
#define CLASS_TRIANGLE	        0x00020000
#define TYPE_MOVE               0x00000001
#define TYPE_ROTATE             0x00000002
#define TYPE_SCALE              0x00000003
#define TYPE_VERTEXNODE         0x00000001
#define TYPE_TRIA               0x00000001
#define TYPE_TRIAREF            0x00000002
#define TYPE_CPOINT_3D          0x00000003
#define TYPE_CPOINT_4D          0x00000004
#define ANIM_MOVE              (CLASS_ANIMATION|TYPE_MOVE)
#define ANIM_ROTATE            (CLASS_ANIMATION|TYPE_ROTATE)
#define ANIM_SCALE             (CLASS_ANIMATION|TYPE_SCALE)
#define TRIANGLE               (CLASS_TRIANGLE|TYPE_TRIA)
#define TRIANGLEREF            (CLASS_TRIANGLE|TYPE_TRIAREF)
#define VERTEXNODE             (CLASS_VERTEX|TYPE_VERTEXNODE)
#define CPOINT_3D              (CLASS_VERTEX|TYPE_CPOINT_3D)
#define CPOINT_4D              (CLASS_VERTEX|TYPE_CPOINT_4D)

#define ANIMFLAGB_X          0
#define ANIMFLAGB_Y          1
#define ANIMFLAGB_Z          2
#define ANIMFLAGB_DIRECT     4
#define ANIMFLAGB_ACTIVE     5
#define ANIMFLAGB_RECURSIVE  6
#define ANIMFLAGB_OBJECT     8
#define ANIMFLAGB_LIGHT      9
#define ANIMFLAGB_CAMERA    10
#define ANIMFLAGB_CENTER    11
#define ANIMFLAGB_ILLUM     12

#define ANIMFLAGF_X         (1 << ANIMFLAGB_X)
#define ANIMFLAGF_Y         (1 << ANIMFLAGB_Y)
#define ANIMFLAGF_Z         (1 << ANIMFLAGB_Z)
#define ANIMFLAGF_DIRECT    (1 << ANIMFLAGB_DIRECT)
#define ANIMFLAGF_ACTIVE    (1 << ANIMFLAGB_ACTIVE)
#define ANIMFLAGF_RECURSIVE (1 << ANIMFLAGB_RECURSIVE)
#define ANIMFLAGF_OBJECT    (1 << ANIMFLAGB_OBJECT)
#define ANIMFLAGF_LIGHT     (1 << ANIMFLAGB_LIGHT)
#define ANIMFLAGF_CAMERA    (1 << ANIMFLAGB_CAMERA)
#define ANIMFLAGF_CENTER    (1 << ANIMFLAGB_CENTER)
#define ANIMFLAGF_ILLUM     (1 << ANIMFLAGB_ILLUM)

#define ANIMMASK_AXIS       0x0007
#define ANIMMASK_ACTIVE		0x0060
#define ANIMMASK_DEST       0x1f00

#define B3_ANIMSTRINGLEN       32

class B3_PLUGIN b3AnimControl : public b3Item
{
	b3_count      m_Dimension;           // vector dimension
	b3_count      m_Used;                // used number of vectors
	b3_count      m_Max;                 // maximum number of vectors
	b3_vector4D  *m_Controls;

public:
	B3_ITEM_INIT(b3AnimControl);
	B3_ITEM_LOAD(b3AnimControl);

	void b3Write();
	void b3InitNurbs(b3Nurbs &nurbs);
};

class b3Animation;
class b3BBox;
class b3Scene;

class B3_PLUGIN b3AnimElement : public b3Item
{
	b3_s32              m_Empty;                   // empty entry
public:
	b3_vector           m_Center;                  // rotation center
	b3_matrix           m_Actual;                  // actual transformation
	b3_matrix           m_NeutralInverse;          // inverse of neutral position
	b3_f64              m_Ratio;                   // parametrisation ratio
	b3_f64              m_Start;                   // start of action
	b3_f64              m_End;                     // end of action
	b3_u32              m_Flags;                   // ANIMF_xxx flags
	b3_index            m_TrackIndex;              // start track number
	b3_bool             m_CurveUse;                // number of curves used
	b3Nurbs             m_Param;                   // param. curves
	b3_f32              m_Knots[B3_MAX_KNOTS];     // knot vectors
	char                m_Name[B3_ANIMSTRINGLEN];  // element name
	char                m_Object[B3_BOXSTRINGLEN]; // name of destination object

public:
	B3_ITEM_INIT(b3AnimElement);
	B3_ITEM_LOAD(b3AnimElement);

	       void b3Write();

public:
	       char           *b3GetName();
	       void            b3GetPosition(b3_vector32_4D *position,b3_f64 t);
	       void            b3GetPosition(b3_vector      *position,b3_f64 t);
	       void            b3ComputeTransformationMatrix(b3Animation *AnimRoot,b3_matrix *transform,b3_f64 t);

	       b3_bool         b3SelectAnimElement (b3Scene *scene);
	       void            b3SelectObjects (b3BBox *BBox);
private:
	       void            b3AnimateMove  (b3Animation *AnimRoot,b3_matrix *transform,b3_f64 t);
	       void            b3AnimateRotate(b3Animation *AnimRoot,b3_matrix *transform,b3_f64 t);
	       void            b3AnimateScale (b3Animation *AnimRoot,b3_matrix *transform,b3_f64 t);
};

#endif
