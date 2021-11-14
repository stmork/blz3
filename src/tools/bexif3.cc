/*
**
**	$Filename:	bexif3.cc $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - EXIF image preparation
**
**	(C) Copyright 2021  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/system/b3Date.h"
#include "blz3/system/b3Dir.h"
#include "blz3/system/b3File.h"
#include "blz3/system/b3Runtime.h"
#include "blz3/base/b3Array.h"
#include "blz3/base/b3FileList.h"
#include "blz3/image/b3Tx.h"
#include "blz3/image/b3TxExif.h"

#include <fcntl.h>
#include <utime.h>
#include <sys/stat.h>
#include <sys/time.h>

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBEXIV2
static void b3Banner(const char * command)
{
	const b3Date now;

	b3PrintF(B3LOG_NORMAL, "%s EXIF image manipulator\n", b3Runtime::b3GetProduct());
	b3PrintF(B3LOG_NORMAL, "Copyright (C) Steffen A. Mork  2001-%d\n", now.year);
	b3PrintF(B3LOG_NORMAL, "\n");

	if (command != nullptr)
	{
		b3PrintF(B3LOG_NORMAL, "USAGE:\n");
		b3PrintF(B3LOG_NORMAL, "%s [-g][-G][-p][-k][-x] {images}\n", command);
		b3PrintF(B3LOG_NORMAL, "\n");
		b3PrintF(B3LOG_NORMAL, "  -g        leave geo tagging.\n");
		b3PrintF(B3LOG_NORMAL, "  -G        remove geo tagging.\n");
		b3PrintF(B3LOG_NORMAL, "  -p        update product info.\n");
		b3PrintF(B3LOG_NORMAL, "  -k        keep filesystem date.\n");
		b3PrintF(B3LOG_NORMAL, "  -r <DPI>  set DPI.\n");
		b3PrintF(B3LOG_NORMAL, "  -x        update filesystem date from EXIF.\n");
		b3PrintF(B3LOG_NORMAL, "\n");
	}
	b3PrintF(B3LOG_NORMAL, "Compile date: %s %s\n", __DATE__, __TIME__);
	b3PrintF(B3LOG_NORMAL, "%s\n", b3Runtime::b3GetCompiler());
}
#endif

int main(int argc, char * argv[])
{
#ifdef HAVE_LIBEXIV2
	bool     remove_geo         = false;
	bool     update_productinfo = false;
	bool     keep_timestamp     = false;
	bool     extract_timestamp  = false;
	unsigned dpi                = 0;

	if (argc <= 1)
	{
		b3Banner(argv[0]);
		exit(EXIT_SUCCESS);
	}

	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-g") == 0)
		{
			remove_geo = false;
		}
		else if (strcmp(argv[i], "-G") == 0)
		{
			remove_geo = true;
		}
		else if (strcmp(argv[i], "-p") == 0)
		{
			update_productinfo = true;
		}
		else if (strcmp(argv[i], "-k") == 0)
		{
			keep_timestamp = true;
		}
		else if (strcmp(argv[i], "-x") == 0)
		{
			extract_timestamp = true;
		}
		else if (strcmp(argv[i], "-r") == 0)
		{
			const char * number;

			if (strlen(argv[i]) > 2)
			{
				number = argv[i] + 2;
			}
			else
			{
				i++;
				number = i < argc ? argv[i] : nullptr;
			}

			if (number != nullptr)
			{
				const int result = atoi(number);

				if (result > 0)
				{
					dpi = result;
				}
			}
		}
		else
		{
			try
			{
				struct stat file_attributes;
				b3TxExif    exif(argv[i]);
				int         success = stat(argv[i], &file_attributes);
				b3_count    count   = 0;

				if (remove_geo)
				{
					exif.b3RemoveGpsData();
					count++;
				}
				if (update_productinfo)
				{
					exif.b3Update();
					count++;
				}
				if (extract_timestamp)
				{
					b3Date date = exif;

					file_attributes.st_ctim = date;
					file_attributes.st_mtim = date;
				}

				if (dpi > 0)
				{
					exif.b3SetResolution(dpi, dpi);
					count++;
				}

				if (count > 0)
				{
					exif.b3Write(argv[i]);
				}

				if ((keep_timestamp || extract_timestamp) && (success == 0))
				{
					struct timeval times[2];

					times[0].tv_sec  = file_attributes.st_atim.tv_sec;
					times[0].tv_usec = file_attributes.st_atim.tv_nsec / 1000;
					times[1].tv_sec  = file_attributes.st_mtim.tv_sec;
					times[1].tv_usec = file_attributes.st_mtim.tv_nsec / 1000;

					utimes(argv[i], times);
				}
			}
			catch (b3TxException & t)
			{
				b3PrintF(B3LOG_NORMAL, "Error code: %d\n", t.b3GetError());
				b3PrintF(B3LOG_NORMAL, "Error msg:  %s\n", t.b3GetErrorMsg());
			}
			catch (std::exception & e)
			{
				b3PrintF(B3LOG_NORMAL, "Error msg:  %s\n", e.what());
			}
		}
	}
#else
	(void)argc;
	(void)argv;

	b3PrintF(B3LOG_NORMAL, "Missing EXIF support!\n");
#endif
	return EXIT_SUCCESS;
}
