/*
**
**	$Filename:	b3BaseTrans.h $
**	$Release:	Dortmund 2005 $
**	$Revision$
**	$Date$
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

class B3_PLUGIN b3BaseTransformation
{
protected:
	b3_vector64       m_Normals[3];       // cross products
	b3_f64            m_Denom;            // denominator of lin. system
	b3_f64            m_DirLen[3];        // length of direction vectors

public:
	b3_vector         m_Base;
	b3_vector         m_Dir1,m_Dir2,m_Dir3;

	b3_bool b3Prepare();
	void    b3BaseTransform(b3_line64 *in,b3_line64 *out);
	void    b3BaseTransform(b3_vector *in,b3_vector *out);
	void    b3Project(b3_vector *in,b3_vector *out);
};

#endif
