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
#include "blz3/base/b3Aux.h"
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
**	Revision 1.9  2003/02/25 15:56:21  sm
**	- Added SplineRot to control grid drawing.
**	- Added support for pixel format selection in dialog items
**	- Restructured b3PickInfo
**
**	Revision 1.8  2003/02/19 16:52:53  sm
**	- Cleaned up logging
**	- Clean up b3CPU/b3Runtime
**	
**	Revision 1.7  2002/08/10 16:07:46  sm
**	- Added some OS version output
**	- Corrected language specifiers for version output.
**	- Changed CDlgScene CSpinButtonCtrl to CB3IntSpinButtonCtrl
**	  to avoid thousands point.
**	
**	Revision 1.6  2002/02/27 20:14:52  sm
**	- Added stencil creation for creating simple shapes.
**	- Fixed material creation.
**	- Cleaned up some files.
**	
**	Revision 1.5  2002/01/20 12:48:51  sm
**	- Added splash screen
**	- Corrected repeat buttons (capture change)
**	
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

BEGIN_MESSAGE_MAP(CB3App, CWinApp)
	//{{AFX_MSG_MAP(CB3App)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

CB3App::CB3App(const char *appName) : 
	CWinApp(),
	CB3Reg(BLIZZARD3_REG_COMPANY,BLIZZARD3_REG_PRODUCT),
	CB3ToolbarState(appName)
{
	b3Path DebugFile;

	m_RunAutomated       = false;
	m_AutoSave           = true;
	m_ClientName         = appName;
	m_lastGC             = (HGLRC)0xdeadbeef;
	m_lastDC             = (HDC)0xbadc0ded;
	b3Log::b3GetLogFile(DebugFile);
	b3ReadString("Settings","DebugFile",DebugFile);
	b3Log::b3SetLogFile(DebugFile);
}

CB3App::~CB3App()
{
}


void CB3App::b3SelectRenderContext(HDC dc,HGLRC gc)
{
	if ((dc != m_lastDC) || (gc != m_lastGC) || (dc == 0) || (gc == 0) || (m_UncheckedContextSwitch))
	{
		b3PrintF(B3LOG_FULL,"######################################### CB3App::b3SelectRenderContext(HDC:0x%x,HGLRC:0x%x)\n",
			dc,gc);
		wglMakeCurrent(dc,gc);
		m_lastDC = dc;
		m_lastGC = gc;
	}
}

const char *CB3App::b3ClientName()
{
	B3_ASSERT(m_ClientName != null);
	return m_ClientName;
}

bool CB3App::b3InitInstance()
{
	CString key("GL unchecked context switch");

#ifdef USE_COOL_CONTROLS
	GetCtrlManager().InstallHook();
#endif
	m_UncheckedContextSwitch = b3ReadInt(b3ClientName(),key,0) != 0;
	b3WriteInt(b3ClientName(),key,m_UncheckedContextSwitch);
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
	b3PrintF (B3LOG_NORMAL,"### Blizzard III Screen information:\n");
	b3PrintF (B3LOG_NORMAL,"      Screen dimensions: %ldx%ld pixels\n",
		xSize,ySize);
	b3PrintF (B3LOG_NORMAL,"      Monitor dimensions: %2.2fx%2.2f inches "
		"(using %2.2f monitor)\n",
		width,height,sqrt(width * width + height * height));
	b3PrintF (B3LOG_NORMAL,"      Monitor dimensions: %ldx%ld mm²\n",
		dc->GetDeviceCaps(HORZSIZE),
		dc->GetDeviceCaps(VERTSIZE));
	b3PrintF (B3LOG_NORMAL,"      Using %ld bits per pixel\n",
		dc->GetDeviceCaps(BITSPIXEL));
	b3PrintF (B3LOG_NORMAL,"      Palette size %ld entries\n",
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

b3_f64 CB3App::b3ReadProfileFloat(const char *title,b3_f64 default_value)
{
	CString  value;

	value.Format("%lf",default_value);
	return atof(GetProfileString(b3ClientName(),title,value));
}

b3_bool CB3App::b3WriteProfileFloat(const char *title,b3_f64 default_value)
{
	CString  value;

	value.Format("%lf",default_value);
	return WriteProfileString(b3ClientName(),title,value);
}

void CB3App::b3ReadProfileVector(const char *title,b3_vector *default_vector)
{
	CString  heading;

	heading.Format("%s.x",title);
	default_vector->x = b3ReadProfileFloat(heading,default_vector->x);
	heading.Format("%s.y",title);
	default_vector->y = b3ReadProfileFloat(heading,default_vector->y);
	heading.Format("%s.z",title);
	default_vector->z = b3ReadProfileFloat(heading,default_vector->z);
}

b3_bool CB3App::b3WriteProfileVector(const char *title,const b3_vector *vector)
{
	CString  heading;
	b3_bool  success = true;

	heading.Format("%s.x",title);
	success &= b3WriteProfileFloat(heading,vector->x);
	heading.Format("%s.y",title);
	success &= b3WriteProfileFloat(heading,vector->y);
	heading.Format("%s.z",title);
	success &= b3WriteProfileFloat(heading,vector->z);
	return success;
};

void CB3App::b3ReadProfileColor(const char *title,b3_color *default_color)
{
	b3_pkd_color pkd = GetProfileInt(b3ClientName(),title,0);

	b3Color::b3GetColor(default_color,pkd);
}

b3_bool CB3App::b3WriteProfileColor(const char *title,const b3_color *color)
{
	b3_pkd_color pkd = b3Color::b3GetColor(color);
	
	return WriteProfileInt(b3ClientName(),title,pkd);
}

BOOL CB3App::PreTranslateMessage(MSG *pMSG)
{
	if (b3PreTranslateMsg(pMSG))
	{
		return true;
	}

	return CWinApp::PreTranslateMessage(pMSG);
}
