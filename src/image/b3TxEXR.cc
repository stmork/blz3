/*
**
**	$Filename:	b3TxEXR.cc $
**	$Release:	Dortmund 2005 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Loading OpenEXR images
**
**	(C) Copyright 2005 - 2021  Steffen A. Mork
**	    All Rights Reserved
**
**
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

#include <ImfIO.h>
#include <ImfInputFile.h>
#include <ImfHeader.h>
#include <ImfFrameBuffer.h>

#include <ImathBox.h>

#ifdef HAVE_LIBOPENEXR_3_1
#	include <Imath/ImathVec.h>
#	include <Imath/half.h>
#else
#	include <OpenEXR/ImathVec.h>
#	include <OpenEXR/half.h>
#endif

using namespace Imath;
using namespace Imf;
using namespace Iex;

/*************************************************************************
**                                                                      **
**                        OpenEXR - decoder                             **
**                                                                      **
*************************************************************************/

class b3ExrInputStream : public IStream
{
	const b3_u08  * m_Buffer = nullptr;
	uint64_t        m_Size   = 0;
	uint64_t        m_Index  = 0;

public:
	b3ExrInputStream(const b3_u08 * buffer, b3_size size, const char * filename) :
		IStream(filename),
		m_Buffer(buffer),
		m_Size(size)
	{
	}

	virtual bool read(char c[/*n*/], int n) override
	{
		b3_size max = m_Index + n;

		if (max > m_Size)
		{
			n = m_Size - m_Index;
		}
		memcpy(c, &m_Buffer[m_Index], n);
		m_Index += n;

		// Return EOF.
		return m_Index < m_Size;
	}

	virtual uint64_t tellg() override
	{
		return m_Index;
	}

	virtual void seekg(uint64_t pos) override
	{
		if (pos >= m_Size)
		{
			throw Iex::InputExc("New file position greater than file size.");
		}
		m_Index = pos;
	}

	virtual void clear() override
	{
		// We know now error state since everything is already in memory.
	}
};

b3_result b3Tx::b3ParseOpenEXR(const b3_u08 * buffer, b3_size size)
{
	b3_result result = B3_ERROR;

	b3PrintF(B3LOG_FULL, "IMG EXR  # b3ParseOpenEXR(%s)\n",
		static_cast<const char *>(image_name));

	try
	{
		b3ExrInputStream input(buffer, size, image_name);
		InputFile        file(input);
		Box2i            dw = file.header().dataWindow();
		b3_res           width  = dw.max.x - dw.min.x + 1;
		b3_res           height = dw.max.y - dw.min.y + 1;

		if (b3AllocTx(width, height, 128))
		{
			b3_color  *  ptr = b3GetHdrData();
			FrameBuffer  fb;

			fb.insert("R", Slice(FLOAT, reinterpret_cast<char *>(&ptr->r), sizeof(b3_color), sizeof(b3_color) * width, 1, 1, 0.0));
			fb.insert("G", Slice(FLOAT, reinterpret_cast<char *>(&ptr->g), sizeof(b3_color), sizeof(b3_color) * width, 1, 1, 0.0));
			fb.insert("B", Slice(FLOAT, reinterpret_cast<char *>(&ptr->b), sizeof(b3_color), sizeof(b3_color) * width, 1, 1, 0.0));
			file.setFrameBuffer(fb);
			file.readPixels(dw.min.y, dw.max.y);

			result = B3_OK;
		}
	}
	catch (std::exception & exc)
	{
		b3FreeTx();
		b3PrintF(B3LOG_NORMAL, "IMG EXR  # Error reading file:\n");
		b3PrintF(B3LOG_NORMAL, "           Cause:\n%s", exc.what());
		B3_THROW(b3TxException, B3_TX_ERR_HEADER);
	}
	return result;
}
#endif
