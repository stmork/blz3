/*
**
**	$Filename:	b3App.h $
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

#ifndef B3_SYSTEM_APP_H
#define B3_SYSTEM_APP_H

#include "blz3/system/b3Reg.h"
#include "blz3/system/b3Toolbar.h"

class CB3App : public CWinApp, public CB3Reg, public CB3ToolbarState
{
	const char   *m_ClientName;
public:
	bool          m_RunAutomated;
	bool          m_AutoSave;

public:
	              CB3App(const char *appName = "");
				 ~CB3App();
	        bool  b3InitInstance();
			const char *b3ClientName();
			bool  b3ExitInstance();
			void  b3GfxType(CWnd *wnd);
	virtual void  b3GetWindowMode();
	virtual void  b3SetWindowMode(bool ForceSave);
			void  b3MoveWindow(CWnd *wnd);
			void  b3CoolEnable(bool enable=true);

// From CWinApp
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

#define CB3GetApp()       ((CB3App *)AfxGetApp())
#define CB3ClientString() (CB3GetApp()->b3ClientName())

#endif
