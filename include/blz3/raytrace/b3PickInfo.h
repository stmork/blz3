/*
**
**	$Filename:	b3PickInfo.h $ 
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Setup pick points depending on shapes
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifndef B3_RAYTRACE_PICKINFO_H
#define B3_RAYTRACE_PICKINFO_H

#include "blz3/base/b3Pick.h"

class b3Shape;

class b3PickInfo : public b3PickBase
{
protected:
	b3Shape *m_Shape;

public:
	                 b3PickInfo();
	virtual         ~b3PickInfo();
	        b3_bool  b3SetShape(b3Shape *shape);
	virtual void     b3AddPickPoint(b3_vector *point,const char *title = null) = 0;
	virtual void     b3AddPickDir(b3_vector *point,b3_vector *dir,const char *title = null) = 0;
};

#endif
