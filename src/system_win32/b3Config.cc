/*
**
**	$Filename:	b3Config.c $
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

static b3Runtime static_runtime_environment;

b3Runtime::b3Runtime()
{
	b3_u32  value = 0x01020304;
	b3_u08 *ptr   = (b3_u08 *)&value;

	cpu_type = (ptr[0] == 0x01 ? B3_BIG_ENDIAN : B3_LITTLE_ENDIAN);
}

b3_cpu_type b3Runtime::b3GetCPUType()
{
	return static_runtime_environment.cpu_type;
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
