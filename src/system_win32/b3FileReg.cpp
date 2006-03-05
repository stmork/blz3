/*
**
**	$Filename:	b3FileReg.c $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - IO into Windows registry
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

#include "b3SystemInclude.h"
#include "blz3/system/b3FileReg.h"
#include "blz3/system/b3App.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.5  2006/03/05 21:22:36  sm
**	- Added precompiled support for faster comiling :-)
**
**	Revision 1.4  2005/06/10 21:31:43  sm
**	- Vectorization fix for Windows
**	- Read binary from registry returns null when no
**	  entry is in registry.
**	
**	Revision 1.3  2005/01/02 19:15:25  sm
**	- Fixed signed/unsigned warnings
**	
**	Revision 1.2  2004/05/09 16:37:59  sm
**	- Added scaling property page to oak plank material
**	- Corrected so,e input parameters.
**	
**	Revision 1.1  2004/04/25 13:40:59  sm
**	- Added file saving into registry
**	- Added last b3Item state saving for cloned b3Item
**	  creation.
**	- Now saving refresh state per b3Item dialog
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        b3FileReg implementation                      **
**                                                                      **
*************************************************************************/

b3FileReg::b3FileReg  () :
	m_Reg(CB3Reg(BLIZZARD3_REG_COMPANY,BLIZZARD3_REG_PRODUCT,HKEY_CURRENT_USER))
{
	m_Pos = 0;
}

b3FileReg::b3FileReg  (const char *file_name,const b3_access_mode mode) :
	m_Reg(CB3Reg(BLIZZARD3_REG_COMPANY,BLIZZARD3_REG_PRODUCT,HKEY_CURRENT_USER))
{
	if (!b3Open(file_name,mode))
	{
		B3_THROW(b3FileException,B3_FILE_NOT_FOUND);
	}
}

b3FileReg::~b3FileReg  ()
{
}

b3_bool  b3FileReg::b3Open      (const char *file_name,const b3_access_mode mode)
{
	m_Mode = mode;
	m_Buffer.b3Clear();
	m_FileName = file_name;
	switch (mode)
	{
		case B_READ :
		case T_READ :
			if (!b3ReadInternal())
			{
				return false;
			}
			m_Pos = 0;
			break;

		case B_WRITE :
		case T_WRITE :
			m_Pos = 0;
			break;

		case B_APPEND :
		case T_APPEND :
			if (!b3ReadInternal())
			{
				return false;
			}
			break;

		default:
			return false;
	}
	return true;
}

b3_bool b3FileReg::b3ReadInternal()
{
	b3_size size;
	b3_u08 *buffer = (b3_u08 *)m_Reg.b3ReadBinary(CB3ClientString(),m_FileName,size);

	if (buffer != null)
	{
		for (m_Pos = 0;m_Pos < size;m_Pos++)
		{
			m_Buffer.b3Add(buffer[m_Pos]);
		}
		free(buffer);
	}
	return buffer != null;
}

b3_size  b3FileReg::b3Read      (void *read_buffer,const b3_size size)
{
	b3_u08  *src = m_Buffer.b3GetBuffer();
	b3_size  end = B3_MIN(m_Pos + size,(b3_size)m_Buffer.b3GetCount());

	memcpy(read_buffer,src,end - m_Pos);
	return end - m_Pos;
}

b3_u08 *b3FileReg::b3ReadBuffer(const char *filename,b3_size &filesize)
{
	m_Mode = B_READ;
	m_FileName = filename;

	if (b3ReadInternal())
	{
		filesize = m_Buffer.b3GetCount();
		return m_Buffer.b3GetBuffer();
	}
	return null;
}

b3_size  b3FileReg::b3Write     (const void * write_buffer,const b3_size size)
{
	const b3_u08 *buffer = (const b3_u08 *)write_buffer;
	b3_size       i;

	for (i = 0;i < size;i++)
	{
		if (m_Pos < (b3_size)m_Buffer.b3GetCount())
		{
			m_Buffer[(b3_index)m_Pos] = buffer[i];
			m_Pos++;
		}
		else
		{
			m_Buffer.b3Add(buffer[i]);
			m_Pos++;
		}
	}
	return i;
}

b3_bool  b3FileReg::b3Flush     ()
{
	return true;
}

b3_size  b3FileReg::b3Seek      (const b3_offset offset,const b3_seek_type type)
{
	return 0;
}

b3_size  b3FileReg::b3Size      ()
{
	return m_Buffer.b3GetCount();
}

b3_bool  b3FileReg::b3Buffer    (const b3_size new_cache_size)
{
	return true;
}

void b3FileReg::b3Close()
{
	switch (m_Mode)
	{
		case B_WRITE :
		case T_WRITE :
		case B_APPEND :
		case T_APPEND :
			m_Reg.b3WriteBinary(CB3ClientString(),m_FileName,m_Buffer.b3GetBuffer(),m_Buffer.b3GetCount());
			break;
	}
}
