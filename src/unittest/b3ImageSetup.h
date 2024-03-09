/*
**
**	$Filename:	b3ImageSetup.h $
**	$Release:	Dortmund 2024 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Unit tests for file access classes.
**
**      (C) Copyright 2024  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#ifndef B3_IMAGE_SETUP_H
#define B3_IMAGE_SETUP_H

#include "blz3/image/b3Tx.h"

class b3ImageSetup
{
protected:
	static const b3_res       TEST_IMG_XMAX = 400;
	static const b3_res       TEST_IMG_YMAX = 300;

	b3Tx m_TxGIF;
	b3Tx m_TxBW;
	b3Tx m_TxPallColor;
	b3Tx m_TxHighColor;
	b3Tx m_TxTrueColor;
	b3Tx m_TxRealColor;

	void b3SetupImages();
};

#endif
