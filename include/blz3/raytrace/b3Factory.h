/*
**
**	$Filename:	b3ExampleScene.h $  
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - constructing simple scenes
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

#ifndef B3_EXAMPLESCENE_H
#define B3_EXAMPLESCENE_H

#include "blz3/raytrace/b3Raytrace.h"

class b3ExampleScene
{
public:
	static inline b3Scene *b3GetNull()
	{
		return null;
	}
	static        b3Scene *b3CreateNew(const char *filename);
	static        b3Scene *b3CreateGlobal();
	static        b3Scene *b3CreateMaterial();

private:
	static        void     b3Consolidate(b3Scene *scene);
};

#endif
