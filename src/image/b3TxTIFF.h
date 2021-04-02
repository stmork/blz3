/*
**
**	$Filename:	b3TxTIFF.h $
**	$Release:	Dortmund 2005 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - TIFF classes
**
**	(C) Copyright 2005 Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_IMAGE_TXTIFF_H
#define B3_IMAGE_TXTIFF_H

#include "blz3/b3Config.h"

/*************************************************************************
**                                                                      **
**                        TIFF class definitions                        **
**                                                                      **
*************************************************************************/

/**
 * This enumeration lists the possible endian version.
 */
enum b3_tiff_endian
{
	TIFF_INTEL    = 0x4949, // Intel little endian.
	TIFF_MOTOROLA = 0x4d4d  // Motorola big endian.
};

/**
 * This structure defines a TIFF format header.
 */
#pragma pack(2)
struct b3HeaderTIFF
{
	b3_u16         TypeCPU;     //!< The endian version (MM or II).
	b3_u16         VersionTIFF; //!< The version (always 0x2a).
	b3_u32         FirstTag;    //!< The offset to the first TIFF directory.
};
#pragma pack()

/**
 * This structure defines a TIFF tag.
 */
#pragma pack(2)
struct TagTIFF
{
	b3_u16         Code;
	b3_u16         Type;
	b3_u32         Data[2];
};
#pragma pack()

class b3TIFF_Dir;

class b3TIFF_Strip : public b3Link<b3TIFF_Strip>
{
	unsigned long   end;
	char      *     buffer;
	long            size;
	long            offset;
public:
	b3TIFF_Strip();
	void b3Init(char *);
	long b3Size(long);
	long b3Offset();
	long b3WriteData(b3FileAbstract * file_handle, long);
	long b3OrgTags(long);
	long b3OrgStrips(long);
	void b3Traverse(
		b3Base<class T> * head,
		void (*func)(b3Base<class T> *, b3Link<class T> *, void *),
		void       *       ptr);
};

/**
 * This class represents one TIFF directory tag. One tag holds information about one
 * single item such as resolution or strip data.
 */
class b3TIFF_Entry : public b3Link<b3TIFF_Entry>, public b3Mem
{
	char      *     TIFF;
	unsigned long   end;
	struct TagTIFF  tag;
	void      *     buffer;		// buffer to data
	b3_u32     *    ptr;		// new offsets of strips
	long            size;		// size of data
	long            offset;		// offset of this tag
	long            act;        // offset of buffer (if buffer != null)

public:
	b3TIFF_Entry(b3TIFF_Dir *, struct TagTIFF *, char *);
	~b3TIFF_Entry();
	long  b3WriteTag(b3FileAbstract * file_handle, b3Base<b3TIFF_Strip> *, long, long);
	long  b3WriteData(b3FileAbstract * file_handle, long);
	void  b3Traverse(
		b3Base<class T> * head,
		void (*func)(b3Base<class T> * base, b3Link<class T> * link, void * ptr),
		void      *      ptr);
	void  b3RemoveIFW(b3Base<b3TIFF_Entry> *);
	long  b3OrgTags(long);
	long  b3OrgStrips(long);
	static int  b3SortTags(const b3TIFF_Entry * a, const b3TIFF_Entry * b);
};

/**
 * This class represents one TIFF image.
 */
class b3TIFF_Dir : public b3Link<b3TIFF_Dir>
{
	b3Base<b3TIFF_Entry>  tags;
	b3Base<b3TIFF_Strip>  strips;
	unsigned long         end;
	b3TIFF_Strip     *    stripArray;
	long                  num;			// number of tags inside directory
	long                  stripNum;		// number of strips
	long                  offset;		// offset of this dir structure
	long                  next;			// offset of next dir structure
public:
	b3TIFF_Dir(char *, struct TagTIFF *, long);
	~b3TIFF_Dir();
	void            b3Append(b3TIFF_Strip *);
	void            b3Append(b3TIFF_Entry *);
	b3TIFF_Strip * b3FirstStrip();
	b3TIFF_Entry * b3FirstEntry();
	void            b3TravRemoveIFW();
	void            b3Traverse(
		b3Base<class T> * head,
		void (*func)(b3Base<class T> *, b3Link<class T> *, void *),
		void        *       ptr);
	long            b3StripInc();
	long            b3TagInc();
	long            b3Offset();
	void            b3SetStrips(b3TIFF_Strip *);
	long            b3OrgTags(long);
	void            b3OrgStrips();
	long            b3WriteTags(b3FileAbstract * file_handle, long);
};

