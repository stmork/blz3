/*
**
**	$Filename:	b3Factory.h $
**	$Release:	Dortmund 2005 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - constructing simple scenes
**
**	(C) Copyright 2005  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

#ifndef B3_RAYTRACE_FACTORY_H
#define B3_RAYTRACE_FACTORY_H

#include "blz3/raytrace/b3BBox.h"
#include "blz3/raytrace/b3Special.h"
#include "blz3/raytrace/b3Scene.h"

/**
 * This static class creates scenes for diverse purposes.
 */
class b3Factory
{
public:
	/**
	 * This method creates a new empty scene for modeling.
	 *
	 * @param filename The new filename.
	 * @param classType The class type of the new scene.
	 * @return The new empty scene.
	 */
	static b3Scene      *b3CreateNew(const char *filename, b3_u32 classType = TRACEPHOTO_MORK);

	/**
	 * This method creates an example scene containing a checker board and
	 * a mirrored and a glass sphere. The scene can be used for previewing
	 * global settings such as special items.
	 *
	 * @param classType The shading type of the new scene.
	 * @return The new preview scene.
	 */
	static b3Scene      *b3CreateGlobal(b3_u32 classType);

	/**
	 * This method creates an example scene which contains only the given object for
	 * previewing. If the original camera is given the perspective is cloned.
	 *
	 * @param originalBBox The object to clone.
	 * @param classType The class type of the new scene.
	 * @param originalCamera
	 * @return The preview scene with the cloned object.
	 */
	static b3Scene      *b3CreateBBox(b3BBox *originalBBox, b3_u32 classType,b3CameraPart *originalCamera = null);

	/**
	 * This method creates a scene for previewing materials. The scene contains a checker
	 * board. On the checker board is a sphere which has got the preview material.
	 *
	 * @param ptrMatHead This pointer receives the pointer to the example material map list base.
	 * @param classType The class type for scene rendering.
	 * @return The created scene.
	 */
	static b3Scene      *b3CreateMaterial(b3Base<b3Item> **ptrMatHead, b3_u32 classType);

	/**
	 * This method creates a scene for previewing bump mapping. The scene contains a checker
	 * board. On the checker board is a sphere which has got the preview bump map.
	 *
	 * @param ptrBumpHead This pointer receives the pointer to the example bump map list base.
	 * @param classType The class type for scene rendering.
	 * @return The created scene.
	 */
	static b3Scene      *b3CreateBump(b3Base<b3Item> **ptrBumpHead, b3_u32 classType);

private:
	static void          b3Consolidate(b3Scene *scene);
	static b3CameraPart *b3CreateCamera(b3Scene *scene,b3_f64 xAngle = 225,b3_f64 yAngle =  30);

};

#endif
