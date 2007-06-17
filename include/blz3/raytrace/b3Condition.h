/*
**
**	$Filename:	b3Condition.h $ 
**	$Release:	Dortmund 2004 $
**	$Revision: 1449 $
**	$Date: 2006-06-28 19:17:02 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
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

/**
 * This enumeration defines the purpose of a polar coordinate.
 */
enum b3_stencil_unit
{
	B3_STENCIL_UNIT,    //!< The value represents a simple unit.
	B3_STENCIL_LENGTH,  //!< The value represents a length.
	B3_STENCIL_ANGLE    //!< The value represents an angle.
};

/**
 * This structure represents the bounding box of one shape formed by the
 * conditional stencil classes.
 */
struct b3_stencil_limit
{
	b3_f64 x1,y1,x2,y2;
};

/**
 * This structure contains information about maximal bounds in one direction.
 */
struct b3_stencil_bound_info
{
	b3_f64          min;    //!< The minimum lower bound.
	b3_f64          max;    //!< The maximum upper bound.
	b3_f64          factor; //!< The scaling factor for human readable values.
	b3_stencil_unit unit;
};

/**
 * This structure represents info about maximum surface bounds.
 */
struct b3_stencil_bound
{
	b3_stencil_bound_info xInfo; //!< The horizontal bound.
	b3_stencil_bound_info yInfo; //!< The vertical bound.
};

/**
 * This base class represents the behaviour for shape surface stencelling.
 */
class B3_PLUGIN b3Condition : public b3Item
{
	static const b3_u32 m_LogicOps[];

protected:
	B3_ITEM_BASE(b3Condition); //!< This is a base class deserialization constructor.

public:
	B3_ITEM_INIT(b3Condition); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3Condition); //!< This constructor handles deserialization.

	/**
	 * Method for registering the shapes into the item registry.
	 */
	static  void    b3Register();
	virtual b3_bool b3Prepare(b3_preparation_info *prep_info);

	/**
	 * This method compute the bounding box of this stencil condition
	 * depending on the condition values.
	 *
	 * @param limit The info structure to fill.
	 */
	virtual void    b3ComputeBound(b3_stencil_limit *limit);

	/**
	 * This method checks wether the given polar coordinates matches into the
	 * condition of this stencil.
	 *
	 * @param polar The polar coordinates to check.
	 * @return True if the polar coordinates matches the stencil condition.
	 */
	virtual b3_bool b3CheckStencil(b3_polar *polar);

	/**
	 * This Method computes a Boolean expression depending of the
	 * Boolean operand type specified with this stencil condition.
	 *
	 * @param input     All previous Boolean operations.
	 * @param operation The actual result from this condition.
	 * @return The result of the Boolean term specified with this condition.
	 */
	b3_bool b3Conditionate(b3_bool input,b3_bool operation);

protected:
	/**
	 * This method checks a given limit against the object limit. The object
	 * bound values are corrected so that the first values are smaller than
	 * the second values.
	 *
	 * @param limit The bound to adjust.
	 * @param object The object bound.
	 */
	static  void    b3CheckInnerBound(b3_stencil_limit *limit,b3_stencil_limit *object);

	/**
	 * This method checks a given limit against the object limit. The object
	 * bound values are corrected so that the first values are smaller than
	 * the second values.
	 *
	 * @param limit The bound to adjust.
	 * @param object The object bound.
	 */
	static  void    b3CheckOuterBound(b3_stencil_limit *limit,b3_stencil_limit *object);
};

/**
 * This class represents a simple rectangular area for stencilling.
 */
class B3_PLUGIN b3CondRectangle : public b3Condition
{
public:
	b3_f32  m_xStart,m_yStart;    //!< Polar start coordinates.
	b3_f32  m_xEnd,  m_yEnd;      //!< Polar end coordinates.
	b3_s32  m_Flags;              //!< Some flags.

public:
	B3_ITEM_INIT(b3CondRectangle); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3CondRectangle); //!< This constructor handles deserialization.

	void    b3Write();
	void    b3ComputeBound(b3_stencil_limit *limit);
	b3_bool b3CheckStencil(b3_polar *polar);
};

#define RCB_ACTIVE  0
#define RCF_ACTIVE (1 << RCB_ACTIVE)

/**
 * This class represents 2d shapes for stencelling such as a triangle or
 * a parallelogramme.
 */
class B3_PLUGIN b3Cond2 : public b3Condition
{
protected:
	b3_f32  m_Denom;              //!< The determinand of the direction vectors.

public:
	b3_f32  m_xPos, m_yPos;       //!< Base of triangle/ parallelogramme.
	b3_f32  m_xDir1,m_yDir1;      //!< First direction vector.
	b3_f32  m_xDir2,m_yDir2;      //!< Second direction vector.

protected:
	B3_ITEM_BASE(b3Cond2); //!< This is a base class deserialization constructor.

public:
	B3_ITEM_INIT(b3Cond2); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3Cond2); //!< This constructor handles deserialization.

	void    b3Write();
	b3_bool b3Prepare(b3_preparation_info *prep_info);
	void    b3ComputeBound(b3_stencil_limit *limit);
};

