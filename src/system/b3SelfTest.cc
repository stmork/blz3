/*
**
**	$Filename:	TestIo.cc $
**	$Release:	Dortmund 2005 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Test routines
**
**	(C) Copyright 2005  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3SystemIndInclude.h"
#include "blz3/system/b3Date.h"
#include "blz3/system/b3Dir.h"
#include "blz3/system/b3File.h"
#include "blz3/system/b3Mem.h"
#include "blz3/system/b3SelfTest.h"
#include "blz3/base/b3FileMem.h"

#define MEM_MIN         1000
#define MEM_LOW_MULT       2
#define MEM_HIGH_MULT    200

/*************************************************************************
**                                                                      **
**                        b3SelfTest implementation                     **
**                                                                      **
*************************************************************************/

b3_bool b3SelfTest::b3TestDataSize()
{
	b3_bool     *    ptr;

	b3PrintF(B3LOG_NORMAL, "%d-Bit-CPU\n", b3Runtime::b3GetCPUBits());

	switch (b3Runtime::b3GetCPUType())
	{
	case B3_BIG_ENDIAN:
		b3PrintF(B3LOG_NORMAL, "Big endian (MC 680x0)\n");
		break;

	case B3_LITTLE_ENDIAN:
		b3PrintF(B3LOG_NORMAL, "Little endian (x86)\n");
		break;

	default:
		b3PrintF(B3LOG_NORMAL, "Endian type unknown!\n");
		break;
	}

	b3PrintF(B3LOG_NORMAL, "size of some basic data types:\n");
	b3PrintF(B3LOG_NORMAL, "        uns. sgn.\n");
	b3PrintF(B3LOG_NORMAL, "Integer:\n");
	b3PrintF(B3LOG_NORMAL, " 8 bit: %3d  %3d (should be 1)\n", sizeof(b3_u08), sizeof(b3_s08));
	b3PrintF(B3LOG_NORMAL, "16 bit: %3d  %3d (should be 2)\n", sizeof(b3_u16), sizeof(b3_s16));
	b3PrintF(B3LOG_NORMAL, "32 bit: %3d  %3d (should be 4)\n", sizeof(b3_u32), sizeof(b3_s32));
	b3PrintF(B3LOG_NORMAL, "64 bit: %3d  %3d (should be 8)\n", sizeof(b3_u64), sizeof(b3_s64));
	b3PrintF(B3LOG_NORMAL, "bool:   %3d      (should be 4)\n", sizeof(b3_bool));
	b3PrintF(B3LOG_NORMAL, "Float:\n");
	b3PrintF(B3LOG_NORMAL, "32 bit: %3d      (should be  4)\n",       sizeof(b3_f32));
	b3PrintF(B3LOG_NORMAL, "64 bit: %3d      (should be  8)\n",       sizeof(b3_f64));
	b3PrintF(B3LOG_NORMAL, "96 bit: %3d      (should be 12 or 16)\n", sizeof(b3_f96));
	b3PrintF(B3LOG_NORMAL, "\n");
	b3PrintF(B3LOG_NORMAL, "Pointer size:                     %d bytes.\n", sizeof(ptr));
	b3PrintF(B3LOG_NORMAL, "Int size for pointer arithmetics: %d bytes (%s).\n",
		sizeof(b3_ptr), sizeof(ptr) == sizeof(b3_ptr) ? "OK" : "different - not good");

	return true;
}

