/*
**
**	$Filename:	b3Version.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Version handling
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/system/b3Version.h"
#include "blz3/system/b3Dir.h"
#include "blz3/system/b3Date.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.4  2005/01/02 21:18:34  sm
**	- Changed version output
**
**	Revision 1.3  2003/08/28 14:44:27  sm
**	- Further buffer overflow prevention:
**	  o added b3Path::b3Format
**	  o added b3Path::b3Append
**	- Further strcat/strcpy removal necessary
**	
**	Revision 1.2  2002/08/10 16:07:46  sm
**	- Added some OS version output
**	- Corrected language specifiers for version output.
**	- Changed CDlgScene CSpinButtonCtrl to CB3IntSpinButtonCtrl
**	  to avoid thousands point.
**	
**	Revision 1.1  2002/08/05 16:04:55  sm
**	- Found first texture init bug. This wasn't an OpenGL bug. This
**	  couldn't be because every implementation had got the same
**	  bug. The static aux image for creating textures wasn't initialized
**	  at the right time.
**	- Version handling introduced: The version number is extracted
**	  from the version resource now.
**	- The b3Tx::b3AllocTx() method uses b3Realloc() for better
**	  memory usage.
**	- Some b3World messages removed.
**	- The 0x7fff class is registered into the b3ItemRegister now. This
**	  prevents printing a warning when this class isn't found. Due to
**	  the fact that *every* Blizzard data contains this class every
**	  data read put out this warning.
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        CB3Version: Gets Blizzard product version     **
**                                                                      **
*************************************************************************/

CB3Version::CB3Version(b3_bool no_cr)
{
	         CWinApp *app = AfxGetApp();
			 void    *ptr;
	         void    *info;
			 char    *debug;
			 char    *cr;
	         b3Path   FileName(app->m_pszHelpFilePath);
	unsigned long     handle;
	unsigned int      len;
	b3Date            today;

	// Predefine version information
	m_VerMajor   =  B3_VERSION;
	m_VerMinor   =  B3_REVISION;
	m_RevMajor   =  0;
	m_RevMinor   =  0;
	m_PreRelease = false;

#ifdef _DEBUG
	debug = " (DEBUG)";
#else
	debug = "";
#endif

	cr = (no_cr ? " - " : "\n");

	// We assume that the help file is at the same
	// position like this *.exe. So get *.hlp with
	// full path and convert it to the *.exe.
	FileName.b3RemoveExt();
	FileName.b3Append(".exe");

	// Now we get a handle about this *.exe information
	len = GetFileVersionInfoSize(FileName, &handle);
	if (len > 0)
	{
		// We need some memory for the information
		info = calloc(len,1);
		if (info)
		{
			// We get the information, and...
			if (GetFileVersionInfo(FileName, handle, len, info) != 0)
			{
				DWORD lang = 0x040704b0;

				// We get the version information
				if (VerQueryValue(info, "\\", &ptr, &len) != 0)
				{
					VS_FIXEDFILEINFO *vsf = (VS_FIXEDFILEINFO *)ptr;

					// Extract product version
					m_VerMajor   = HIWORD(vsf->dwProductVersionMS);
					m_VerMinor   = LOWORD(vsf->dwProductVersionMS);
					m_RevMajor   = HIWORD(vsf->dwProductVersionLS);
					m_RevMinor   = LOWORD(vsf->dwProductVersionLS);
					m_PreRelease = (vsf->dwFileFlags & vsf->dwFileFlagsMask & VS_FF_PRERELEASE) != 0;
				}
				if (VerQueryValue(info, "VarFileInfo\\Translation", &ptr, &len) != 0)
				{
					USHORT *var = (USHORT *)ptr;

					lang = MAKELONG(var[1],var[0]);
				}
				CString company(  b3GetFileInfo(info,"CompanyName" ,   &ptr, &len,lang));
				CString copyright(b3GetFileInfo(info,"LegalCopyright", &ptr, &len,lang));
			
				m_Copyright.Format("%s 2001 - %lu by%s%s",
					copyright,today.year,cr,company);
			}
			free (info);
		}
	}

	// Setup version information from this exe file.
	m_Version.Format ("Version %u.%u Revision %u.%u%s%s%s%s\n",
		m_VerMajor,m_VerMinor,m_RevMajor,m_RevMinor,
		b3IsPreRelease() ? "pre" : "",cr,__DATE__,debug);
}

const char *CB3Version::b3GetFileInfo(
	void          *info,
	const char    *key,
	void         **ptr,
	unsigned int  *len,
	DWORD          lang)
{
	CString     StringFileInfoKey;
	const char *result = "";

	StringFileInfoKey.Format("\\StringFileInfo\\%08x\\%s",lang,key);
	if (VerQueryValue(info,(char *)((const char*)StringFileInfoKey),ptr,len) != 0)
	{
		result = (const char *)*ptr;
	}
	return result;
}

void CB3Version::b3DumpOS()
{
	OSVERSIONINFO version_info;
	b3Date        today;
	b3_count      CPUs = b3Runtime::b3GetNumCPUs();

	version_info.dwOSVersionInfoSize = sizeof(version_info);
	GetVersionEx(&version_info);

	// Dumping system information
	b3PrintF (B3LOG_NORMAL,"### Blizzard III started on %02ld.%02ld.%ld - %02ld:%02ld:%02ld\n",
		today.day, today.month,today.year,
		today.hour,today.min,  today.sec);
	b3PrintF (B3LOG_NORMAL,"      Compiled with MS VC++: V%2.02f\n",
		(double)_MSC_VER / 100.0);
	b3PrintF (B3LOG_NORMAL,"      Optimized for an i%d processor.\n",_M_IX86 + 86);
	b3PrintF (B3LOG_NORMAL,"      OS-Version: %ld.%ld Build: %ld %s%s%s\n",
		version_info.dwMajorVersion,version_info.dwMinorVersion,
		IS_NT ? version_info.dwBuildNumber : LOWORD(version_info.dwBuildNumber),
		version_info.szCSDVersion,
		IS_NT ?    " [Windows NT]" : "",
		IS_WIN95 ?
			(version_info.dwMinorVersion == 0 ?
				" [Windows 95]" :
				" [Windows 98]") :
			"");
	b3PrintF (B3LOG_NORMAL,"      MFC version: %d.%02d\n",
		_MFC_VER >> 8, _MFC_VER & 0xff);

	switch (CPUs)
	{
	case 1:
		b3PrintF(B3LOG_NORMAL,"      Found only one CPU.\n");
		break;

	case 2:
		b3PrintF(B3LOG_NORMAL,"      Found two CPUs - Good.\n");
		break;

	default:
		B3_ASSERT(CPUs > 2);
		b3PrintF(B3LOG_NORMAL,"      Found %d CPUs - Fine!\n",CPUs);
		break;
	}
	b3PrintF (B3LOG_NORMAL,"\n");
}

// Put version string into buffer
void CB3Version::b3CopyVersionString (char *version_ptr)
{
	strcpy (version_ptr,m_Version);
}

// Put version string into buffer
void CB3Version::b3CopyCopyrightString (char *copyright_ptr)
{
	strcpy (copyright_ptr,m_Copyright);
}

// Return version string
const char *CB3Version::b3GetVersionString()
{
	return m_Version;
}

// Return copyright string
const char *CB3Version::b3GetCopyrightString()
{
	return m_Copyright;
}

// Return if this is a pre release
b3_bool CB3Version::b3IsPreRelease()
{
	return m_PreRelease;
}