typedef void (*b3LogTiffFunc)(const char * output, void * ptr);

/**
 * This class is for TIFF format inspection.
 */
class b3TIFF : public b3Link<b3TIFF>, public b3Mem
{
	b3Base<b3TIFF_Dir>  dirs;
	unsigned long       end;
	b3HeaderTIFF          head;
	long                offset;	// offset of this header (= 0)

public:
	b3TIFF(struct b3HeaderTIFF *);
	b3TIFF();
	~b3TIFF();
	long  b3DirCount();
	void  b3Remove(b3TIFF *);
	void  b3Append(b3TIFF *);
	void  b3Write(char *);

	long  b3OrgTags(long);
	void  b3OrgStrips();
	void  b3TravRemoveIFW();
	void  b3Traverse(
		b3Base<class T> * head,
		void (*func)(b3Base<class T> *, b3Link<class T> *, void *),
		void       *      ptr);

public:
	static long           b3GetTIFFTypeSize(struct TagTIFF * DataTag);
	static long           b3GetTIFFSize(struct TagTIFF * DataTag);
	static const char  *  b3GetModeString(long mode);
	static long           b3GetTIFFValue(void * PtrTIFF, struct TagTIFF * DataTag, long Index);
	static long           b3TAFcorrectOffset(long offset);
	static long           b3TAFwriteGap(b3FileAbstract * out, long offset);
	static void           b3LogTIFF(const char * format, ...);

private:
	static void           b3ChangeTag(void * PtrTIFF, struct TagTIFF * DataTag);
	static void           b3ChangeTIFF(struct b3HeaderTIFF * TIFF);
	static void     *     b3LogFuncPtr;
	static b3LogTiffFunc  b3LogFuncTIFF;
	static void           b3SetLogTiffFunc(b3LogTiffFunc log_func, void * ptr);
	static void           b3TravTIFF(b3Base<class T> * Head, b3Link<class T> * Node, void * ptr);
	static void           b3TravRemIFW(b3Base<class T> * Head, b3Link<class T> * Node, void * ptr);
	static void           b3TravOffset(b3Base<class T> * Head, b3Link<class T> * Node, void * ptr);
};

// void          b3SetLogTiffFunc(b3LogTiffFunc log_func,void *ptr = null);

// b3Node classes
#define CLASS_TIFF_HEAD  0x40000
#define CLASS_TIFF_DIR   0x30000
#define CLASS_TIFF_TAG   0x20000
#define CLASS_TIFF_STRIP 0x10000

// TIFF tags
#define TIFF_NEWSUBFILE	0x00fe
#define TIFF_XSIZE		0x0100
#define TIFF_YSIZE		0x0101
#define TIFF_BPS		0x0102
#define TIFF_COMPR		0x0103
#define TIFF_COLORTYPE	0x0106
#define TIFF_DOCNAME	0x010d
#define TIFF_MODEL		0x0110
#define TIFF_STRIPS		0x0111
#define TIFF_SAMPLES	0x0115
#define TIFF_ROWSPERSTR	0x0116
#define TIFF_STRIPBYTES	0x0117
#define TIFF_XRES		0x011a
#define TIFF_YRES		0x011b
#define TIFF_PLANARCONF	0x011c
#define TIFF_RESUNIT	0x0128
#define TIFF_SOFTWARE	0x0131
#define TIFF_PALETTE	0x0140
#define TIFF_IFW1       0x80a4
#define TIFF_IFW2       0x80a6

// TIFF data types
#define MODE_BYTE		1
#define MODE_ASCII		2
#define MODE_SHORT		3
#define MODE_LONG		4
#define MODE_RATIONAL	5

// TAF token
#define TAF_MAGIC_LEN(s)     (long)((strlen(s) + 4) & (~3))
#define TAF_MAGIC            "Dr. Materna GmbH\0\0\0\0\0"
#define TAF_START            0xdeadcafe
#define TAF_SEPARATOR        0xbadc0ded
#define TAF_END              0xdeadbeef

// Puhh!

#endif
