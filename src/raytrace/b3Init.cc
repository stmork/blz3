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
**      Revision 1.1  2001/07/01 12:24:59  sm
**      Initial revision
**
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

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
