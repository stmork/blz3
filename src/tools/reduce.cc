/*
**
**	$Filename:      reduce.cc $
**	$Release:       Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Simple reduce of an image
**
**	(C) Copyright 2004  Steffen A. Mork
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

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**  $Log$
**  Revision 1.2  2004/11/29 09:58:01  smork
**  - Changed exit states to correct defines.
**  - Added switch for disabling VBO in OpenGL renderer.
**  - Added switches for logging level in OpenGL renderer as in brt3.
**
**  Revision 1.1  2004/08/09 10:09:06  sm
**  - Added color palette reduction and its tool.
**
**
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3_pkd_color palette[8] =
{
	0xd8f0fa,
	0x000000,
	0xfa820a,
	0xaf1414,
	0x0f1e78,
	0x3096fa,
	0x084019,
	0x641e10
};

int main(int argc,char *argv[])
{
	for (int i = 1;i < argc;i++)
	{
		b3Tx src,dst;
		
		if (src.b3LoadImage(argv[i]) == B3_OK)
		{
			if (dst.b3AllocTx(src.xSize,src.ySize,3))
			{
				dst.b3SetPalette(palette,8);
				if (dst.b3TxReduce(&src))
				{
					b3Path path(argv[i]);
					
					path.b3RemoveExt();
					path.b3Append(".tga");
					dst.b3SaveTGA(path);
				}
			}
		}
	}

	return EXIT_SUCCESS;
}
