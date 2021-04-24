#include "b3ImageSetup.h"

void b3ImageSetup::b3SetupImages()
{
	b3Color        row[TEST_IMG_XMAX];
	b3_u08    *    bRow;
	b3_u08    *    pRow;
	b3_u16    *    hRow;
	b3_pkd_color * tRow;
	b3_color   *   rRow;
	b3_res         width   = TEST_IMG_XMAX >> 3;

	m_TxBW.b3AllocTx(TEST_IMG_XMAX, TEST_IMG_YMAX,   1); // black/white
	m_TxPallColor.b3AllocTx(TEST_IMG_XMAX, TEST_IMG_YMAX,   8); // 8 bit palette entry
	m_TxHighColor.b3AllocTx(TEST_IMG_XMAX, TEST_IMG_YMAX,  16); // High color, 4 bit per color
	m_TxTrueColor.b3AllocTx(TEST_IMG_XMAX, TEST_IMG_YMAX,  32); // True color, 8 bit per color
	m_TxRealColor.b3AllocTx(TEST_IMG_XMAX, TEST_IMG_YMAX, 128); // Real color, 32 bit per color (floating point)

	for (b3_res x = 0; x < TEST_IMG_XMAX; x++)
	{
		b3_f32   r, g, b, value;
		b3_index idx = x / width;

		value = (b3_f32)(x % width) / width;
		b = idx & 1 ? value : 0.0;
		r = idx & 2 ? value : 0.0;
		g = idx & 4 ? value : 0.0;
		row[x].b3Init(r, g, b);
	}

	b3_pkd_color * palette = m_TxPallColor.b3GetPalette();
	for (b3_index i = 0; i < (1 << m_TxPallColor.depth); i++)
	{
		*palette++ = b3Tx::b3IndexToColor(i);
	}
	bRow = m_TxBW.b3GetIndexData();
	pRow = m_TxPallColor.b3GetIndexData();
	hRow = m_TxHighColor.b3GetHighColorData();
	tRow = m_TxTrueColor.b3GetTrueColorData();
	rRow = m_TxRealColor.b3GetHdrData();

	for (b3_res y = 0; y < TEST_IMG_YMAX; y++)
	{
		for (b3_res x = 0; x < TEST_IMG_XMAX; x++)
		{
			b3_pkd_color true_color = row[x];
			b3_u16       high_color = 0;

			for (b3_index i = 0; i < 4; i++)
			{
				high_color = (high_color << 4) | b3_u16(row[x][i] * 15);
			}

			*pRow++ = b3Tx::b3ColorToIndex(true_color);
			*hRow++ = high_color;
			*tRow++ = true_color;
			*rRow++ = row[x];
		}
		for (b3_res x = 0; x < TEST_IMG_XMAX; x += 16)
		{
			*bRow++ = (y & 2) ? 0xcc : 0x33;
			*bRow++ = (y & 2) ? 0x00 : 0xff;
		}
	}
}
