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
	// OpenGL general values
	HGLRC         m_lastGC;
	HDC           m_lastDC;
	b3_bool       m_UncheckedContextSwitch;

	const char   *m_ClientName;
public:
	bool          m_RunAutomated;
	bool          m_AutoSave;

public:
	                    CB3App(const char *appName = "");
				       ~CB3App();
	        bool        b3InitInstance();
			const char *b3ClientName();
			bool        b3ExitInstance();
			void        b3GfxType(CWnd *wnd);
	virtual void        b3GetWindowMode();
	virtual void        b3SetWindowMode(bool ForceSave);
			void        b3MoveWindow(CWnd *wnd);
			void        b3CoolEnable(bool enable=true);
	        void        b3SelectRenderContext(HDC dc,HGLRC gc);
	        b3_f64      b3ReadProfileFloat(const char *title,b3_f64 default_value);
	        b3_bool     b3WriteProfileFloat(const char *title,b3_f64 default_value);
	        void        b3ReadProfileVector(const char *title,b3_vector *default_vector);
	        b3_bool     b3WriteProfileVector(const char *title,const b3_vector *vector);
	        void        b3ReadProfileColor(const char *title,b3_color *default_vector);
	        b3_bool     b3WriteProfileColor(const char *title,const b3_color *vector);

// From CWinApp
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CB3App)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CB3App)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#define CB3GetApp()       ((CB3App *)AfxGetApp())
#define CB3ClientString() (CB3GetApp()->b3ClientName())

#endif
