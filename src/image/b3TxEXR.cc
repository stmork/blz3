/*
**
**	$Filename:	b3TxEXR.cc $
**	$Release:	Dortmund 2005 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Loading OpenEXR images
**
**	(C) Copyright 2005  Steffen A. Mork
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

#include "b3TxInclude.h"

#ifdef BLZ3_USE_OPENEXR
#include <ImfIO.h>
#include <ImfInputFile.h>
#include <ImfHeader.h>
#include <ImathBox.h>

using namespace Imath;
using namespace Imf;
using namespace Iex;

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.6  2006/05/24 16:07:55  sm
**	- Done some Un*x b3GetData() corrections.
**
**	Revision 1.5  2006/05/11 15:34:22  sm
**	- Added unit tests
**	- Corrected normal computation for ocean waves
**	- Optimized b3Complex
**	- Added new FFT
**	- Added own assertion include
**	
**	Revision 1.4  2006/03/05 21:22:34  sm
**	- Added precompiled support for faster comiling :-)
**	
**	Revision 1.3  2005/10/17 18:51:33  sm
**	- Compile fix.
**	
**	Revision 1.2  2005/10/16 09:43:41  sm
**	- Minor changes.
**	
**	Revision 1.1  2005/10/16 09:35:45  sm
**	- Added OpenEXR parsing file.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        OpenEXR - decoder                             **
**                                                                      **
*************************************************************************/

class b3ExrInputStream: public IStream
{
	b3_u08   *m_Buffer;
	b3_size   m_Size;
	b3_index  m_Index;

public:
	b3ExrInputStream(b3_u08 *buffer, b3_size size, const char *filename) : IStream(filename)
	{
		m_Buffer = buffer;
		m_Size   = size;
		m_Index  = 0;
	}

	virtual bool read (char c[/*n*/], int n)
	{
		b3_size max = m_Index + n;

		if (max > m_Size)
		{
			n = m_Size - m_Index;
		}
		memcpy(c, &m_Buffer[m_Index], n);
		m_Index += n;

		// Return EOF.
		return m_Index < static_cast<b3_index>(m_Size);
	}

	virtual Int64 tellg ()
	{
		return m_Index;
	}

	virtual void seekg (Int64 pos)
	{
		if (pos >= m_Size)
		{
			throw Iex::InputExc ("New file position greater than file size.");
		}
		m_Index = pos;
	}

	virtual void clear ()
	{
		// We know now error state since everything is already in memory.
	}
};

b3_result b3Tx::b3ParseOpenEXR(b3_u08 *buffer, b3_size size)
{
	b3_result result = B3_ERROR;

	b3PrintF(B3LOG_FULL,"IMG EXR  # b3ParseOpenEXR(%s)\n",
		(const char *)image_name);

	try
	{
		b3ExrInputStream input(buffer, size, image_name);
		InputFile        file(input);
		Box2i            dw = file.header().dataWindow();
		b3_res           width  = dw.max.x - dw.min.x + 1;
		b3_res           height = dw.max.y - dw.min.y + 1;

		if (b3AllocTx(width, height, 128))
		{
			b3_color    *ptr = b3GetHdrData();
			FrameBuffer  fb;

			fb.insert("R", Slice(FLOAT, (char *)&ptr->r, sizeof(b3_color), sizeof(b3_color) * width, 1, 1, 0.0));
			fb.insert("G", Slice(FLOAT, (char *)&ptr->g, sizeof(b3_color), sizeof(b3_color) * width, 1, 1, 0.0));
			fb.insert("B", Slice(FLOAT, (char *)&ptr->b, sizeof(b3_color), sizeof(b3_color) * width, 1, 1, 0.0));
			file.setFrameBuffer(fb);
			file.readPixels(dw.min.y, dw.max.y);

			result = B3_OK;
		}
	}
	catch(std::exception &exc)
	{
		b3FreeTx();
		b3PrintF(B3LOG_NORMAL,"IMG EXR  # Error reading file:\n");
		b3PrintF(B3LOG_NORMAL,"           Cause:\n%s", exc.what());
		B3_THROW(b3TxException,B3_TX_ERR_HEADER);
	}
	return result;
}
#endif
