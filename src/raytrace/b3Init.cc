/*
**
**      $Filename:      b3Init.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Initialize raytracing
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
   
/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/
    
/*
**      $Log$
**      Revision 1.3  2001/11/01 13:22:43  sm
**      - Introducing performance meter
**
**      Revision 1.2  2001/11/01 09:43:11  sm
**      - Some image logging cleanups.
**      - Texture preparing now in b3Prepare().
**      - Done some minor fixes.
**
**      Revision 1.1.1.1  2001/07/01 12:24:59  sm
**      Blizzard III is born
**
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3TxPool texture_pool;

void b3InitRaytrace::b3Init()
{
	b3InitScene::b3Init();
	b3InitSpecial::b3Init();
	b3InitAnimation::b3Init();
	b3InitLight::b3Init();
	b3InitBBox::b3Init();
	b3InitShape::b3Init();
	b3InitMaterial::b3Init();
	b3InitBump::b3Init();
	b3InitCondition::b3Init();
}

b3_bool b3CheckTexture(b3Tx **tx,const char *name)
{
	const char *txName;
	b3_size     txLen,nameLen,diff;

	if (*tx != null)
	{
		txName  = (*tx)->b3Name();
		txLen   = strlen(txName);
		nameLen = strlen(name);
		diff    = txLen - nameLen;
		if (strcmp(&txName[diff >= 0 ? diff : 0],name) != 0)
		{
			*tx = texture_pool.b3FindTexture(name);
		}
	}

	if (*tx == null)
	{
		*tx = texture_pool.b3LoadTexture(name);
	}

	return *tx != null;
}
