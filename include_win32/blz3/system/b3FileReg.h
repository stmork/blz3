/*
**
**	$Filename:	b3FileReg.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - IO into Windows registry
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_FILEREG_H
#define B3_SYSTEM_FILEREG_H

#include "stdafx.h"
#include "blz3/system/b3FileAbstract.h"
#include "blz3/system/b3Mem.h"
#include "blz3/system/b3Reg.h"
#include "blz3/system/b3Dir.h"
#include "blz3/base/b3Array.h"

class b3FileReg : public b3FileAbstract, public b3Mem
{
	CB3Reg          m_Reg;
	b3Array<b3_u08> m_Buffer;
	b3Path          m_FileName;
	b3_size         m_Pos;
	b3_access_mode  m_Mode;

public:
	b3FileReg();
	b3FileReg(const char * file_name, const b3_access_mode mode);
	~b3FileReg();

	b3_bool  b3Open(const char * file_name, const b3_access_mode mode);
	b3_size  b3Read(void * read_buffer, const b3_size size);
	b3_u08 * b3ReadBuffer(const char * filename, b3_size & filesize);
	b3_size  b3Write(const void * write_buffer, const b3_size size);
	b3_bool  b3Flush();
	b3_size  b3Seek(const b3_offset offset, const b3_seek_type type);
	b3_size  b3Size();
	b3_bool  b3Buffer(const b3_size new_cache_size);
	void     b3Close();

private:
	b3_bool  b3ReadInternal();
};

#endif