/**
 * This class represents a parallelogramme stencil.
 */
class B3_PLUGIN b3CondPara : public b3Cond2
{
public:
	B3_ITEM_INIT(b3CondPara); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3CondPara); //!< This constructor handles deserialization.

	b3_bool b3CheckStencil(b3_polar *polar);
};

/**
 * This class represents a triangle stencil.
 */
class B3_PLUGIN b3CondTria : public b3Cond2
{
public:
	B3_ITEM_INIT(b3CondTria); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3CondTria); //!< This constructor handles deserialization.

	b3_bool b3CheckStencil(b3_polar *polar);
};

/**
 * This class represents a circle condition.
 */
class B3_PLUGIN b3CondCircle : public b3Condition
{
protected:
	b3_f32  m_xCenter;     //!< Polar x coordinate of center.
	b3_f32  m_yCenter;     //!< Polar y coordinate of center.
	b3_f32  m_Radius;      //!< Radius

public:
	B3_ITEM_INIT(b3CondCircle); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3CondCircle); //!< This constructor handles deserialization.

	void    b3Write();
	void    b3ComputeBound(b3_stencil_limit *limit);
	b3_bool b3CheckStencil(b3_polar *polar);
};

/**
 * This class represents a circular segment bounded by radiuses and angles.
 */
class B3_PLUGIN b3CondSegment : public b3Condition
{
protected:
	b3_f32  m_xCenter,   m_yCenter;  //!< Center.
	b3_f32  m_RadStart,  m_RadEnd;   //!< Radius bounds.
	b3_f32  m_AngleStart,m_AngleEnd; //!< Angular bounds.

public:
	B3_ITEM_INIT(b3CondSegment); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3CondSegment); //!< This constructor handles deserialization.

	void    b3Write();
	void    b3ComputeBound(b3_stencil_limit *limit);
	b3_bool b3CheckStencil(b3_polar *polar);
};

/**
 * This method represents an ellipse segment bounded by radiuses and angles.
 */
class B3_PLUGIN b3CondEllipse : public b3Condition
{
protected:
	b3_f32  m_xCenter,   m_yCenter;  //!< Center.
	b3_f32  m_xRadius,   m_yRadius;  //!< The horizontal and vertical radius.
	b3_f32  m_RadStart,  m_RadEnd;   //!< Radius bounds.
	b3_f32  m_AngleStart,m_AngleEnd; //!< Anglular bounds.

public:
	B3_ITEM_INIT(b3CondEllipse); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3CondEllipse); //!< This constructor handles deserialization.

	void    b3Write();
	void    b3ComputeBound(b3_stencil_limit *limit);
	b3_bool b3CheckStencil(b3_polar *polar);
};

/**
 * This class implements simple stencil mapping. The 2D polar surface coordinates
 * are transformed into texture coordinate domain using start point (translation)
 * and scaling. The texture can be tiled.
 */
class B3_PLUGIN b3CondTexture : public b3Condition
{
protected:
	b3_f32            m_xStart,m_yStart;    //!< Surface coordinate start.
	b3_f32            m_xScale,m_yScale;    //!< Texture scale.
	b3_s32            m_xTimes,m_yTimes;    //!< Repeatition in x- y-direction.
	b3_s32            m_Flags;              //!< Unused.
	b3Path            m_Name;               //!< The texture file name.
	b3Tx             *m_Texture;            //!< The selected texture.

public:
	B3_ITEM_INIT(b3CondTexture); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3CondTexture); //!< This constructor handles deserialization.

	void    b3Write();
	b3_bool b3Prepare(b3_preparation_info *prep_info);
	void    b3ComputeBound(b3_stencil_limit *limit);
	b3_bool b3CheckStencil(b3_polar *polar);
};

/**
 * This class implements a wrapping stencil texture. This fits into the specified 2D
 * polar surface coordinates.
 */
class B3_PLUGIN b3CondWrapTexture : public b3Condition
{
protected:
	b3_f32            m_xStart,m_yStart;    //!< Surface coordinate start
	b3_f32            m_xEnd,m_yEnd;        //!< Surface coordinate end
	b3_s32            m_Flags;              //!< Unused.
	b3Path            m_Name;               //!< The texture file name.
	b3Tx             *m_Texture;            //!< The selected texture.

public:
	B3_ITEM_INIT(b3CondWrapTexture); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3CondWrapTexture); //!< This constructor handles deserialization.

	void    b3Write();
	b3_bool b3Prepare(b3_preparation_info *prep_info);
	void    b3ComputeBound(b3_stencil_limit *limit);
	b3_bool b3CheckStencil(b3_polar *polar);
};

#endif
