// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#ifndef BLZ3_STDAFX_H
#define BLZ3_STDAFX_H

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Selten verwendete Teile der Windows-Header nicht einbinden
#endif

// �ndern Sie folgende Definitionen f�r Plattformen, die �lter als die unten angegebenen sind.
// Unter MSDN finden Sie die neuesten Informationen �ber die entsprechenden Werte f�r die unterschiedlichen Plattformen.
#ifndef WINVER				// Lassen Sie die Verwendung von Features spezifisch f�r Windows 95 und Windows NT 4 oder sp�ter zu.
#define WINVER 0x0400		// �ndern Sie den entsprechenden Wert, um auf Windows 98 und mindestens Windows 2000 abzuzielen.
#endif

#ifndef _WIN32_WINNT		// Lassen Sie die Verwendung von Features spezifisch f�r Windows NT 4 oder sp�ter zu.
#define _WIN32_WINNT 0x0400		// �ndern Sie den entsprechenden Wert, um auf Windows 98 und mindestens Windows 2000 abzuzielen.
#endif						

#ifndef _WIN32_WINDOWS		// Lassen Sie die Verwendung von Features spezifisch f�r Windows 98 oder sp�ter zu.
#define _WIN32_WINDOWS 0x0410 // �ndern Sie den entsprechenden Wert, um auf mindestens Windows Me abzuzielen.
#endif

#ifndef _WIN32_IE			// Lassen Sie die Verwendung von Features spezifisch f�r IE 4.0 oder sp�ter zu.
#define _WIN32_IE 0x0400	// �ndern Sie den entsprechenden Wert, um auf mindestens IE 5.0 abzuzielen.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// einige CString-Konstruktoren sind explizit

// Deaktiviert das Ausblenden von einigen h�ufigen und oft ignorierten Warnungen
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC-Kern- und -Standardkomponenten
#include <afxext.h>         // MFC-Erweiterungen
#include <afxdisp.h>        // MFC-Automatisierungsklassen

#include <afxdtctl.h>		// MFC-Unterst�tzung f�r allgemeine Steuerelemente von Internet Explorer 4
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC-Unterst�tzung f�r allgemeine Windows-Steuerelemente
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>		// MFC-Socket-Erweiterungen

#endif
