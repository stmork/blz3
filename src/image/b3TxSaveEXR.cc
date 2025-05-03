/*
**
**	$Filename:	b3TxSaveEXR.cc $
**	$Release:	Dortmund 2011, 2016 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Saving an OpenEXR image
**
**	(C) Copyright 2011 - 2021  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/image/b3Tx.h"

#ifdef BLZ3_USE_OPENEXR

#include "b3TxSaveInfo.h"

#include <ImfOutputFile.h>
#include <ImfChannelList.h>
#include <ImfHeader.h>
#include <ImfIO.h>
#include <ImfFrameBuffer.h>

#ifdef HAVE_LIBOPENEXR_3_1
#	include <Imath/ImathVec.h>
#	include <Imath/half.h>
#else
#	include <OpenEXR/ImathVec.h>
#	include <OpenEXR/half.h>
#endif

using namespace Imf;
using namespace Imath;

/*************************************************************************
**                                                                      **
**                          EXR                                         **
**                                                                      **
*************************************************************************/

class b3InfoEXR : protected b3TxSaveInfo, protected OStream
{
public:
	b3InfoEXR(b3Tx * tx, const char * filename) :
		b3TxSaveInfo(tx, filename, B3_BWRITE),
		OStream(filename)
	{
		if (!tx->b3IsHdr())
		{
			B3_THROW(b3TxException, B3_TX_ILLEGAL_DATATYPE);
		}
		b3PrintF(B3LOG_FULL, "  b3InfoEXR initialized.\n");
	}

	virtual ~b3InfoEXR() = default;

	virtual void        write(const char c[/*n*/], int n) override
	{
		clearerr(m_FileHandle);

		if (fwrite(c, 1, n, m_FileHandle) != static_cast<size_t>(n))
		{
			B3_THROW(b3TxException, B3_TX_NOT_SAVED);
		}
	}

	virtual uint64_t    tellp() override
	{
		return ftell(m_FileHandle);
	}

	virtual void        seekp(uint64_t pos) override
	{
		clearerr(m_FileHandle);
		fseek(m_FileHandle, pos, SEEK_SET);
	}

	void  b3Write()
	{
		try
		{
			b3_color  *  data = m_Tx->b3GetHdrData();
			Header       header(m_Tx->xSize, m_Tx->ySize);

			header.compression() = PIZ_COMPRESSION;
			header.channels().insert("R", Channel(FLOAT));
			header.channels().insert("G", Channel(FLOAT));
			header.channels().insert("B", Channel(FLOAT));

			OutputFile   file(*this, header);
			FrameBuffer  fb;

#if 0
			std::for_each(data, data + m_Tx->xSize * m_Tx->ySize, [](b3_color & color)
			{
				color.r = pow(color.r, 2.2);
				color.g = pow(color.g, 2.2);
				color.b = pow(color.b, 2.2);
			});
#endif
			fb.insert("R", Slice(FLOAT, reinterpret_cast<char *>(&data->r), sizeof(b3_color), sizeof(b3_color) * m_Tx->xSize));
			fb.insert("G", Slice(FLOAT, reinterpret_cast<char *>(&data->g), sizeof(b3_color), sizeof(b3_color) * m_Tx->xSize));
			fb.insert("B", Slice(FLOAT, reinterpret_cast<char *>(&data->b), sizeof(b3_color), sizeof(b3_color) * m_Tx->xSize));

			file.setFrameBuffer(fb);
			file.writePixels(m_Tx->ySize);
		}
		catch (const std::exception & e)
		{
			b3PrintF(B3LOG_NORMAL, "Error writing OpenEXR file %s\n", e.what());
			B3_THROW(b3TxException, B3_TX_NOT_SAVED);
		}
	}
};

b3_result b3Tx::b3SaveEXR(const char * filename B3_UNUSED)
{
	b3PrintF(B3LOG_FULL, "Saving EXR: %s\n", filename);

	b3InfoEXR info(this, filename);
	info.b3Write();
	return B3_OK;
}

#endif
