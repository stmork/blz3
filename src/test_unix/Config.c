/*
**
**	$Filename:	Config.c  $
**	$Release:	Dortmund 1994, 1995, 1996, 1997 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork
**
**	Check Configuration of BLIZZARD
**
**	(C) Copyright 1993, 1994, 1995, 1996, 1997 Blizzard Development Group
**	    All Rights Reserved
**
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        includes                                      **
**                                                                      **
*************************************************************************/

#include <blizzard/options.h>
#include <blizzard/sys/config.h>
#include <blizzard/sys/tdisplay.h>
#include <blizzard/sys/filedlg.h>
#include <blizzard/sys/bio.h>
#include <blizzard/proto/save.h>

/*************************************************************************
**                                                                      **
**                        variables                                     **
**                                                                      **
*************************************************************************/

struct MemNode		 Global;
char				*CharPtr;
char				 User[OPTSTRINGLEN] = "<undefined>";
long				 LongWord;
long				 SAVETYPE    = -1;
float				 AspectRatio =  0.75;

struct OptList ConfigOptions[] =
{
	{ OPT_TYPE_LONG , "SaveType",    (void *)&SAVETYPE },
	{ OPT_TYPE_FLOAT, "AspectRatio", (void *)&AspectRatio },
	{ OPT_TYPE_STRING,"BlizzardUser",(void *)User },
	{ OPT_TYPE_END,   null,          (void *)null }
};

/*************************************************************************
**                                                                      **
**                        main routines                                 **
**                                                                      **
*************************************************************************/

int main(argc,argv)

long	 argc;
char	*argv[];

