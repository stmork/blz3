/*
**
**	$Filename:	b3PickInfoObject.h $ 
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

#ifndef B3_PICKINFOOBJECT_H
#define B3_PICKINFOOBJECT_H

#include "blz3/raytrace/b3PickInfo.h"
#include "blz3/raytrace/b3RenderView.h"

class b3PickInfoObject : public b3PickInfo
{
	b3RenderView  *m_RenderView;

public:
	     b3PickInfoObject(b3RenderView *render_view);
	void b3AddPickPoint(b3_vector *point,const char *title = null);
	void b3AddPickDir(b3_vector *point,b3_vector *dir,const char *title = null);
};

#endif