b3_bool b3SelfTest::b3TestMemory()
{
	b3_u32    v1, v2;
	void   *  ptr1, *ptr2;
	b3Mem     mem;
	b3_count  count = 0;
	b3_u08    buffer[MEM_MIN];
	b3_bool   equal;
	b3_bool   result = true;
	int       i, v;

	// Put some stuff into realloc buffer
	b3PrintF(B3LOG_NORMAL, "\ntesting memory handling...\n");
	for (i = 0; i < MEM_MIN; i++)
	{
#if 1
		v         = B3_IRAN(256);
#else
		v         = i;
#endif
		buffer[i] = (b3_u08)(v & 0xff);
	}
	mem.b3Dump();

	ptr1 = mem.b3Alloc(MEM_MIN);
	result &= (ptr1 != nullptr);
	b3PrintF(B3LOG_NORMAL, "ptr1: %p\n", ptr1);
	mem.b3Dump();

	ptr2 = mem.b3Alloc(MEM_MIN);
	result &= (ptr2 != nullptr);
	b3PrintF(B3LOG_NORMAL, "ptr2: %p\n", ptr2);
	mem.b3Dump();

	result &= mem.b3Free(ptr1);
	b3PrintF(B3LOG_NORMAL, "ptr1 freed...\n");
	mem.b3Dump();

	result &= (!mem.b3Free(NULL));
	b3PrintF(B3LOG_NORMAL, "NULL-Pointer freed...\n");
	mem.b3Dump();

	result &= mem.b3Free();
	b3PrintF(B3LOG_NORMAL, "whole node freed...\n");
	mem.b3Dump();

	ptr1 = mem.b3Realloc(nullptr,  MEM_MIN * 2);
	mem.b3Dump();
	for (i = 0; i < MEM_MIN; i++)
	{
		((b3_u08 *)ptr1)[i] = buffer[i];
	}
	b3PrintF(B3LOG_NORMAL, "ptr1 = %p after b3Realloc() (%s)\n",
		ptr1,
		ptr1 != nullptr ? "OK" : "wrong");
	result &= (ptr1 != nullptr);

	ptr2 = mem.b3Realloc(ptr1,  MEM_MIN);
	mem.b3Dump();
	b3PrintF(B3LOG_NORMAL, "ptr2 = %p, ptr1 = %p after b3Realloc() with size reduction (%s)\n",
		ptr2, ptr1,
		(ptr1 == ptr2) && (ptr2 != nullptr) ? "OK" : "wrong");
	result &= ((ptr1 == ptr2) && (ptr2 != nullptr));

	ptr1 = mem.b3Realloc(ptr2, MEM_MIN * MEM_HIGH_MULT);
	mem.b3Dump();
	b3PrintF(B3LOG_NORMAL, "ptr1 = %p, ptr2 = %p after b3Realloc() with size enlargement (%s)\n",
		ptr1, ptr2,
		(ptr1 != ptr2) && (ptr1 != nullptr) ? "OK" : "wrong");
	result &= ((ptr1 != ptr2) && (ptr1 != nullptr));

	equal = memcmp(buffer, ptr1, MEM_MIN) == 0;
	b3PrintF(B3LOG_NORMAL, "   Memory buffer is %s\n",
		equal ? "preserved (OK)" : "corrupted (wrong)");

	if (!equal)
	{
		result = false;
		for (i = 0; i < MEM_MIN; i++)
		{
			if ((i & 7) == 0)
			{
				b3PrintF(B3LOG_NORMAL, "\n%04x: ", i);
			}
			b3PrintF(B3LOG_NORMAL, " %02x-%02x", buffer[i], ((b3_u08 *)ptr1)[i]);
		}
		b3PrintF(B3LOG_NORMAL, "\n");
	}

	for (i = MEM_MIN; i < (MEM_MIN * MEM_HIGH_MULT); i++)
	{
		count += ((char *)ptr1)[i];
	}
	b3PrintF(B3LOG_NORMAL, "   Rest memory buffer is %s\n",
		count == 0 ? "zero initialized (OK)" : "garbled (wrong)");
	result &= (count == 0);

	ptr2 = mem.b3Realloc(ptr1,     0);
	b3PrintF(B3LOG_NORMAL, "ptr2 = %p, ptr1 = %p after b3Realloc() with zero size allocation (%s)\n",
		ptr2, ptr1,
		(ptr1 != ptr2) && (ptr2 == nullptr) ? "OK" : "wrong");
	result &= ((ptr1 != ptr2) && (ptr2 == nullptr));

	b3PrintF(B3LOG_NORMAL, "\n");
	v1 = 1;
	v2 = 2;
	b3PrintF(B3LOG_NORMAL, "SWAP:  i=%ld k=%ld\n", v1, v2);
	B3_SWAP(v1, v2);
	b3PrintF(B3LOG_NORMAL, "       i=%ld k=%ld\n", v1, v2);
	result &= ((v1 == 2) && (v2 == 1));

	v1 = 1;
	v2 = 2;
	b3PrintF(B3LOG_NORMAL, "PSWAP: i=%ld k=%ld\n", v1, v2);
	B3_PSWAP(&v1, &v2);
	b3PrintF(B3LOG_NORMAL, "       i=%ld k=%ld\n", v1, v2);
	result &= ((v1 == 2) && (v2 == 1));

	return result;
}

b3_bool b3SelfTest::b3TestDate()
{
	b3Date date;

	b3PrintF(B3LOG_NORMAL, "\ntesting date handling...\n");
	return date.b3Y2K_Selftest();
}

b3_bool b3SelfTest::b3TestDir()
{
	b3Dir        dir;
	b3Path       name;
	b3_path_type code;

	dir.b3OpenDir(".");
	name.b3Empty();
	while ((code = dir.b3DirNext(name)) != B3_NOT_EXISTANT)
	{
		switch (code)
		{
		case B3_TYPE_FILE :
			b3PrintF(B3LOG_NORMAL, "f: %s\n", (const char *)name);
			break;
		case B3_TYPE_DIR :
			b3PrintF(B3LOG_NORMAL, "d: %s\n", (const char *)name);
			break;
		default :
			b3PrintF(B3LOG_NORMAL, "?: %s\n", (const char *)name);
			break;
		}
		name.b3Empty();
	}
	dir.b3CloseDir();

	return true;
}

