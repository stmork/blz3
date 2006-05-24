/*
**
**      $Filename:      calibrate.cc $
**      $Release:       Dortmund 2003 $
**      $Revision$
**      $Date$
**      $Author$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Calibrating scanned photos.
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
#include "blz3/system/b3Dir.h"
#include "blz3/system/b3File.h"
#include "blz3/base/b3Array.h"
#include "blz3/base/b3Color.h"
#include "blz3/base/b3FileList.h"
#include "blz3/image/b3Tx.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.7  2006/05/24 16:07:55  sm
**	- Done some Un*x b3GetData() corrections.
**
**	Revision 1.6  2005/10/15 16:43:03  sm
**	- Added HDR texture access.
**	
**	Revision 1.5  2004/11/29 09:58:01  smork
**	- Changed exit states to correct defines.
**	- Added switch for disabling VBO in OpenGL renderer.
**	- Added switches for logging level in OpenGL renderer as in brt3.
**	
**	Revision 1.4  2004/07/08 10:43:00  sm
**	- Make some makefile cleanups.
**	- Removed some compiler warnings.
**	
**	Revision 1.3  2003/08/27 14:54:23  sm
**	- sprintf changed into snprintf to avoid buffer overflows.
**	
**	Revision 1.2  2003/05/18 14:59:01  sm
**	- Fixed predefined constat MAX
**	- Fixed typo
**	
**	Revision 1.1  2003/05/17 21:09:48  sm
**	- Added feeder scanning calibration
**	
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

class b3Calibrate
{
	b3Array<b3Color> m_RowAvrg;
	b3Array<b3Color> m_WhiteScale;
	b3Array<b3_bool> m_Error;

public:
	b3Calibrate(b3Tx &source)
	{
		b3Color avrg;
		b3Color scale;
		b3Color diff;
		b3Color white(B3_WHITE);
		b3_coord x,y;
		b3_f64   fxSize = source.xSize;
		b3_f64   size   = source.ySize * fxSize;
		b3_f64   grey;
		b3_f64   dist;
		b3_count error[source.xSize];
		b3_bool  result = false;

		for (x = 0;x < source.xSize;x++)
		{
			error[x] = 0;
		}

		avrg.b3Init();
		for (y = 20;y < source.ySize;y++)
		{
			for (x = 0;x < source.xSize;x++)
			{
				avrg += source.b3GetHdrValue(x,y);
			}
		}
		avrg /= size;
		grey  = 0.35 * avrg[b3Color::R] + 0.54 * avrg[b3Color::G] + 0.11 * avrg[b3Color::B];

		white.b3Init(grey,grey,grey);
		for (y = 0;y < source.ySize;y++)
		{
			avrg.b3Init();
			for (x = 0;x < source.xSize;x++)
			{
				avrg += source.b3GetHdrValue(x,y);
			}

			avrg /= fxSize;
			m_RowAvrg.b3Add(avrg);

			scale.b3Init(
				avrg[b3Color::R] / white[b3Color::R],
				avrg[b3Color::G] / white[b3Color::G],
				avrg[b3Color::B] / white[b3Color::B]);
			m_WhiteScale.b3Add(scale);

#ifdef _DEBUG
			b3PrintF(B3LOG_NORMAL,"%3d: %2.3f %2.3f %2.3f # %2.3f %2.3f %2.3f\n",
				y,
				scale[b3Color::R],scale[b3Color::G],scale[b3Color::B],
				avrg[b3Color::R], avrg[b3Color::G], avrg[b3Color::B]);
#endif

			for (x = 0;x < source.xSize;x++)
			{
				diff = source.b3GetHdrValue(x,y) - avrg;
				dist = sqrt(
					diff[b3Color::R] * diff[b3Color::R] +
					diff[b3Color::B] * diff[b3Color::B] +
					diff[b3Color::G] * diff[b3Color::G]);
				if (dist > 0.145)
				{
					error[x]++;
				}
			}
		}

		for (x = 0;x < source.xSize;x++)
		{
			m_Error.b3Add(result);
			b3PrintF(B3LOG_NORMAL,"%s",result ? "#" : ".");
		}
		m_Error[ 58] =
		m_Error[ 75] =
		m_Error[200] =
		m_Error[202] =
		m_Error[448] =
		m_Error[449] =
		m_Error[635] =
		m_Error[771] =
		m_Error[793] =
		m_Error[794] =
		m_Error[798] =
		m_Error[799] =
		m_Error[805] = true;
		b3PrintF(B3LOG_NORMAL,"\n");
	}

	void b3Adjust(b3Tx &image)
	{
		b3PrintF(B3LOG_NORMAL,"%s\n",image.b3Name());

		b3_pkd_color *ptr = image.b3GetTrueColorData();
		b3_coord      x,y;
		b3Color       result,scale(B3_WHITE),offset(B3_BLACK);

		for (y = 0;y < image.ySize;y++)
		{
			offset = m_RowAvrg[y];
			scale  = m_WhiteScale[y];
			for (x = 0;x < image.xSize;x++)
			{
				b3Color pixel(ptr[x]);

				result = ((pixel - offset) * scale) + offset;
				ptr[x] = result;

#if 0
				b3PrintF(B3LOG_NORMAL,"%2.3f %2.3f %2.3f\n",
					result[b3Color::R], result[b3Color::G], result[b3Color::B]);
#endif
			}
			for (x = 1;x < (image.xSize - 1);x++)
			{
				if (m_Error[x])
				{
					ptr[x] = b3Color(ptr[x-1]) * 0.5 + b3Color(ptr[x+1]) * 0.5;
				}
			}
			ptr += image.xSize;
		}
	}
};


int main(int argc,char *argv[])
{
	b3Tx         reference,image;
	b3Calibrate *calibrate;
	b3Path       source = "/tmp/maui";
	b3Path       dest   = "/usr/local/maui/postproc2/data";
	b3Path       file;
	char         name[32];
	b3FileList   list;
	b3FileEntry *entry;
	b3_count     count = 0;

	reference.b3LoadImage("/home/sm/Kalibrierung.tif");
	calibrate = new b3Calibrate(reference);

	list.b3CreateList(source);
	list.b3Sort();

	for (entry = list.b3First();entry != null;entry = entry->Succ)
	{
		image.b3LoadImage(entry->b3Name());
		calibrate->b3Adjust(image);
		snprintf(name,sizeof(name),"%08x.tif",(unsigned)++count);
		file.b3LinkFileName(dest,name);
		image.b3SaveTIFF(file);
		b3PrintF(B3LOG_NORMAL,"%s\n",(const char *)file);
	}
	return EXIT_SUCCESS;
}
