#include "b3ImageSetup.h"

void b3ImageSetup::b3SetupImages()
{
	m_TxBW.b3AllocTx(TEST_IMG_XMAX, TEST_IMG_YMAX,   1); // black/white
	m_TxPallColor.b3AllocTx(TEST_IMG_XMAX, TEST_IMG_YMAX,   8); // 8 bit palette entry
	m_TxHighColor.b3AllocTx(TEST_IMG_XMAX, TEST_IMG_YMAX,  16); // High color, 4 bit per color
	m_TxTrueColor.b3AllocTx(TEST_IMG_XMAX, TEST_IMG_YMAX,  32); // True color, 8 bit per color
	m_TxRealColor.b3AllocTx(TEST_IMG_XMAX, TEST_IMG_YMAX, 128); // Real color, 32 bit per color (floating point)

	m_TxBW.b3TestPattern();
	m_TxPallColor.b3TestPattern();
	m_TxHighColor.b3TestPattern();
	m_TxTrueColor.b3TestPattern();
	m_TxRealColor.b3TestPattern();
}
