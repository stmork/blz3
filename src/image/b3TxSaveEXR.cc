/*
**
**	$Filename:	b3TxSaveEXR.cc $
**	$Release:	Dortmund 2005 $
**	$Revision:	2.04 
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Saving an OpenEXR image
**
**	(C) Copyright 2005  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3TxInclude.h"

#ifdef BLZ3_USE_OPENEXR
#include <ImfOutputFile.h>
#include <ImfChannelList.h>
#include <ImfHeader.h>
#include <ImfIO.h>

using namespace std;
using namespace Imf;
using namespace Imath;
#endif

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.4  2006/03/05 21:22:34  sm
**	- Added precompiled support for faster comiling :-)
**
**	Revision 1.3  2005/10/15 16:43:03  sm
**	- Added HDR texture access.
**	
**	Revision 1.2  2005/10/10 18:51:22  sm
**	- Added OpenEXR image saving.
**	
**	Revision 1.1  2005/10/09 12:05:34  sm
**	- Changed to HDR image computation.
**	
*/

/*************************************************************************
**                                                                      **
**                          EXR                                         **
**                                                                      **
*************************************************************************/

#ifdef BLZ3_USE_OPENEXR
class b3InfoEXR : protected b3TxSaveInfo, protected OStream
{
public:
	b3InfoEXR(b3Tx *tx,const char *filename) :
		b3TxSaveInfo(tx, filename, B3_BWRITE),
		OStream(filename)
	{
		if (!tx->b3IsHDR())
		{
			B3_THROW(b3TxException, B3_TX_ILLEGAL_DATATYPE);
		}
		b3PrintF(B3LOG_FULL,"  b3InfoEXR initialized.\n");
	}

	virtual void        write (const char c[/*n*/], int n)
	{
		clearerr (m_FileHandle);

		if (fwrite (c, 1, n, m_FileHandle) != n)
		{
			B3_THROW(b3TxException, B3_TX_NOT_SAVED);
		}
	}

	virtual Int64       tellp ()
	{
		return ftell (m_FileHandle);
	}

	virtual void        seekp (Int64 pos)
	{
		clearerr (m_FileHandle);
		fseek    (m_FileHandle, pos, SEEK_SET);
	}

	void  b3Write()
	{
		try
		{
			b3_color    *data = (b3_color *)m_Tx->b3GetData();
			Header       header (m_Tx->xSize, m_Tx->ySize);

			header.compression() = ZIP_COMPRESSION;
			header.channels().insert("R", Channel(FLOAT));
			header.channels().insert("G", Channel(FLOAT));
			header.channels().insert("B", Channel(FLOAT));

			OutputFile   file (*this, header);
			FrameBuffer  fb;

			fb.insert("R", Slice(FLOAT,(char *)&data->r,sizeof(b3_color),sizeof(b3_color) * m_Tx->xSize));
			fb.insert("G", Slice(FLOAT,(char *)&data->g,sizeof(b3_color),sizeof(b3_color) * m_Tx->xSize));
			fb.insert("B", Slice(FLOAT,(char *)&data->b,sizeof(b3_color),sizeof(b3_color) * m_Tx->xSize));

			file.setFrameBuffer(fb);
			file.writePixels(m_Tx->ySize);
		}
		catch(const std::exception &e)
		{
			b3PrintF(B3LOG_NORMAL,"Error writing OpenEXR file %s\n",e.what());
			B3_THROW(b3TxException, B3_TX_NOT_SAVED);
		}
	}

	virtual ~b3InfoEXR()
	{
	}
};
#endif

b3_result b3Tx::b3SaveEXR(const char *filename)
{
#ifdef BLZ3_USE_OPENEXR
	b3PrintF(B3LOG_FULL, "Saving EXR: %s\n", filename);

	b3InfoEXR info(this,filename);
	info.b3Write();
	return B3_OK;
#else
	return B3_ERROR;
#endif
}
