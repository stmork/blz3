/*
**
**	$Filename:	b3Init.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Initialize raytracing
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/raytrace/b3Bump.h"
#include "blz3/raytrace/b3Condition.h"
#include "blz3/raytrace/b3BBox.h"
#include "blz3/raytrace/b3Material.h"
#include "blz3/raytrace/b3Scene.h"

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

void b3RaytracingItems::b3Register()
{
	b3Scene::b3Register();
	b3Special::b3Register();
	b3Animation::b3Register();
	b3Light::b3Register();
	b3BBox::b3Register();
	b3Shape::b3Register();
	b3Material::b3Register();
	b3Bump::b3Register();
	b3Condition::b3Register();
}

bool b3Scene::b3CheckTexture(b3Tx ** tx, const char * name)
{
	const char * txName;
	b3_size      txLen, nameLen;
	b3_offset    diff;

	if (*tx != nullptr)
	{
		txName  = (*tx)->b3Name();
		txLen   = strlen(txName);
		nameLen = strlen(name);
		diff    = (b3_offset)txLen - (b3_offset)nameLen;
		if (strcmp(&txName[diff >= 0 ? diff : 0], name) != 0)
		{
			*tx = m_TexturePool.b3FindTexture(name);
		}
	}

	if (*tx == nullptr)
	{
		*tx = m_TexturePool.b3LoadTexture(name);
	}

	return *tx != nullptr;
}

bool b3Scene::b3CutTextureName(const char * full_name, char * short_name)
{
	return m_TexturePool.b3CutName(full_name, short_name);
}
