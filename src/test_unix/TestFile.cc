#include "blz3/b3Config.h"
#include "blz3/system/b3Log.h"
#include "blz3/system/b3Dir.h"
#include "blz3/system/b3File.h"

void b3TestFile()
{
	b3File       file;
	b3_path_type code;
	char         array[1024];

	if (file.b3Open("Config.tst",B_WRITE))
	{
		b3PrintF (B3LOG_NORMAL,"File 'Config.tst' opened...\n");
		file.b3Buffer (2048);
		file.b3Write  (array,1024);
		file.b3Flush  ();
		file.b3Write  (array,1024);
		file.b3Write  (array,1024);
		file.b3Write  (array,1024);
		file.b3Buffer (512);
		file.b3Write  (array,1024);
		b3PrintF (B3LOG_NORMAL,"File Size: %ld (should be 5120 Bytes)\n",file.b3Size());
		file.b3Close  ();

		if (file.b3Open("Config.tst",B_READ))
		{
			b3PrintF (B3LOG_NORMAL,"File 'Config.tst' opened...\n");
			if (file.b3Read(array,128) < 128) b3PrintF (B3LOG_NORMAL,"128 not read...\n");
			b3PrintF (B3LOG_NORMAL,"Seek1: %4ld (should be  128)\n",file.b3Seek ( 512,B3_SEEK_START));
			b3PrintF (B3LOG_NORMAL,"Seek2: %4ld (should be  512)\n",file.b3Seek ( 512,B3_SEEK_CURRENT));
			b3PrintF (B3LOG_NORMAL,"Seek3: %4ld (should be 1024)\n",file.b3Seek (-120,B3_SEEK_END));
			b3PrintF (B3LOG_NORMAL,"Seek4: %4ld (should be 5000)\n",file.b3Seek (   0,B3_SEEK_CURRENT));
			file.b3Close ();
		}
		else b3PrintF (B3LOG_NORMAL,"File 'Config.tst' not opened for reading...\n");

		code = b3Dir::b3Exists ("Config.tst");
		switch (code)
		{
			case B3_NOT_EXISTANT :
				b3PrintF (B3LOG_NORMAL,"Config.tst is not existant...\n");
				break;
			case B3_TYPE_DIR :
				b3PrintF (B3LOG_NORMAL,"Config.tst is a directory...\n");
				break;
			case B3_TYPE_FILE :
				b3PrintF (B3LOG_NORMAL,"Config.tst is a file...\n");
				break;

			default :
				b3PrintF (B3LOG_NORMAL,"Config.tst is if unknown file type (code %ld)\n",code);
				break;
		}

		if (remove ("Config.tst") == 0) b3PrintF (B3LOG_NORMAL,"File 'Config.tst' removed...\n");
		else b3PrintF (B3LOG_NORMAL,"File 'Config.tst' not removed...\n");

		code = b3Dir::b3Exists ("Config.tst");
		switch (code)
		{
			case B3_NOT_EXISTANT :
				b3PrintF (B3LOG_NORMAL,"Config.tst is not existant... (all right)\n");
				break;
			case B3_TYPE_DIR :
				b3PrintF (B3LOG_NORMAL,"Config.tst is a directory...\n");
				break;
			case B3_TYPE_FILE :
				b3PrintF (B3LOG_NORMAL,"Config.tst is a file...\n");
				break;

			default :
				b3PrintF (B3LOG_NORMAL,"Config.tst is if unknown file type (code %ld)\n",code);
				break;
		}
	}
	else b3PrintF (B3LOG_NORMAL,"File 'Config.tst' not opened for writing...\n");
}