b3_bool b3SelfTest::b3TestFile(b3FileAbstract & file)
{
	b3_path_type code;
	char         array[1024];
	b3_bool      success;

	b3PrintF(B3LOG_NORMAL, "File 'Config.tst' opened...\n");
	file.b3Buffer(2048);
	file.b3Write(array, 1024);
	file.b3Flush();
	file.b3Write(array, 1024);
	file.b3Write(array, 1024);
	file.b3Write(array, 1024);
	file.b3Buffer(512);
	file.b3Write(array, 1024);
	b3PrintF(B3LOG_NORMAL, "File Size: %ld (should be 5120 Bytes)\n", file.b3Size());
	success = file.b3Size() == 5120;
	file.b3Close();

	if (file.b3Open("Config.tst", B_READ))
	{
		b3PrintF(B3LOG_NORMAL, "File 'Config.tst' opened...\n");
		if (file.b3Read(array, 128) < 128)
		{
			b3PrintF(B3LOG_NORMAL, "128 not read...\n");
			success = false;
		}
		b3PrintF(B3LOG_NORMAL, "Seek1: %4ld (should be  128)\n", file.b3Seek(512, B3_SEEK_START));
		b3PrintF(B3LOG_NORMAL, "Seek2: %4ld (should be  512)\n", file.b3Seek(512, B3_SEEK_CURRENT));
		b3PrintF(B3LOG_NORMAL, "Seek3: %4ld (should be 1024)\n", file.b3Seek(-120, B3_SEEK_END));
		b3PrintF(B3LOG_NORMAL, "Seek4: %4ld (should be 5000)\n", file.b3Seek(0, B3_SEEK_CURRENT));
		file.b3Close();
	}
	else
	{
		b3PrintF(B3LOG_NORMAL, "File 'Config.tst' not opened for reading...\n");
		success = false;
	}

	code = b3Dir::b3Exists("Config.tst");
	switch (code)
	{
	case B3_NOT_EXISTANT :
		b3PrintF(B3LOG_NORMAL, "Config.tst is not existant...\n");
		break;
	case B3_TYPE_DIR :
		b3PrintF(B3LOG_NORMAL, "Config.tst is a directory...\n");
		break;
	case B3_TYPE_FILE :
		b3PrintF(B3LOG_NORMAL, "Config.tst is a file...\n");
		break;

	default :
		b3PrintF(B3LOG_NORMAL, "Config.tst is if unknown file type (code %ld)\n", code);
		success = false;
		break;
	}

	return success;
}

b3_bool b3SelfTest::b3TestIO()
{
	b3File        file;
	b3FileMem     filemem;
	b3_path_type  code;
	b3_bool       success = true;

	b3PrintF(B3LOG_NORMAL, "\ntesting basic i/o handling...\n");
	b3PrintF(B3LOG_NORMAL, "Disk file: --------------------\n");
	if (file.b3Open("Config.tst", B_WRITE))
	{
		success &= b3TestFile(file);
	}
	else
	{
		success &= false;
	}

	b3PrintF(B3LOG_NORMAL, "Memory file: ------------------\n");
	if (filemem.b3Open(B_WRITE))
	{
		success &= b3TestFile(filemem);
	}
	else
	{
		success = false;
	}

	b3PrintF(B3LOG_NORMAL, "file operations: --------------\n");
	if (remove("Config.tst") == 0)
	{
		b3PrintF(B3LOG_NORMAL, "File 'Config.tst' removed...\n");
	}
	else
	{
		b3PrintF(B3LOG_NORMAL, "File 'Config.tst' not removed...\n");
		success = false;
	}

	code = b3Dir::b3Exists("Config.tst");
	switch (code)
	{
	case B3_NOT_EXISTANT :
		b3PrintF(B3LOG_NORMAL, "Config.tst is not existant... (all right)\n");
		break;
	case B3_TYPE_DIR :
		b3PrintF(B3LOG_NORMAL, "Config.tst is a directory...\n");
		break;
	case B3_TYPE_FILE :
		b3PrintF(B3LOG_NORMAL, "Config.tst is a file...\n");
		break;

	default :
		b3PrintF(B3LOG_NORMAL, "Config.tst is if unknown file type (code %ld)\n", code);
		success = false;
		break;
	}
	success &= b3TestDir();

	return success;
}
