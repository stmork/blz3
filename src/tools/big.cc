/*
**
**	$Filename:	big.cc $
**	$Release:	Dortmund 2007 $
**	$Revision: 1500 $
**	$Date: 2007-06-17 17:40:56 +0200 (Sun, 17 Jun 2007) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Create a very big image
**
**	(C) Copyright 2007  Steffen A. Mork
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

int main(int argc, char * argv[])
{
	b3_size size = atoi(argv[1]);
	for (int i = 2; i < argc; i++)
	{
		b3Tx tx;

		tx.b3AllocTx(size, size, 24);
		tx.b3SaveImage(argv[i]);
	}
	exit(EXIT_SUCCESS);
}
