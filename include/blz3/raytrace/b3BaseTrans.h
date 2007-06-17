/*
**
**	$Filename:	b3BaseTrans.h $
**	$Release:	Dortmund 2005 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Performing base transformation
**
**	(C) Copyright 2005  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifndef B3_RAYTRACE_BASETRANS_H
#define B3_RAYTRACE_BASETRANS_H

#include "blz3/raytrace/b3Base.h"

/*************************************************************************
**                                                                      **
**                        Base class for all 3D shapes with base        **
**                        transformation.                               **
**                                                                      **
*************************************************************************/

/**
 * This class handles base transformation. The base transformation
 * transforms one coordinate system into another.
 */
class B3_PLUGIN b3BaseTransformation
{
protected:
	b3_vector64       m_Normals[3];          //!< The cross products.
	b3_f64            m_Denom;               //!< Denominator of lin. system.
	b3_f64            m_DirLen[3];           //!< Length of direction vectors.

public:
	b3_vector         m_Base;                //!< Base position.
	b3_vector         m_Dir1,m_Dir2,m_Dir3;  //!< Direction vectors.

	/**
	 * This method precomputes some values which would not change on
	 * base transform.
	 *
	 * @return True if linear system is not singular.
	 * @see b3BaseTransform()
	 */
	b3_bool b3Prepare();

	/**
	 * This method performs a base transformation on a ray.
	 *
	 * @param in The input ray.
	 * @param out The transformed ray.
	 */
	void    b3BaseTransform(const b3_line64 *in,b3_line64 *out);

	/**
	 * This method performs a base transformation on a direction vector.
	 *
	 * @param in The input ray.
	 * @param out The transformed ray.
	 */
	void    b3BaseTransform(const b3_vector *in,b3_vector *out);

	/**
	 * This method projects a vector. This is a base transformation
	 * in addition with a homogenous normalization.
	 *
	 * @param in The input ray.
	 * @param out The transformed ray.
	 */
	void    b3Project(const b3_vector *in,b3_vector *out);
};

#endif
