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
  
#include "blz3/raytrace/b3Condition.h"
#include "blz3/raytrace/b3Bump.h"
#include "blz3/raytrace/b3Material.h"
#include "blz3/raytrace/b3Shape.h"   
#include "blz3/raytrace/b3BBox.h"    
#include "blz3/raytrace/b3Light.h"   
#include "blz3/raytrace/b3Animation.h"
#include "blz3/raytrace/b3Special.h"  
#include "blz3/raytrace/b3Scene.h"    
   
/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/
    
/*
**      $Log$
**      Revision 1.11  2005/01/01 16:43:19  sm
**      - Fixed some aliasing warnings.
**
**      Revision 1.10  2004/04/17 09:40:55  sm
**      - Splitting b3Raytrace.h into their components for
**        better oversightment.
**
**      Revision 1.9  2003/07/20 09:38:30  sm
**      - Registering raytracing items manually.
**
**      Revision 1.8  2003/07/12 17:44:47  sm
**      - Cleaned up raytracing b3Item registration
**
**      Revision 1.7  2003/02/22 17:21:34  sm
**      - Changed some global variables into static class members:
**        o b3Scene::epsilon
**        o b3Scene::m_TexturePool et. al.
**        o b3SplineTemplate<class VECTOR>::bspline_errno
**
**      Revision 1.6  2001/11/12 16:50:29  sm
**      - Scene properties dialog coding
**
**      Revision 1.5  2001/11/11 11:51:21  sm
**      - Added image select feature
**      - Cleaned up scene dialog (Now ready to improve it)
**      - some b3Path fixes
**
**      Revision 1.4  2001/11/02 19:05:36  sm
**      - Introducing time mearuring.
**      - Fixed wrong lens flare handling.
**
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

b3_bool b3Scene::b3CheckTexture(b3Tx **tx,const char *name)
{
	const char *txName;
	b3_size     txLen,nameLen;
	b3_diff     diff;

	if (*tx != null)
	{
		txName  = (*tx)->b3Name();
		txLen   = strlen(txName);
		nameLen = strlen(name);
		diff    = txLen - nameLen;
		if (strcmp(&txName[diff >= 0 ? diff : 0],name) != 0)
		{
			*tx = m_TexturePool.b3FindTexture(name);
		}
	}

	if (*tx == null)
	{
		*tx = m_TexturePool.b3LoadTexture(name);
	}

	return *tx != null;
}

b3_bool b3Scene::b3CutTextureName(const char *full_name,char *short_name)
{
	return m_TexturePool.b3CutName(full_name,short_name);
}
