/*
**
**	$Filename:	b3App.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - application base class
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/b3Config.h"
#include "blz3/system/b3App.h"
#include "blz3/system/b3Dir.h"
#include <math.h>

#ifndef _DEBUG
#define USE_COOL_CONTROLS
#endif

#ifdef USE_COOL_CONTROLS
#include "..\smart_gui\CoolControlsManager.h"
#endif

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.4  2001/12/26 12:00:36  sm
**	- Fixed modeller info dialog
**
**	Revision 1.3  2001/08/16 14:41:24  sm
**	- Some more shading shapes added (only BSPline shapes are missing)
**	
**	Revision 1.2  2001/08/14 13:34:40  sm
**	- Corredted aspect ratio handling when doing somethiing with
**	  the view
**	- New application icon
**	- Minor GUI bug fixes.
**	
**	Revision 1.1  2001/08/11 15:59:59  sm
**	- Rendering cleaned up
**	- CWinApp/CMainFrm derived from Blizzard III classes
**	  supporting more effective GUI.
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III: b3App                           **
**                                                                      **
*************************************************************************/

CB3App::CB3App(const char *appName) : 
	CWinApp(),
	CB3Reg(BLIZZARD3_REG_COMPANY,BLIZZARD3_REG_PRODUCT),
	CB3ToolbarState(appName)
{
	b3Path DebugFile;

	m_RunAutomated       = false;
	m_AutoSave           = true;
	m_ClientName         = appName;
	b3Log_GetFile(DebugFile);
	b3ReadString("Settings","DebugFile",DebugFile);
	b3Log_SetFile(DebugFile);
}

CB3App::~CB3App()
{
}

const char *CB3App::b3ClientName()
{
	B3_ASSERT(m_ClientName != null);
	return m_ClientName;
}

bool CB3App::b3InitInstance()
{
#ifdef USE_COOL_CONTROLS
	GetCtrlManager().InstallHook();
#endif
	return true;
}

bool CB3App::b3ExitInstance()
{
#ifdef USE_COOL_CONTROLS
	GetCtrlManager().UninstallHook();
#endif
	return true;
}

void CB3App::b3CoolEnable(bool enable)
{
#ifdef USE_COOL_CONTROLS
	GetCtrlManager().Enable(enable);
#endif
}

void CB3App::b3GfxType(CWnd *wnd)
{
	// Dumping screen infos
	CDC    *dc;
	long    xSize,ySize,pSize;
	double  width,height;

	dc     = wnd->GetDC();
	xSize  = dc->GetDeviceCaps(HORZRES);
	ySize  = dc->GetDeviceCaps(VERTRES);
	pSize  = dc->GetDeviceCaps(NUMCOLORS);
	width  = (double)xSize / dc->GetDeviceCaps(LOGPIXELSX);
	height = (double)ySize / dc->GetDeviceCaps(LOGPIXELSY);
	b3PrintF (B3LOG_NORMAL,"### Blizzard III DUMPING SCREEN INFORMATION...\n");
	b3PrintF (B3LOG_NORMAL,"### B3 Screen dimensions: %ldx%ld pixels\n",
		xSize,ySize);
	b3PrintF (B3LOG_NORMAL,"### B3 Monitor dimensions: %2.2fx%2.2f inches "
		"(using %2.2f monitor)\n",
		width,height,sqrt(width * width + height * height));
	b3PrintF (B3LOG_NORMAL,"### B3 Monitor dimensions: %ldx%ld mm²\n",
		dc->GetDeviceCaps(HORZSIZE),
		dc->GetDeviceCaps(VERTSIZE));
	b3PrintF (B3LOG_NORMAL,"### B3 Using %ld bits per pixel\n",
		dc->GetDeviceCaps(BITSPIXEL));
	b3PrintF (B3LOG_NORMAL,"### B3 Palette size %ld entries\n",
		pSize < 0 ? 0 : pSize);
	wnd->ReleaseDC(dc);
}

void CB3App::b3GetWindowMode()
{
	long mode;

	m_AutoSave = GetProfileInt(b3ClientName(),"AutoSave",1) != 0;
	mode       = GetProfileInt(b3ClientName(),"ShowCmd",1);
	if (mode == 0)
	{
		m_nCmdShow = SW_SHOWNORMAL;
	}
	if (mode < 0)
	{
		m_nCmdShow = SW_SHOWMINIMIZED;
	}
	if (mode > 0)
	{
		m_nCmdShow = SW_SHOWMAXIMIZED;
	}
}

void CB3App::b3MoveWindow(CWnd *wnd)
{
	CRect    rect;

	wnd->GetWindowRect (&rect);
	rect.left   = GetProfileInt(b3ClientName(),"xMin",rect.left);
	rect.right  = GetProfileInt(b3ClientName(),"xMax",rect.right);
	rect.top    = GetProfileInt(b3ClientName(),"yMin",rect.top);
	rect.bottom = GetProfileInt(b3ClientName(),"yMax",rect.bottom);
	wnd->MoveWindow    (&rect);
}

void CB3App::b3SetWindowMode(bool ForceSave)
{
	CRect rect;
	long  mode = 0;

	WriteProfileInt (b3ClientName(),"AutoSave",m_AutoSave); 

	// Check error case
	if (m_pMainWnd->GetSafeHwnd() == 0)
	{
		return;
	}

	if (m_AutoSave || ForceSave)
	{
		if (m_pMainWnd->IsIconic())
		{
			mode = -1;
		}
		if (m_pMainWnd->IsZoomed())
		{
			mode =  1;
		}
		if (mode == 0)
		{
			m_pMainWnd->GetWindowRect     (&rect);
			WriteProfileInt (b3ClientName(),"xMin",   rect.left);
			WriteProfileInt (b3ClientName(),"xMax",   rect.right);
			WriteProfileInt (b3ClientName(),"yMin",   rect.top);
			WriteProfileInt (b3ClientName(),"yMax",   rect.bottom);
		}
		WriteProfileInt (b3ClientName(),"ShowCmd",mode);
		b3SaveState();
	}
}

BOOL CB3App::PreTranslateMessage(MSG *pMSG)
{
	if (b3PreTranslateMsg(pMSG))
	{
		return true;
	}

	return CWinApp::PreTranslateMessage(pMSG);
}
