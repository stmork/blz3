/*
**
**	$Filename:	b3UndoLight.h $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Undo/Redo for light handling
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifndef B3UNDOLIGHT_H
#define B3UNDOLIGHT_H

#include "AppLines.h"
#include "b3Undo.h"

#include "blz3/raytrace/b3Light.h"
#include "blz3/raytrace/b3Scene.h"

class b3OpLight : public b3Operation
{
protected:
	b3Scene * m_Scene;

protected:
	inline b3OpLight(b3Scene * scene)
	{
		m_Scene = scene;
		m_PrepareGeometry =
			m_PrepareChangedStructure = false;
	}
};

class b3OpLightCreate : public b3OpLight
{
	b3Light    *    m_Light;
	b3Light    *    m_Prev;
	b3Base<b3Item> * m_Base;

public:
	b3OpLightCreate(b3Scene * scene, b3Light * light);

protected:
	void b3Prepare(CAppRenderDoc * pDoc);
	void b3Undo();
	void b3Redo();
	void b3Delete();

	inline int  b3GetId()
	{
		return IDS_OP_LIGHT_CREATE;
	}
};

class b3OpLightDelete : public b3OpLight
{
	b3Light    *    m_Select;
	b3Light    *    m_Light;
	b3Item     *    m_Prev;
	b3Base<b3Item> * m_Base;

public:
	b3OpLightDelete(b3Scene * scene, b3Light * light);

protected:
	void b3Prepare(CAppRenderDoc * pDoc);
	void b3Undo();
	void b3Redo();
	void b3Delete();

	inline int  b3GetId()
	{
		return IDS_OP_LIGHT_DELETE;
	}
};

class b3OpLightMode : public b3Operation
{
	b3_bool * m_FlagPtr;
	b3_bool  m_Mode;

protected:
	b3OpLightMode(b3_bool * ptr);

	void b3Prepare(CAppRenderDoc * pDoc);
	void b3Undo();
	void b3Redo();
};

class b3OpLightEnable : public b3OpLightMode
{
public:
	inline b3OpLightEnable(b3Light * light) : b3OpLightMode(&light->m_LightActive)
	{
	}

	inline int  b3GetId()
	{
		return IDS_OP_LIGHT_ENABLE;
	}
};

class b3OpLightSpot : public b3OpLightMode
{
public:
	inline b3OpLightSpot(b3Light * light) : b3OpLightMode(&light->m_SpotActive)
	{
	}

	inline int  b3GetId()
	{
		return IDS_OP_LIGHT_SPOT;
	}
};

class b3OpLightSoft : public b3OpLightMode
{
public:
	inline b3OpLightSoft(b3Light * light) : b3OpLightMode(&light->m_SoftShadow)
	{
	}

	inline int  b3GetId()
	{
		return IDS_OP_LIGHT_SOFT;
	}
};

#endif
