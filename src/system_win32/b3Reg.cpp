/*
**
**	$Filename:	b3Reg.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Windows registry routines (for HKEY_LOCAL_MACHINE)
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
#include "blz3/system/b3Reg.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2001/08/11 15:59:59  sm
**	- Rendering cleaned up
**	- CWinApp/CMainFrm derived from Blizzard III classes
**	  supporting more effective GUI.
**
**	
*/

/*************************************************************************
**                                                                      **
**                        con/destructor                                **
**                                                                      **
*************************************************************************/

CB3Reg::CB3Reg(
	const char *company,
	const char *software)
{
	hAppKey     = NULL;
	hSoftKey    = NULL;
	hCompanyKey = NULL;
	if (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, "software", 0, KEY_WRITE|KEY_READ,
		&hSoftKey) == ERROR_SUCCESS)
	{
		unsigned long mode;

		if (::RegCreateKeyEx(hSoftKey, company, 0, REG_NONE,
			REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
			&hCompanyKey, &mode) == ERROR_SUCCESS)
		{
			::RegCreateKeyEx(hCompanyKey, software, 0, REG_NONE,
				REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
				&hAppKey, &mode);
		}
	}

}

CB3Reg::~CB3Reg()
{
	if (hCompanyKey != NULL)
	{
		::RegCloseKey(hCompanyKey);
	}
	if (hSoftKey != NULL)
	{
		::RegCloseKey(hSoftKey);
	}
	if (hAppKey != NULL)
	{
		::RegCloseKey(hAppKey);
	}
}

HKEY CB3Reg::b3GetSectionKey(const char *section)
{

	HKEY hSectionKey = NULL;

	if (hAppKey)
	{
		unsigned long mode;

		::RegCreateKeyEx(hAppKey, section, 0, REG_NONE,
			REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
			&hSectionKey,&mode);
	}
	return hSectionKey;
}

void CB3Reg::b3WriteString(const char *section,const char *entry,const char *string)
{
	HKEY hSecKey = b3GetSectionKey(section);

	if (hSecKey)
	{
		::RegSetValueEx(hSecKey, entry,NULL,REG_SZ, 
			(const unsigned char *)string, (strlen(string) + 1) * sizeof(TCHAR));
		::RegCloseKey (hSecKey);
	}
}

void CB3Reg::b3ReadString(const char *section,const char *entry,char *string)
{
	HKEY hSecKey = b3GetSectionKey(section);

	if (hSecKey)
	{
		unsigned long mode = 0,len = 0;

		if (::RegQueryValueEx(hSecKey, entry, NULL, &mode, null, &len) == ERROR_SUCCESS)
		{
			if (mode == REG_SZ)
			{
				::RegQueryValueEx(hSecKey, entry, NULL, &mode, (unsigned char *)string, &len);
			}
		}
		::RegCloseKey (hSecKey);
	}
}

int CB3Reg::b3WriteInt(const char *section,const char *entry,const int def)
{
	HKEY hSecKey = b3GetSectionKey(section);

	if (hSecKey)
	{
		RegSetValueEx(hSecKey, entry, NULL, REG_DWORD,
			(LPBYTE)&def, sizeof(def));
		RegCloseKey(hSecKey);
	}
	return def;
}

int CB3Reg::b3ReadInt(const char *section,const char *entry,int def)
{
	HKEY hSecKey = b3GetSectionKey(section);

	if (hSecKey)
	{
		unsigned long len = sizeof(int),value = 0,type;

		if (::RegQueryValueEx(hSecKey, entry, NULL, &type,(LPBYTE)&value, &len) == ERROR_SUCCESS)
		{
			if ((type == REG_DWORD) && (len == sizeof(int)))
			{
				def = value;
			}
		}
		::RegCloseKey(hSecKey);
	}
	return def;
}
