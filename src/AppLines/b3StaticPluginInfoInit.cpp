/*
**
**	$Filename:	b3StaticPluginInfoInit.cpp $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Adding static plugin infos
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Lines III includes                            **
**                                                                      **
*************************************************************************/

#include "AppLines.h"
#include "b3StaticPluginInfoInit.h"
#include "DlgMatNormal.h"
#include "DlgMatChess.h"
#include "DlgMatMarble.h"
#include "DlgMatWood.h"
#include "DlgMatSlide.h"
#include "DlgMatTexture.h"
#include "DlgMatWrapTexture.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2003/06/20 09:02:45  sm
**	- Added material dialog skeletons
**	- Fixed ticket no. 10 (camera dialog handled camera
**	  dimension wring)
**
**	Revision 1.1  2003/06/09 17:33:30  sm
**	- New item maintainance dialog added.
**	
*/

/*************************************************************************
**                                                                      **
**                        b3StaticPluginInfoInit implementation         **
**                                                                      **
*************************************************************************/

b3HashMap<b3_u32,b3_class_name> b3StaticPluginInfoInit::m_ClassNames;

void b3StaticPluginInfoInit::b3Init()
{
	b3Loader::b3SetUnknownIds(IDS_UNKNOWN,IDI_ITEM_UNKNOWN);

	b3AddClass(CLASS_SPECIAL,   IDS_CLASSDESC_SPECIAL);
	b3AddClass(CLASS_LIGHT,     IDS_CLASSDESC_LIGHT);
	b3AddClass(CLASS_MATERIAL,  IDS_CLASSDESC_MATERIAL);
	b3AddClass(CLASS_BUMP,      IDS_CLASSDESC_BUMP);
	b3AddClass(CLASS_CONDITION, IDS_CLASSDESC_CONDITION);

	b3Loader::b3AddClassType(CAMERA,       IDS_ITEMDESC_CAMERA);
	b3Loader::b3AddClassType(SUPERSAMPLE4, IDS_ITEMDESC_SUPERSAMPLE4);
	b3Loader::b3AddClassType(NEBULAR,      IDS_ITEMDESC_NEBULAR);
	b3Loader::b3AddClassType(LINES_INFO,   IDS_ITEMDESC_LINES_INFO);
	b3Loader::b3AddClassType(ANIMATION,    IDS_ITEMDESC_ANIMATION);
	b3Loader::b3AddClassType(DISTRIBUTE,   IDS_ITEMDESC_DISTRIBUTE);
	b3Loader::b3AddClassType(LENSFLARE,    IDS_ITEMDESC_LENSFLARE);
	b3Loader::b3AddClassType(CAUSTIC,      IDS_ITEMDESC_CAUSTIC);

	CDlgMatNormal::b3Register();
	CDlgMatChess::b3Register();
	CDlgMatMarble::b3Register();
	CDlgMatWood::b3Register();
	CDlgMatSlide::b3Register();
	CDlgMatTexture::b3Register();
	CDlgMatWrapTexture::b3Register();
}

const char *b3StaticPluginInfoInit::b3GetClassName(b3_u32 class_id)
{
	return m_ClassNames.b3Find(class_id)->name;
}

void b3StaticPluginInfoInit::b3AddClass(
	b3_u32    class_id,
	int       DescID)
{
	b3_class_name name;
	CString       aux;

	aux.LoadString(DescID);
	strcpy(name.name,aux);
	m_ClassNames.b3Add(class_id,name);
}