{
	struct ColorDisplayInfo	*ColorDInfo,*NilDInfo;
	struct BlizzardInquiry	*ConfigInquiry;
	struct BOptions			*Opts;
	struct BFile			*File;
	struct BDir             *Dir;
	unsigned char 			*Pointer1,*Pointer2;
	char					*Name,Array[1024];
	char                     name[FILESTRINGLEN];
	long					 i,k,code;

	if (!OpenRequirements()) exit (10);

	PrintF ("Blizzard Config-Test:\n");
	BEEP;
	TIME_START;

	ConfigInquiry = Inquiry (__FILE__);
	PrintF ("\nInquiry:\n");
	PrintF ("Machine: %s\n",ConfigInquiry->Machine);
	PrintF ("Vendor:  %s\n",ConfigInquiry->Vendor);
	PrintF ("Product: %s\n",ConfigInquiry->Product);
	PrintF ("Date:    %s\n",ConfigInquiry->Date);
	PrintF ("SerNum:  %s\n",ConfigInquiry->SerNum);

	PrintF ("\nprinting paths of 'Options.c'...\n");
	if (Opts = BeginPrefs(null))
	{
		if (GetPrefLong (Opts,"SaveType",&SAVETYPE)) switch (SAVETYPE)
		{
			case SAVE_RGB8 :
				PrintF ("SaveType         = RGB8 (24-Bit)\n");
				break;

			case SAVE_MTV :
				PrintF ("SaveType         = MTV\n");
				break;

			case SAVE_PPM :
				PrintF ("SaveType         = PPM\n");
				break;

			case SAVE_TGA24 :
				PrintF ("SaveType         = Targa 24 Bit\n");
				break;

			case SAVE_TIFF :
				PrintF ("SaveType         = TIFF\n");
				break;

			case SAVE_PS :
				PrintF ("SaveType         = PS\n");
				break;

			case SAVE_JPEG :
				PrintF ("SaveType         = JPEG\n");
				break;

			default :
				PrintF ("SaveType         = <no saving>\n");
				break;
		}
		else	PrintF ("SaveType         = <undefined>\n");
		if (GetPrefFloat  (Opts,"AspectRatio",&AspectRatio))
				PrintF ("AspectRatio      = %2.5f\n",AspectRatio);
		else	PrintF ("AspectRatio      = <undefined>\n");
		if (GetPrefString (Opts,"BlizzardUser",User))
			PrintF ("Blizzard User    = %s\n",User);
		EndPrefs (Opts);
	}
	else
	{
		PrintF ("\n");
		PrintF ("WARNING:\n");
		PrintF ("The following messages will apear only once!\n\n");
		PrintF ("Creating default settings...\n");
		GetPrefList (null,ConfigOptions);
		PrintF ("After running >>%s<< please edit file %s\n\n",
			argv[0],PreferencesFile);
	}

	PrintF ("TextureFilePath  = %s\n",TextureFilePath);
	PrintF ("ReadyPicFilePath = %s\n",ReadyPicFilePath);
	PrintF ("ScriptFilePath   = %s\n",ScriptFilePath);
	PrintF ("FilePath         = %s\n",FilePath);
	PrintF ("FileName         = %s\n",FileName);
	PrintF ("NewPicFileName   = %s\n",NewPicFileName);

	PrintF ("\ntesting 'TDisplay.c'...\n");
	ColorDInfo = OpenColorDisplay (0L,0L,"Config Color Display");
	if (ColorDInfo)
	{
		PrintF ("color display opened...\n");
		PrintF ("xSize: %4ld\n",ColorDInfo->xSize);
		PrintF ("ySize: %4ld\n",ColorDInfo->ySize);
		FirstLine (ColorDInfo);
		for (k=0L;k<256L;k++)
		{
			for (i=0L;i<256;i++) SetRGB24 (ColorDInfo,(i<<8) | k);
			if (AskCancelTrace(ColorDInfo,i,k)) k=256L;
			NextLine (ColorDInfo);
		}
		for (i=2*ColorDInfo->xSize;i>=0L;i--)
			PutRGB24 (ColorDInfo,i,0L,0xff0000);
		for (i=2*ColorDInfo->ySize;i>=0L;i--)
			PutRGB24 (ColorDInfo,0L,i,0xff0000);
		PrintF ("Color at 127|127 %08lx\n",GetRGB24(ColorDInfo,127L,127L));
		PrintF ("Color at   0|0   %08lx\n",GetRGB24(ColorDInfo,  0L,  1L));
		ViewEnd	          (ColorDInfo);
		CloseColorDisplay (ColorDInfo);
	}
	else PrintF ("color display not opened...\n");

	ColorDInfo = OpenHiresDisplay (0L,0L,"Config Hires Display");
	if (ColorDInfo)
	{
		PrintF ("hires display opened...\n");
		CloseHiresDisplay (ColorDInfo);
	}
	else PrintF ("hires display not opened...\n");

	PrintF ("\ntesting 'FileDlg.c'...\n");
	NilDInfo = OpenNilDisplay (0L,0L,"Config Nil Display");

	if (NilDInfo)
	{
		PrintF ("nil display opened...\n");
		Name = FileDialogBox (NilDInfo,"Blizzard files:",FilePath,FileName,true);
		if (Name)
		{
			PrintF ("selected name: %s\n",Name);
			PrintF ("file path: %s\n",FilePath);
			PrintF ("file name: %s\n",FileName);
		}
		else PrintF ("no file name selected...\n");

		PrintF ("\n");
		PrintF ("SplitFileName:\n");
		SplitFileName (Name,FilePath,FileName);
		if (Name)
		{
			PrintF ("file path: %s\n",FilePath);
			PrintF ("file name: %s\n",FileName);
		}
		else Name = "(null)";
		PrintF ("original name: %s\n",Name);
		
		PrintF ("LinkFileName:\n");
		LinkFileName (name,FilePath,FileName);
		PrintF ("file path: %s\n",FilePath);
		PrintF ("file name: %s\n",FileName);
		PrintF ("linked name: %s\n",name);

		CloseNilDisplay (NilDInfo);
	}
	else PrintF ("nil display not opened...\n");

	PrintF ("\ntesting 'Config.c'...\n");
	LongWord = 0x12345678;
	CharPtr  = (char *)&LongWord;
	if (CharPtr[0] == 0x78)			/* Intel */
	{
		PrintF ("Intel like processor...\n");
		if (THISPROCESSOR == INTEL) PrintF ("#define THISPROCESSOR is correct...\n");
		else                        PrintF ("Set: #define THISPROCESSOR INTEL\n");
	}
	else							/* Motorola */
	{
		PrintF ("Motorola like processor...\n");
		if (THISPROCESSOR == MOTOROLA) PrintF ("#define THISPROCESSOR is correct...\n");
		else                           PrintF ("Set: #define THISPROCESSOR MOTOROLA\n");
	}

	PrintF ("random numbers (range: 0 - 10):\n");
	PrintF (" integer     float\n");
	for (i=0L;i<10L;i++) PrintF ("%8ld     %5.3f\n",IRAN(10L),FRAN(10));

	i = 1L; k = 2L;
	PrintF ("SWAP:  i=%ld k=%ld\n",i,k);
	SWAP (i,k);
	PrintF ("       i=%ld k=%ld\n",i,k);
	i = 1L; k = 2L;
	PrintF ("PSWAP: i=%ld k=%ld\n",i,k);
	PSWAP (&i,&k);
	PrintF ("       i=%ld k=%ld\n",i,k);

	Pointer1 = AllocKeyMem (&Global,1000L);
	Pointer2 = AllocKeyChipMem (&Global,1000L);
	PrintF ("Pointer1: %8lx (AllocKeyMem)\n",Pointer1);
	PrintF ("Pointer2: %8lx (AllocKeyChipMem)\n",Pointer2);
	FreePartMem (&Global,Pointer1);
	PrintF ("Pointer1 freed...\n");
	FreePartMem (&Global,NULL);
	PrintF ("NULL-Pointer freed...\n");
	FreeKeyMem  (&Global);
	PrintF ("whole node freed...\n");

	PrintF ("\ntesting 'Bio.c'...\n");
	File = BOpen ("Config.tst",B_WRITE);
	if (File)
	{
		PrintF ("File 'Config.tst' opened...\n");
		BBuffer (File,2048L);
		BWrite  (File,Array,1024L);
		BFlush  (File);
		BWrite  (File,Array,1024L);
		BWrite  (File,Array,1024L);
		BWrite  (File,Array,1024L);
		BBuffer (File,512L);
		BWrite  (File,Array,1024L);
		PrintF ("File Size: %ld (should be 5120 Bytes)\n",BSize(File));
		BClose  (File);

		File = BOpen ("Config.tst",B_READ);
		if (File)
		{
			PrintF ("File 'Config.tst' opened...\n");
			if (BRead(File,Array,128L)<128L) PrintF ("128 not read...\n");
			PrintF ("Seek1: %4ld (should be  128)\n",BSeek (File, 512L,BSEEK_START));
			PrintF ("Seek2: %4ld (should be  512)\n",BSeek (File, 512L,BSEEK_CURRENT));
			PrintF ("Seek3: %4ld (should be 1024)\n",BSeek (File,-120L,BSEEK_END));
			PrintF ("Seek4: %4ld (should be 5000)\n",BSeek (File,   0L,BSEEK_CURRENT));
			BClose (File);
		}
		else PrintF ("File 'Config.tst' not opened for reading...\n");

		code = BExists ("Config.tst");
		switch (code)
		{
			case B_NOT_EXISTANT :
				PrintF ("Config.tst is not existant...\n");
				break;
			case B_TYPE_DIR :
				PrintF ("Config.tst is a directory...\n");
				break;
			case B_TYPE_FILE :
				PrintF ("Config.tst is a file...\n");
				break;

			default :
				PrintF ("Config.tst is if unknown file type (code %ld)\n",code);
				break;
		}

		if (remove ("Config.tst") == 0) PrintF ("File 'Config.tst' removed...\n");
		else PrintF ("File 'Config.tst' not removed...\n");

		code = BExists ("Config.tst");
		switch (code)
		{
			case B_NOT_EXISTANT :
				PrintF ("Config.tst is not existant... (all right)\n");
				break;
			case B_TYPE_DIR :
				PrintF ("Config.tst is a directory...\n");
				break;
			case B_TYPE_FILE :
				PrintF ("Config.tst is a file...\n");
				break;

			default :
				PrintF ("Config.tst is if unknown file type (code %ld)\n",code);
				break;
		}
	}
	else PrintF ("File 'Config.tst' not opened for writing...\n");


	Dir = BOpenDir("");
	name[0] = 0;
	while ((code = BDirNext(Dir,name)) != B_NOT_EXISTANT)
	{
		switch (code)
		{
			case B_TYPE_FILE :
				PrintF ("f: %s\n",name);
				break;
			case B_TYPE_DIR :
				PrintF ("d: %s\n",name);
				break;
			default :
				PrintF ("?: %s\n",name);
				break;
		}
		name[0] = 0;
	}
	Dir = BCloseDir (Dir);

	TIME_STOP;
	PrintF ("Configuration Time: %s\n",CalcTime);

	CloseRequirements();
	return 0;
}
