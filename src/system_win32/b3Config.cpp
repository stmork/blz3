/*
**
**	$Filename:	b3Config.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - basic platform configuration
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

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.5  2004/09/23 21:27:39  sm
**	- VBOs still don't work.
**
**	Revision 1.4  2004/09/23 20:02:25  sm
**	- Introduced VBOs on Windows - with success!
**	
**	Revision 1.3  2004/09/23 15:47:04  sm
**	- Splitted b3RenderContext into own file.
**	- Added vertex buffer object support which does not
**	  run yet.
**	
**	Revision 1.2  2003/02/19 16:52:53  sm
**	- Cleaned up logging
**	- Clean up b3CPU/b3Runtime
**	
**	Revision 1.1  2001/10/20 16:15:00  sm
**	- Some runtime environment cleanups. The CPU count is determined
**	  only once.
**	- Introduced preparing routines for raytring to shapes.
**	- Found 5% performance loss: No problem, this was eaten by
**	  bug fxing of the rotation spline shapes. (Phuu!)
**	- The next job is to implement different row sampler. Then we
**	  should implemented the base set of the Blizzard II raytracer.
**	
**	Revision 1.4  2001/07/08 12:30:07  sm
**	- New tool to remove nasty CR/LF from Windoze.
**	- Removing some nasty CR/LF with that new tool.
**	
**	Revision 1.3  2001/07/07 21:21:15  sm
**	- OK! Imported some display stuff using the CScrollView. After getting linked today
**	  it should possible to display real things tomorrow.
**	
**	Revision 1.2  2001/07/02 19:52:03  sm
**	- Cleaning up comments
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III runtime environment              **
**                                                                      **
*************************************************************************/

b3Runtime b3Runtime::m_Runtime;

b3Runtime::b3Runtime()
{
}

void b3Runtime::b3PSwap(
	b3_u32 *uPtr1,
	b3_u32 *uPtr2)
{
	b3_u32 aux;

	aux    = *uPtr1;
	*uPtr1 = *uPtr2;
	*uPtr2 = aux;
}

void b3Runtime::b3Beep()
{
	MessageBeep(0xffffffff);
}

b3_bool b3Runtime::b3Hostname(char *hostname,const b3_size buffer_size)
{
	unsigned long size = buffer_size;

	return GetComputerName (hostname,&size);
}

b3_s32 b3Runtime::b3Execute(const char *commandLine, const b3_bool async)
{
	PROCESS_INFORMATION procInfo;
	STARTUPINFO         strtInfo;

	strtInfo.cb          = sizeof(strtInfo);
	strtInfo.lpReserved  = null;
	strtInfo.lpDesktop   = null;
	strtInfo.lpTitle     = null;
	strtInfo.dwFlags     = 0;
	strtInfo.cbReserved2 = 0;
	strtInfo.lpReserved2 = null;
	if (!CreateProcess (null,(char *)commandLine,null,null,FALSE,
		CREATE_DEFAULT_ERROR_MODE,
		null,null,&strtInfo,&procInfo))
	{
		b3Beep();
	}
	else
	{
		if (!async) WaitForSingleObject (procInfo.hProcess,INFINITE);
	}
	return 0;
}

b3_msg_result b3Runtime::b3MessageBox(
	const char           *message,
	const b3_msgbox_type  type,
	const char           *title)
{
	long          win_msgbox_type;
	long          win_result;
	b3_msg_result result;

	switch (type)
	{
	case B3_MSGBOX_OK:
	default:
		win_msgbox_type = MB_OK|MB_ICONINFORMATION;
		break;

	case B3_MSGBOX_YES_NO:
		win_msgbox_type = MB_YESNO|MB_ICONQUESTION;
		break;

	case B3_MSGBOX_OK_CANCEL:
		win_msgbox_type = MB_OKCANCEL|MB_ICONQUESTION;
		break;

	case B3_MSGBOX_WARN:
		win_msgbox_type = MB_OK|MB_ICONWARNING;
		break;

	case B3_MSGBOX_ERROR:
		win_msgbox_type = MB_OK|MB_ICONERROR;
		break;
	}

	win_result = AfxMessageBox(message,win_msgbox_type);
	switch (win_result)
	{
	case IDOK:
		result = B3_MSG_OK;
		break;

	case IDCANCEL:
	default:
		result = B3_MSG_CANCEL;
		break;

	case IDYES:
		result = B3_MSG_YES;
		break;

	case IDNO:
		result = B3_MSG_NO;
		break;
	}
	return result;
}

b3_msg_result b3Runtime::b3MessageBox(
	const long            message_id,
	const b3_msgbox_type  type,
	const char           *title)
{
	CString message;

	if (!message.LoadString(message_id))
	{
		message.Format(
				"The text with the ID 0x%04lX is not available.\n"
				"Please contact your software vendor about this hazard!\n",
				message_id);
		b3PrintF (B3LOG_NORMAL,
			"### CLASS:  *Blizzard III*  # string ID 0x%04lx not available!\n",
			message_id);
	}
	return b3MessageBox(message,type,title);
}

void *b3Runtime::b3GetOpenGLExtension(const char *procedure_name)
{
#if 0
	HMODULE myself = GetModuleHandle(null);

	return GetProcAddress(myself,procedure_name);
#else
	return wglGetProcAddress(procedure_name);
#endif
}
