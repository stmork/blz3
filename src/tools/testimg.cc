/*
**
**      $Filename:      testimg.cc $
**      $Release:       Dortmund 2003 $
**      $Revision$
**      $Date$
**      $Author$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Creating a test image
**
**      (C) Copyright 2003  Steffen A. Mork
**          All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/b3Config.h"
#include "blz3/base/b3Array.h"
#include "blz3/base/b3Color.h"
#include "blz3/image/b3Tx.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.4  2004/11/29 09:58:01  smork
**	- Changed exit states to correct defines.
**	- Added switch for disabling VBO in OpenGL renderer.
**	- Added switches for logging level in OpenGL renderer as in brt3.
**
**	Revision 1.3  2004/07/08 10:43:00  sm
**	- Make some makefile cleanups.
**	- Removed some compiler warnings.
**	
**	Revision 1.2  2003/08/11 18:49:34  sm
**	- Some minor pattern changes
**	
**	Revision 1.1  2003/08/08 08:50:07  sm
**	- Added test image creation
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3_pkd_color ebu[] = 
{
	0x010101,0x010100,0x000101,0x000100,0x010001,0x010000,0x000001,0x000000
};

b3_pkd_color freqs [] =
{
	3,5,10,15,20,25
};

static b3_f64 freq(b3_f64 freq)
{
	return M_PI * 2.0 / freq;
}

static void create(b3Tx &img)
{
	b3_coord              x,xOct = img.xSize >> 3;
	b3_coord              y,yQuart = img.ySize >> 2,c,max;
	b3Array<b3_pkd_color> bars,sinus;

	b3_pkd_color *lPtr = (b3_pkd_color *)img.b3GetData();
	b3_pkd_color *data = lPtr;
	b3_pkd_color value,color;

	for (c = 0;c < 8;c++)
	{
		for (x = 0;x < xOct;x++)
		{
			value = x * 255 / xOct;
			color = ebu[c] * value;
			bars.b3Add(color);
		}
	}

	lPtr = data;
	for (y = 0;y < yQuart;y++)
	{
		for (x = 0;x < bars.b3GetCount();x++)
		{
			lPtr[x] = bars[x];
		}
		lPtr += img.xSize;
	}

	max = img.ySize - yQuart;
#if 0
	for (y = 0;y < max;y++)
	{
		b3_f64 f = freq(2 + (y + 1) * 48.0 / max);

		for (x = 0;x < img.xSize;x++)
		{
			lPtr[x] = 0x010101 * (b3_pkd_color)(127.5 * sin((b3_f64)x * f) + 127.5);
		}
		lPtr += img.xSize;
	}
#else
	for (c = 0;c < 50;c+=2)
	{
		b3_f64 f = freq(c + 1);
		for (y = c * max / 25;y < ((c + 1) * max / 25);y++)
		{
			for (x = 0;x < img.xSize;x++)
			{
				lPtr[x] = 0x010101 * (b3_pkd_color)(127.5 * sin((b3_f64)x * f) + 127.5);
			}
			lPtr += img.xSize;
		}
	}
#endif
}

int main(int argc,char *argv[])
{
	b3Tx img;
	b3_res xSize = 2048,ySize = xSize * 4 / 3;
	char *imgname = null;

	for (int i = 1;i < argc;i++)
	{
		switch(i)
		{
		case 1:
			sscanf(argv[i],"%ld",&xSize);
			break;
		case 2:
			sscanf(argv[i],"%ld",&ySize);
			break;
		case 3:
			imgname = argv[i];
			break;
		}
	}

	img.b3AllocTx(xSize,ySize,24);
	create(img);
	if (imgname != null)
	{
		img.b3SaveJPEG(imgname);
	}

	return EXIT_SUCCESS;
}
