/*
**
**	$Filename:	b3Condition.h $ 
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing class definitions for conditions
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifndef B3_RAYTRACE_CONDITION_H
#define B3_RAYTRACE_CONDITION_H

#include "blz3/raytrace/b3Base.h"
#include "blz3/image/b3Tx.h"

/*************************************************************************
**                                                                      **
**                        Stencil                                       **
**                                                                      **
*************************************************************************/

#define CLASS_CONDITION     0x30000000
#define MODE_OR             0x00000000
#define MODE_NOT            0x00008000
#define MODE_AND            0x00004000
#define MODE_NAND           0x0000c000
#define MODE_MASK           0x0000f000
#define MODE_FUNC           0x00000fff
#define TYPE_RECTANGLE      0x00000001
#define TYPE_CIRCLE         0x00000002
#define TYPE_CHESS          0x00000003
#define TYPE_SEGMENT        0x00000004
#define TYPE_TRIANGLE       0x00000005
#define TYPE_PARALLELOGRAM  0x00000006
#define TYPE_CTEXTURE       0x00000007
#define TYPE_CTEXTUREWRAP   0x00000008
#define TYPE_ELLIPSE        0x00000009

#define COND_RECTANGLE          (CLASS_CONDITION|TYPE_RECTANGLE)
#define COND_CIRCLE             (CLASS_CONDITION|TYPE_CIRCLE)
#define COND_SEGMENT            (CLASS_CONDITION|TYPE_SEGMENT)
#define COND_TRIANGLE           (CLASS_CONDITION|TYPE_TRIANGLE)
#define COND_PARALLELOGRAM      (CLASS_CONDITION|TYPE_PARALLELOGRAM)
#define COND_TEXTURE            (CLASS_CONDITION|TYPE_CTEXTURE)
#define COND_TEXTUREWRAP        (CLASS_CONDITION|TYPE_CTEXTUREWRAP)
#define COND_ELLIPSE            (CLASS_CONDITION|TYPE_ELLIPSE)

#define COND_NRECTANGLE         (CLASS_CONDITION|MODE_NAND|TYPE_RECTANGLE)
#define COND_NCIRCLE            (CLASS_CONDITION|MODE_NAND|TYPE_CIRCLE)
#define COND_NSEGMENT           (CLASS_CONDITION|MODE_NAND|TYPE_SEGMENT)
#define COND_NTRIANGLE          (CLASS_CONDITION|MODE_NAND|TYPE_TRIANGLE)
#define COND_NPARALLELOGRAM     (CLASS_CONDITION|MODE_NAND|TYPE_PARALLELOGRAM)
#define COND_NTEXTURE           (CLASS_CONDITION|MODE_NAND|TYPE_CTEXTURE)
#define COND_NTEXTUREWRAP       (CLASS_CONDITION|MODE_NAND|TYPE_CTEXTUREWRAP)
#define COND_NELLIPSE           (CLASS_CONDITION|MODE_NAND|TYPE_ELLIPSE)

#define COND_NOR_RECTANGLE      (CLASS_CONDITION|MODE_NOT|TYPE_RECTANGLE)
#define COND_NOR_CIRCLE         (CLASS_CONDITION|MODE_NOT|TYPE_CIRCLE)
#define COND_NOR_SEGMENT        (CLASS_CONDITION|MODE_NOT|TYPE_SEGMENT)
#define COND_NOR_TRIANGLE       (CLASS_CONDITION|MODE_NOT|TYPE_TRIANGLE)
#define COND_NOR_PARALLELOGRAM  (CLASS_CONDITION|MODE_NOT|TYPE_PARALLELOGRAM)
#define COND_NOR_TEXTURE        (CLASS_CONDITION|MODE_NOT|TYPE_CTEXTURE)
#define COND_NOR_TEXTUREWRAP    (CLASS_CONDITION|MODE_NOT|TYPE_CTEXTUREWRAP)
#define COND_NOR_ELLIPSE        (CLASS_CONDITION|MODE_NOT|TYPE_ELLIPSE)

#define COND_ARECTANGLE         (CLASS_CONDITION|MODE_AND|TYPE_RECTANGLE)
#define COND_ACIRCLE            (CLASS_CONDITION|MODE_AND|TYPE_CIRCLE)
#define COND_ASEGMENT           (CLASS_CONDITION|MODE_AND|TYPE_SEGMENT)
#define COND_ATRIANGLE          (CLASS_CONDITION|MODE_AND|TYPE_TRIANGLE)
#define COND_APARALLELOGRAM     (CLASS_CONDITION|MODE_AND|TYPE_PARALLELOGRAM)
#define COND_ATEXTURE           (CLASS_CONDITION|MODE_AND|TYPE_CTEXTURE)
#define COND_ATEXTUREWRAP       (CLASS_CONDITION|MODE_AND|TYPE_CTEXTUREWRAP)
#define COND_AELLIPSE           (CLASS_CONDITION|MODE_AND|TYPE_ELLIPSE)

enum b3_stencil_unit
{
	B3_STENCIL_UNIT,
	B3_STENCIL_LENGTH,
	B3_STENCIL_ANGLE
};

struct b3_stencil_limit
{
	b3_f64 x1,y1,x2,y2;
};

struct b3_stencil_bound_info
{
	b3_f64          min,max,factor;
	b3_stencil_unit unit;
};

struct b3_stencil_bound
{
	b3_stencil_bound_info xInfo;
	b3_stencil_bound_info yInfo;
};

class B3_PLUGIN b3Condition : public b3Item
{
	static const b3_u32 m_LogicOps[];

protected:
	B3_ITEM_BASE(b3Condition);

public:

	B3_ITEM_INIT(b3Condition);
	B3_ITEM_LOAD(b3Condition);

