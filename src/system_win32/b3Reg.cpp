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
**	Revision 1.3  2005/01/02 19:15:25  sm
**	- Fixed signed/unsigned warnings
**
**	Revision 1.2  2004/04/25 13:40:59  sm
**	- Added file saving into registry
**	- Added last b3Item state saving for cloned b3Item
**	  creation.
**	- Now saving refresh state per b3Item dialog
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
**                        con/destructor                                **
**                                                                      **
*************************************************************************/

CB3Reg::CB3Reg(
	const char *company,
	const char *software,
	HKEY        key)
{
	hAppKey     = NULL;
	hSoftKey    = NULL;
	hCompanyKey = NULL;
	if (::RegOpenKeyEx(key, "software", 0, KEY_WRITE|KEY_READ,
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
			(const unsigned char *)string, (DWORD)((strlen(string) + 1) * sizeof(TCHAR)));
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

void CB3Reg::b3WriteBinary(const char *section,const char *entry,const void *buffer,const b3_size size)
{
	HKEY hSecKey = b3GetSectionKey(section);

	if (hSecKey)
	{
		LONG result = ::RegSetValueEx(hSecKey, entry, NULL, REG_BINARY, (LPBYTE)buffer, (DWORD)size);
		::RegCloseKey(hSecKey);
	}
}

void *CB3Reg::b3ReadBinary(const char *section,const char *entry,b3_size &size)
{
	HKEY  hSecKey = b3GetSectionKey(section);
	void *buffer  = null;

	if (hSecKey)
	{
		unsigned long len,type;

		// Determine size
		if (::RegQueryValueEx(hSecKey, entry, NULL, &type,NULL, &len) == ERROR_SUCCESS)
		{
			size = len;
			buffer = malloc(size);

			if (buffer != null)
			{
				// Read buffer
				if (::RegQueryValueEx(hSecKey, entry, NULL, &type,(LPBYTE)buffer, &len) == ERROR_SUCCESS)
				{
					if ((type != REG_BINARY) || (len != size))
					{
						free (buffer);
						buffer = null;
					}
				}
			}
		}
		::RegCloseKey(hSecKey);
	}
	return buffer;
}
