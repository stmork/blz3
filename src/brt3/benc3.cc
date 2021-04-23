/*
**
**	$Filename:	benc3.cc $
**	$Release:	Dortmund 2001 - 2021 $
**
**	Blizzard III - The video stream builder
**
**      (C) Copyright 2001 - 2021  Steffen A. Mork
**          All Rights Reserved
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
#include "blz3/image/b3Encoder.h"
#include "blz3/base/b3FileList.h"

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

static void b3Banner(const char * command)
{
	b3PrintF(B3LOG_NORMAL, "Blizzard III movie maker\n");
	b3PrintF(B3LOG_NORMAL, "Copyright (C) Steffen A. Mork  2001-2021\n");
	b3PrintF(B3LOG_NORMAL, "\n");

	if (command != nullptr)
	{
#ifdef HAVE_VIDEO_ENCODER
		b3PrintF(B3LOG_NORMAL, "USAGE:\n");
		b3PrintF(B3LOG_NORMAL, "%s Video-File [-f][-d] {frame images}\n", command);
		b3PrintF(B3LOG_NORMAL, "\n");
		b3PrintF(B3LOG_NORMAL, "  -d        debug level output\n");
		b3PrintF(B3LOG_NORMAL, "  -f        verbose level output\n");
		b3PrintF(B3LOG_NORMAL, "\n");
#else
		b3PrintF(B3LOG_NORMAL, "%s has no video encoding support!\n", command);
#endif
	}
	b3PrintF(B3LOG_NORMAL, "Compile date: %s %s\n", __DATE__, __TIME__);
	b3PrintF(B3LOG_NORMAL, "%s\n", b3Runtime::b3GetCompiler());
}

int main(int argc, char * argv[])
{
	const char   *  filename = "test-movie.mp4";
	b3FileList      list;
	b3FileEntry  *  entry;

	b3Log::b3SetLevel(B3LOG_NORMAL);
	if (argc <= 1)
	{
		b3Banner(argv[0]);
		exit(EXIT_SUCCESS);
	}

	if (argc >= 2)
	{
		filename = argv[1];
	}

	b3CodecRegister::b3Instance().b3PrepareCodecs();

	for (int i = 2; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			switch (argv[i][1])
			{
			case 'd' :
				b3Log::b3SetLevel(B3LOG_DEBUG);
				break;

			case 'f' :
				b3Log::b3SetLevel(B3LOG_FULL);
				break;
			}
		}
		else
		{
			switch (b3Dir::b3Exists(argv[i]))
			{
			case B3_TYPE_DIR:
				list.b3RecCreateList(argv[i]);
				break;

			case B3_TYPE_FILE:
				list.b3Add(argv[i]);
				break;

			case B3_NOT_EXISTANT:
				break;

			default:
				break;
			}
		}
	}

	if (list.b3GetCount() > 0)
	{
		b3Tx            tx;

		list.b3Sort();
		tx.b3LoadImage(list.b3First()->b3Name());
		b3MovieEncoder encoder(filename, &tx, 25);

		for (entry = list.b3First(); entry != nullptr; entry = entry->Succ)
		{
			try
			{
				tx.b3LoadImage(entry->b3Name());
				encoder.b3AddFrame(&tx);
			}
			catch (b3TxException & t)
			{
				b3PrintF(B3LOG_NORMAL, "\n");
				b3PrintF(B3LOG_NORMAL, "Image error when processing image %s!\n", entry->b3Name());
				b3PrintF(B3LOG_NORMAL, "Error code: %d\n", t.b3GetError());
				b3PrintF(B3LOG_NORMAL, "Error msg:  %s\n", t.b3GetErrorMsg());
			}
			catch (b3ExceptionBase & e)
			{
				b3PrintF(B3LOG_NORMAL, "\n");
				b3PrintF(B3LOG_NORMAL, "General Blizzard III error on image %s!\n", entry->b3Name());
				b3PrintF(B3LOG_NORMAL, "Error code: %d\n", e.b3GetError());
				b3PrintF(B3LOG_NORMAL, "Error msg:  %s\n", e.b3GetErrorMsg());
			}
			catch (...)
			{
				b3PrintF(B3LOG_NORMAL, "\n");
				b3PrintF(B3LOG_NORMAL, "Unknown error occured on image %s!\n", entry->b3Name());
			}
		}
	}

	return EXIT_SUCCESS;
}
