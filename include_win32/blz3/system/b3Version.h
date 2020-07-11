/*
**
**	$Filename:	b3Version.h $
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

#ifndef B3_SYSTEM_VERSION_H
#define B3_SYSTEM_VERSION_H

#include "blz3/b3Config.h"

#define B3_VERSION_LEN 128

class CB3Version
{
	b3_u32   m_VerMajor;
	b3_u32   m_VerMinor;
	b3_u32   m_RevMajor;
	b3_u32   m_RevMinor;
	b3_bool  m_PreRelease;
	CString  m_Version;
	CString  m_Copyright;
public:
	CB3Version(b3_bool no_cr = false);
	void         b3CopyVersionString(char * version);
	void         b3CopyCopyrightString(char * copyright);
	const char * b3GetVersionString();
	const char * b3GetCopyrightString();
	b3_bool      b3IsPreRelease();

	static void  b3DumpOS();
private:
	const char * b3GetFileInfo(void * info, const char * key, void ** ptr, unsigned int * len, DWORD langID);
};

/*
**	$Log$
**	Revision 1.2  2002/08/10 16:07:46  sm
**	- Added some OS version output
**	- Corrected language specifiers for version output.
**	- Changed CDlgScene CSpinButtonCtrl to CB3IntSpinButtonCtrl
**	  to avoid thousands point.
**
**	Revision 1.1  2002/08/05 16:04:52  sm
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

#endif