	static  void    b3Register();
	virtual b3_bool b3Prepare();
	virtual void    b3ComputeBound(b3_stencil_limit *limit);
	virtual b3_bool b3CheckStencil(b3_polar *polar);
	        b3_bool b3Conditionate(b3_bool input,b3_bool operation);

protected:
	static  void    b3CheckInnerBound(b3_stencil_limit *limit,b3_stencil_limit *object);
	static  void    b3CheckOuterBound(b3_stencil_limit *limit,b3_stencil_limit *object);
};

// TYPE_RECTANGLE
class B3_PLUGIN b3CondRectangle : public b3Condition
{
public:
	b3_f32  m_xStart,m_yStart;    // rel. start coordinates
	b3_f32  m_xEnd,  m_yEnd;      // rel. end koordinates
	b3_s32  m_Flags;

public:
	B3_ITEM_INIT(b3CondRectangle);
	B3_ITEM_LOAD(b3CondRectangle);

	void    b3Write();
	void    b3ComputeBound(b3_stencil_limit *limit);
	b3_bool b3CheckStencil(b3_polar *polar);
};

#define RCB_ACTIVE  0
#define RCF_ACTIVE (1 << RCB_ACTIVE)

// TYPE_TRIANGLE, TYPE_PARALLELOGRAM
class B3_PLUGIN b3Cond2 : public b3Condition
{
public:
	b3_f32  m_xPos, m_yPos;       // base of triangle/ parallelogramme
	b3_f32  m_xDir1,m_yDir1;      // direction 1
	b3_f32  m_xDir2,m_yDir2;      // direction 2
	b3_f32  m_Denom;

protected:
	B3_ITEM_BASE(b3Cond2);

public:
	B3_ITEM_INIT(b3Cond2);
	B3_ITEM_LOAD(b3Cond2);

	void    b3Write();
	b3_bool b3Prepare();
	void    b3ComputeBound(b3_stencil_limit *limit);
};

class B3_PLUGIN b3CondPara : public b3Cond2
{
public:
	B3_ITEM_INIT(b3CondPara);
	B3_ITEM_LOAD(b3CondPara);

	b3_bool b3CheckStencil(b3_polar *polar);
};

class B3_PLUGIN b3CondTria : public b3Cond2
{
public:
	B3_ITEM_INIT(b3CondTria);
	B3_ITEM_LOAD(b3CondTria);

	b3_bool b3CheckStencil(b3_polar *polar);
};


// TYPE_CIRCLE
class B3_PLUGIN b3CondCircle : public b3Condition
{
protected:
	b3_f32  m_xCenter,m_yCenter,m_Radius;      // Mittelpunkt und Radius

public:
	B3_ITEM_INIT(b3CondCircle);
	B3_ITEM_LOAD(b3CondCircle);

	void    b3Write();
	void    b3ComputeBound(b3_stencil_limit *limit);
	b3_bool b3CheckStencil(b3_polar *polar);
};

// TYPE_SEGMENT
class B3_PLUGIN b3CondSegment : public b3Condition
{
protected:
	b3_f32  m_xCenter,   m_yCenter;  // Mittelpunkt
	b3_f32  m_RadStart,  m_RadEnd;   // gültiger Bereich im Ring
	b3_f32  m_AngleStart,m_AngleEnd; // Segment

public:
	B3_ITEM_INIT(b3CondSegment);
	B3_ITEM_LOAD(b3CondSegment);

	void    b3Write();
	void    b3ComputeBound(b3_stencil_limit *limit);
	b3_bool b3CheckStencil(b3_polar *polar);
};

// TYPE_ELLIPSE
class B3_PLUGIN b3CondEllipse : public b3Condition
{
protected:
	b3_f32  m_xCenter,   m_yCenter;  // Mittelpunkt
	b3_f32  m_xRadius,   m_yRadius;  // Radien
	b3_f32  m_RadStart,  m_RadEnd;   // gültiger Bereich im Ring
	b3_f32  m_AngleStart,m_AngleEnd; // Segment

public:
	B3_ITEM_INIT(b3CondEllipse);
	B3_ITEM_LOAD(b3CondEllipse);

	void    b3Write();
	void    b3ComputeBound(b3_stencil_limit *limit);
	b3_bool b3CheckStencil(b3_polar *polar);
};

// TYPE_TEXTURE
class B3_PLUGIN b3CondTexture : public b3Condition
{
protected:
	b3Tx   *m_Texture;
	b3_s32  m_Flags;               // unused yet
	b3_f32  m_xStart,m_yStart;     // base of texture
	b3_f32  m_xScale,m_yScale;     // scale of texture
	b3_s32  m_xTimes,m_yTimes;     // repetion of texture in x- and y-direction
	b3Path  m_Name;  // title

public:
	B3_ITEM_INIT(b3CondTexture);
	B3_ITEM_LOAD(b3CondTexture);

	void    b3Write();
	b3_bool b3Prepare();
	void    b3ComputeBound(b3_stencil_limit *limit);
	b3_bool b3CheckStencil(b3_polar *polar);
};

// TYPE_WRAP_TEXTURE
class B3_PLUGIN b3CondWrapTexture : public b3Condition
{
protected:
	b3Tx   *m_Texture;
	b3_s32  m_Flags;               // unused yet
	b3_f32  m_xStart,m_yStart;     // borders in hor. direction
	b3_f32  m_xEnd,m_yEnd;         // borders in vert. direction
	b3Path  m_Name;                // title

public:
	B3_ITEM_INIT(b3CondWrapTexture);
	B3_ITEM_LOAD(b3CondWrapTexture);

	void    b3Write();
	b3_bool b3Prepare();
	void    b3ComputeBound(b3_stencil_limit *limit);
	b3_bool b3CheckStencil(b3_polar *polar);
};


#endif
