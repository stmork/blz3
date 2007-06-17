/*
**
**	$Filename:	b3Reg.h $
**	$Release:	Dortmund 2001 $
**	$Revision: 1025 $
**	$Date: 2005-01-09 16:04:30 +0100 (So, 09 Jan 2005) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - registry prototypes
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_REG_H
#define B3_SYSTEM_REG_H

#include "stdafx.h"

class CB3Reg
{
	HKEY hAppKey;
	HKEY hSoftKey;
	HKEY hCompanyKey;
public:
	        CB3Reg(const char *company,const char *application,HKEY key = HKEY_LOCAL_MACHINE);
	       ~CB3Reg();
	void    b3WriteString  (const char *client,const char *,const char *string);
	int     b3WriteInt     (const char *client,const char *,const int   value);
	void    b3WriteBinary  (const char *client,const char *,const void *buffer,const b3_size size);
	void    b3ReadString   (const char *client,const char *,char       *string);
	int     b3ReadInt      (const char *client,const char *,int         value);
	void   *b3ReadBinary   (const char *client,const char *,b3_size     &size);

protected:
	HKEY    b3GetSectionKey(const char *section_name);
};

#endif

