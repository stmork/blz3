/*
**
**	$Filename:	TestFile.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Testing file routines
**
**      (C) Copyright 2001  Steffen A. Mork
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
  
#include "blz3/b3Config.h"
#include "blz3/system/b3Log.h"
#include "blz3/system/b3Dir.h"
#include "blz3/system/b3FileAbstract.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.5  2002/08/11 06:38:54  sm
**	- Started some library reorganizations: Moved folowing classes into
**	  system lib. Introduced new system library which is platform
**	  independend but part of platform dependend library.
**	  o b3FileAbstract
**	  o b3DirAbstract
**	  o b3Exception
**	- The following classes should be reorganized with abstract
**	  base classes to:
**	  o b3Date
**	  o b3Time
**	  o b3Log
**
**	Revision 1.4  2002/01/03 15:50:15  sm
**	- Added cut/copy/paste
**	
**	Revision 1.3  2001/12/30 14:16:58  sm
**	- Abstracted b3File to b3FileAbstract to implement b3FileMem (not done yet).
**	- b3Item writing implemented and updated all raytracing classes
**	  to work properly.
**	- Cleaned up spline shapes and CSG shapes.
**	- Added b3Caustic class for compatibility reasons.
**	
**	Revision 1.2  2001/09/30 15:46:07  sm
**	- Displaying raytracing under Windows
**	- Major cleanups in Lines III with introducing CAppRaytraceDoc/
**	  CAppRaytraceView pair for displaying Raytracing
**	
**
*/

/*************************************************************************
**                                                                      **
**                        b3TestFile implementation                     **
**                                                                      **
*************************************************************************/

void b3TestFile(b3FileAbstract &file)
{
	b3_path_type code;
	char         array[1024];

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
}
