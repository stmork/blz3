/*
**
**	$Filename:	b3TxTIFF.cc $
**	$Release:	Dortmund 2001, 2016 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - handling TIFFs
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3TxInclude.h"
#include "blz3/system/b3File.h"

#include "b3TxTIFF.h"

/*************************************************************************
**                                                                      **
**                            handle TIFF's                             **
**                                                                      **
*************************************************************************/

const char * b3TIFF::b3GetModeString(long mode)
{
	switch (mode)
	{
	case MODE_BYTE     :
		return "byte ";
	case MODE_SHORT    :
		return "short";
	case MODE_ASCII    :
		return "ascii";
	case MODE_LONG     :
		return "long ";
	case MODE_RATIONAL :
		return "ratio";
	}
	return "???  ";
}

long b3TIFF::b3GetTIFFTypeSize(struct TagTIFF * DataTag)
{
	switch (DataTag->Type)
	{
	case MODE_BYTE     :
		return (sizeof(char));
	case MODE_SHORT    :
		return (sizeof(short));
	case MODE_ASCII    :
	case MODE_LONG     :
		return (sizeof(long));
	case MODE_RATIONAL :
		return (sizeof(long) * 2);
	}
	return 0;
}

long b3TIFF::b3GetTIFFSize(struct TagTIFF * DataTag)
{
	long size;

	size = b3GetTIFFTypeSize(DataTag);
	if (size != 0)
	{
		return 4 / size;
	}
	return 0;
}

void b3TIFF::b3ChangeTag(
	void      *     PtrTIFF,
	struct TagTIFF * DataTag)
{
	short * Short;
	char * TIFF = (char *)PtrTIFF;
	long * Long;
	long   i, TagSize;

	b3Endian::b3ChangeEndian16(&DataTag->Code);
	b3Endian::b3ChangeEndian16(&DataTag->Type);
	b3Endian::b3ChangeEndian32(&DataTag->Data[0]);
	TagSize = b3Endian::b3Get32(&DataTag->Data[0]);
	switch (DataTag->Type)
	{
	case MODE_BYTE :					/* Char */
		if (TagSize > b3GetTIFFSize(DataTag))
		{
			b3Endian::b3ChangeEndian32(&DataTag->Data[1]);
		}
		break;

	case MODE_SHORT :					/* Short */
		if (TagSize > b3GetTIFFSize(DataTag))
		{
			b3Endian::b3ChangeEndian32(&DataTag->Data[1]);
			TIFF += DataTag->Data[1];
			Short = (short *)TIFF;
			for (i = 0L; i < TagSize; i++)
			{
				b3Endian::b3ChangeEndian16(Short);
				Short++;
			}
		}
		else
		{
			Short = (short *)&DataTag->Data[1];
			b3Endian::b3ChangeEndian16(&Short[0]);
			b3Endian::b3ChangeEndian16(&Short[1]);
		}
		break;

	case MODE_LONG :					/* Long */
		b3Endian::b3ChangeEndian32(&DataTag->Data[1]);
		if (TagSize > b3GetTIFFSize(DataTag))
		{
			TIFF += DataTag->Data[1];
			Long = (long *)TIFF;
			for (i = 0; i < TagSize; i++)
			{
				b3Endian::b3ChangeEndian32(Long);
				(long *)Long++;
			}
		}
		break;

	case MODE_ASCII :					/* ASCII */
	case MODE_RATIONAL :				/* Fractional */
		b3Endian::b3ChangeEndian32(&DataTag->Data[1]);
		break;
	}
}

void b3TIFF::b3ChangeTIFF(struct HeaderTIFF * TIFF)
{
	long            offset, i, Tags;
	b3_u08     *    Data;
	b3_u16     *    Shorts;
	unsigned long * Longs;

	b3Endian::b3ChangeEndian16(&TIFF->VersionTIFF);
	b3Endian::b3ChangeEndian32(&TIFF->FirstTag);
	offset = TIFF->FirstTag;
	while (offset != 0)
	{
		Data   = (b3_u08 *)TIFF;
		Data  += offset;
		b3Endian::b3ChangeEndian16(Data);
		Shorts = (b3_u16 *)Data;
		Tags   = b3Endian::b3Get16(Shorts);
		Data  += 2;
		for (i = 0; i < Tags; i++)
		{
			b3ChangeTag(TIFF, (struct TagTIFF *)Data);
			Data += sizeof(struct TagTIFF);
		}
		Longs = (unsigned long *)Data;
		b3Endian::b3ChangeEndian32(Longs);
		offset = b3Endian::b3Get32(Longs);
	}
}

long b3TIFF::b3GetTIFFValue(
	void      *     PtrTIFF,
	struct TagTIFF * DataTag,
	long            Index)
{
	b3_u08 * TIFF = (b3_u08 *)PtrTIFF;
	b3_u08 * Char;
	b3_u16 * Short;
	b3_u32 * Long, value;

	switch (DataTag->Type)
	{
	case MODE_BYTE :					/* char, ASCII */
	case MODE_ASCII :
		if ((b3_s32)b3Endian::b3Get32(&DataTag->Data[0]) > b3GetTIFFSize(DataTag))
		{
			TIFF += b3Endian::b3Get32(&DataTag->Data[1]);
			Char  = TIFF;
		}
		else
		{
			Char = (b3_u08 *)&DataTag->Data[1];
		}
		Char += Index;
		return (Char[0]);
	case MODE_SHORT :					/* short */
		if ((b3_s32)b3Endian::b3Get32(&DataTag->Data[0]) > b3GetTIFFSize(DataTag))
		{
			TIFF += b3Endian::b3Get32(&DataTag->Data[1]);
			Short = (b3_u16 *)TIFF;
		}
		else
		{
			Short = (b3_u16 *)&DataTag->Data[1];
		}
		Short += Index;
		return (b3Endian::b3Get16(Short));
	case MODE_LONG :					/* long */
		if ((b3_s32)b3Endian::b3Get32(&DataTag->Data[0]) > b3GetTIFFSize(DataTag))
		{
			TIFF += b3Endian::b3Get32(&DataTag->Data[1]);
			Long  = (b3_u32 *)TIFF;
		}
		else
		{
			Long = (b3_u32 *)&DataTag->Data[1];
		}
		Long += Index;
		return (b3Endian::b3Get32(Long));
	case MODE_RATIONAL :
		TIFF += b3Endian::b3Get32(&DataTag->Data[1]);
		Long  = (b3_u32 *)TIFF;
		Long += (Index * 2);
		value = b3Endian::b3Get32(Long);
		Long++;
		return value / b3Endian::b3Get32(Long);
		break;
	}
	return (0L);
}

/*************************************************************************
**                                                                      **
**                            TAF series                                **
**                                                                      **
*************************************************************************/

long b3TIFF::b3TAFcorrectOffset(long offset)
{
#ifdef FILL_GAP
	return (offset + 3) & 0xfffffffc;
#else
	return offset;
#endif
}

long b3TIFF::b3TAFwriteGap(
	b3FileAbstract * out B3_UNUSED,
	long             offset)
{
#ifdef FILL_GAP
	char gap[8];
	long i, toWrite;

	toWrite = (4 - offset) & 0x3;
	for (i = 0; i < toWrite; i++)
	{
		gap[i] = 0;
	}
	out->b3Write(gap, toWrite) ;
#endif
	return b3TAFcorrectOffset(offset);
}

/*************************************************************************
**                                                                      **
**                            Logging routines                          **
**                                                                      **
*************************************************************************/

void     *     b3TIFF::b3LogFuncPtr  = nullptr;
b3LogTiffFunc  b3TIFF::b3LogFuncTIFF = nullptr;

void b3TIFF::b3SetLogTiffFunc(b3LogTiffFunc log_func, void * ptr)
{
	b3LogFuncTIFF = log_func;
	b3LogFuncPtr  = ptr;
}

void b3TIFF::b3LogTIFF(const char * format, ...)
{
	char    message[10240];
	va_list list;

	va_start(list, format);
	vsnprintf(message, sizeof(message), format, list);
	va_end(list);
	b3PrintF(B3LOG_FULL, message);

	if (b3LogFuncTIFF != nullptr)
	{
		b3LogFuncTIFF(message, b3LogFuncPtr);
	}
}

/*************************************************************************
**                                                                      **
**                            b3TIFF_Dir                                **
**                                                                      **
*************************************************************************/

b3TIFF_Dir::b3TIFF_Dir(
	char      *     TIFF,
	struct TagTIFF * ThisTag,
	long            Tags) : b3Link<b3TIFF_Dir>(sizeof(b3TIFF_Dir), CLASS_TIFF_DIR)
{
	b3TIFF_Entry * tagTIFF;
	long            i;

	tags.b3InitBase(CLASS_TIFF_TAG);
	strips.b3InitBase(CLASS_TIFF_STRIP);
	num        = 0;
	stripNum   = 0;
	offset     = 0;
	next       = 0;
	stripArray = nullptr;

	// For each tag do...
	b3TIFF::b3LogTIFF("code type data[0]  data[1]\n");
	for (i = 0; i < Tags; i++)
	{
		// Set real tag
		// Allocate structure class (b3TIFF_Entry)
		try
		{
			tagTIFF = new b3TIFF_Entry(this, ThisTag, TIFF);
		}
		catch (...)
		{
			tagTIFF = nullptr;
		}
		if (tagTIFF == nullptr)
		{
			B3_THROW(b3TxException, B3_TX_ERROR);
		}

		tags.b3Append(tagTIFF);
		b3TagInc();
		ThisTag++;
	}
}

b3TIFF_Dir::~b3TIFF_Dir()
{
	b3TIFF_Entry * tTIFF;

	while ((tTIFF = tags.First) != nullptr)
	{
		tags.b3Remove(tTIFF);
		delete tTIFF;
	}

	if (stripArray)
	{
		delete[] stripArray;
	}
}

void b3TIFF_Dir::b3Append(b3TIFF_Entry * entry)
{
	tags.b3Append(entry);
}

void b3TIFF_Dir::b3Append(b3TIFF_Strip * strip)
{
	strips.b3Append(strip);
}

b3TIFF_Strip * b3TIFF_Dir::b3FirstStrip()
{
	return strips.First;
}

b3TIFF_Entry * b3TIFF_Dir::b3FirstEntry()
{
	return tags.First;
}

long b3TIFF_Dir::b3StripInc()
{
	stripNum++;
	return stripNum;
}

long b3TIFF_Dir::b3TagInc()
{
	num++;
	return num;
}

void b3TIFF_Dir::b3SetStrips(b3TIFF_Strip * array)
{
	stripArray = array;
}

long b3TIFF_Dir::b3Offset()
{
	return offset;
}

void b3TIFF_Dir::b3Traverse(
	b3Base<class T> * head,
	void (*func)(b3Base<class T> *, b3Link<class T> *, void *),
	void        *       ptr)
{
	b3TIFF_Entry * tTIFF, *tnTIFF;
	b3TIFF_Strip * sTIFF, *snTIFF;

	func(head, (b3Link<class T> *)this, ptr);
	for (tTIFF  = tags.First;
		tTIFF != nullptr;
		tTIFF  = tnTIFF)
	{
		tnTIFF = tTIFF->Succ;
		tTIFF->b3Traverse((b3Base<class T> *)&tags, func, ptr);
	}

	for (sTIFF  = strips.First;
		sTIFF != nullptr;
		sTIFF  = snTIFF)
	{
		snTIFF = sTIFF->Succ;
		sTIFF->b3Traverse((b3Base<class T> *)&strips, func, ptr);
	}
}

void b3TIFF_Dir::b3TravRemoveIFW()
{
	b3TIFF_Entry * tTIFF, *succ;

	for (tTIFF = tags.First; tTIFF != nullptr; tTIFF = succ)
	{
		succ = tTIFF->Succ;
		tTIFF->b3RemoveIFW(&tags);
	}
}

long b3TIFF_Dir::b3OrgTags(long act_offset)
{
	b3TIFF_Entry * son;

	offset = act_offset;
	num    = 0;
	for (son = tags.First; son != nullptr; son = son->Succ)
	{
		num++;
	}

	tags.b3Sort(b3TIFF_Entry::b3SortTags, nullptr);
	act_offset += (sizeof(short) + sizeof(long));
	b3TIFF::b3LogTIFF("  Dir:   %6ld # %ld tags, %ld strips\n",
		offset, num, stripNum);
	return act_offset;
}

void b3TIFF_Dir::b3OrgStrips()
{
	b3TIFF_Dir * nTIFF;

	if ((nTIFF = (b3TIFF_Dir *)Succ) != nullptr)
	{
		next = nTIFF->b3Offset();
	}
	else
	{
		next = 0;
	}
}

long b3TIFF_Dir::b3WriteTags(b3FileAbstract * out, long act_offset)
{
	b3TIFF_Entry  * tTIFF;
	volatile short  write_num;

	// Write number of tags
	write_num = (short)(num & 0xffff);
	out->b3Write((const void *)&write_num, sizeof(write_num));
	b3TIFF::b3LogTIFF("  D:   %6ld - %6ld\n", offset, act_offset);
	act_offset += sizeof(short);

	// For each tag
	for (tTIFF  = (b3TIFF_Entry *)tags.First;
		tTIFF != nullptr;
		tTIFF  = (b3TIFF_Entry *)tTIFF->Succ)
	{
		act_offset = tTIFF->b3WriteTag(out, &strips, act_offset, stripNum);
	}
	out->b3Write(&next, sizeof(next));
	b3TIFF::b3LogTIFF("  O:            %6ld : %6ld\n", act_offset, next);

	act_offset += sizeof(long);
	return act_offset;
}

/*************************************************************************
**                                                                      **
**                            b3TIFF_Entry                              **
**                                                                      **
*************************************************************************/

b3TIFF_Entry::b3TIFF_Entry(
	b3TIFF_Dir  * dirTIFF,
	struct TagTIFF * ThisTag,
	char      *     ptrTIFF) : b3Link<b3TIFF_Entry>(sizeof(b3TIFF_Entry), CLASS_TIFF_TAG)
{
	b3TIFF_Strip * strips = nullptr;
	long            tag_size, num, s;

	tag_size = b3TIFF::b3GetTIFFTypeSize(ThisTag);
	TIFF     = (char *)ptrTIFF;
	buffer   = nullptr;
	ptr      = nullptr;

	tag  = *ThisTag;
	size = b3Endian::b3Get32(&ThisTag->Data[0]) * tag_size;
	b3TIFF::b3LogTIFF("%4x %4x %08lx %08lx\n",
		tag.Code & 0xffffL,
		tag.Type & 0xffffL,
		tag.Data[0],
		tag.Data[1]);

	if (tag_size > 0)
	{
		long tag_num  = b3Endian::b3Get32(&tag.Data[0]);
		long tag_size = b3TIFF::b3GetTIFFSize(&tag);

		if (tag_num > tag_size)
		{
			buffer = b3Endian::b3Get32(&ThisTag->Data[1]) + TIFF;
			//			b3LogTIFF ("### %ld - %ld (%lx)###\n",TIFF,buffer,(long)buffer - (long)TIFF);
		}
	}

	// getting strip offsets
	if (ThisTag->Code == TIFF_STRIPS)
	{
		num = b3Endian::b3Get32(&ThisTag->Data[0]);
		tag.Type = MODE_LONG;
		if (num > 1)
		{
			buffer = (long *)b3Alloc(num * sizeof(long));
			ptr    = (long *)buffer;
		}
		else
		{
			ptr = &tag.Data[1];
		}

		try
		{
			strips = new b3TIFF_Strip[num];
		}
		catch (...)
		{
			strips = nullptr;
		}

		if (strips)
		{
			dirTIFF->b3SetStrips(strips);
			for (s = 0; s < num; s++)
			{
				long offset = b3TIFF::b3GetTIFFValue(TIFF, ThisTag, s);

				strips[s].b3Init((char *)(offset + (char *)TIFF));

				dirTIFF->b3Append(&strips[s]);
				dirTIFF->b3StripInc();
			}
		}
		else
		{
			B3_THROW(b3TxException, B3_TX_ERROR);
		}
	}

	// getting strip length
	if (ThisTag->Code == TIFF_STRIPBYTES)
	{
		num      = b3Endian::b3Get32(&ThisTag->Data[0]);
		tag.Type = MODE_LONG;

		// Get strips itself
		if (num > 1)
		{
			buffer = (char *)b3Alloc(num * sizeof(long));
			ptr    = (long *)buffer;
		}
		else
		{
			ptr = &tag.Data[1];
		}

		// Get strip length
		s = 0;
		for (strips  = (b3TIFF_Strip *)dirTIFF->b3FirstStrip();
			strips != nullptr;
			strips  = (b3TIFF_Strip *)strips->Succ)
		{
			ptr[s] = strips->b3Size(b3TIFF::b3GetTIFFValue(TIFF, ThisTag, s));
			s++;
		}
	}
}

b3TIFF_Entry::~b3TIFF_Entry()
{
}

void b3TIFF_Entry::b3Traverse(
	b3Base<class T> * head,
	void (*func)(b3Base<class T> *, b3Link<class T> *, void *),
	void       *       ptr)
{
	b3Link<class T> * node;

	node = (b3Link<class T> *)this;
	func(head, node, ptr);
}

void b3TIFF_Entry::b3RemoveIFW(b3Base<b3TIFF_Entry> * head)
{
	switch (tag.Code)
	{
	case TIFF_IFW1 :
	case TIFF_IFW2 :
		head->b3Remove(this);
		delete this;
		break;
	}
}

long b3TIFF_Entry::b3OrgTags(long act_offset)
{
	offset = act_offset - sizeof(long);
	act_offset += sizeof(struct TagTIFF);
	b3TIFF::b3LogTIFF("    Tag: %6ld # %4x %4x %08lx %08lx\n",
		offset,
		tag.Code & 0xffffL,
		tag.Type & 0xffffL,
		tag.Data[0],
		tag.Data[1]);
	return act_offset;
}

long b3TIFF_Entry::b3OrgStrips(long act_offset)
{
	if (buffer)
	{
		act        = b3TIFF::b3TAFcorrectOffset(act_offset);
		act_offset = act + size;
	}
	else
	{
		act = 0;
	}

	return act_offset;
}

long b3TIFF_Entry::b3WriteTag(
	b3FileAbstract    *   out,
	b3Base<b3TIFF_Strip> * strips,
	long                  act_offset,
	long                  stripNum)
{
	b3TIFF_Strip * sTIFF;

	// insert buffer new reference
	if (buffer)
	{
		tag.Data[1] = act;
	}

	// if we have the strips here compute
	// new offsets but do not write them.
	if (tag.Code == TIFF_STRIPS)
	{
		long s;

		sTIFF = strips->First;
		for (s = 0; s < stripNum; s++)
		{
			ptr[s] = sTIFF->b3Offset();
			sTIFF  = sTIFF->Succ;
		}
	}

	// Write and make debug output
	out->b3Write(&tag, sizeof(tag));
	b3TIFF::b3LogTIFF("    T: %6ld - %6ld : %6ld %4lx\n",
		offset, act_offset, buffer ? act : 0, tag.Code);
	act_offset += sizeof(struct TagTIFF);
	return act_offset;
}

long b3TIFF_Entry::b3WriteData(b3FileAbstract * out, long act_offset)
{
	if (buffer)
	{
		act_offset = b3TIFF::b3TAFwriteGap(out, act_offset);
		out->b3Write(buffer, size);
		b3TIFF::b3LogTIFF("  B:   %6ld - %6ld :        %4x %6ld\n",
			act, act_offset, tag.Code, size);
		act_offset += size;
	}
	return act_offset;
}

/*************************************************************************
**                                                                      **
**                            b3TIFF_Strip                              **
**                                                                      **
*************************************************************************/

b3TIFF_Strip::b3TIFF_Strip() :
	b3Link<b3TIFF_Strip>(sizeof(b3TIFF_Strip), CLASS_TIFF_STRIP)
{
}

void b3TIFF_Strip::b3Init(char * new_buffer)
{
	buffer = new_buffer;
	offset = 0;
}

long b3TIFF_Strip::b3Size(long new_size)
{
	size = new_size;
	return size;
}

long b3TIFF_Strip::b3Offset()
{
	return offset;
}

void b3TIFF_Strip::b3Traverse(
	b3Base<class T> * head,
	void (*func)(b3Base<class T> *, b3Link<class T> *, void *),
	void       *       ptr)
{
	func(head, (b3Link<class T> *)this, ptr);
}

long b3TIFF_Strip::b3OrgTags(long act_offset)
{
	b3TIFF::b3LogTIFF("    Str: %6ld # %p %6ld\n", offset, buffer, size);
	return act_offset;
}

long b3TIFF_Strip::b3OrgStrips(long act_offset)
{
	if (buffer)
	{
		offset     = b3TIFF::b3TAFcorrectOffset(act_offset);
		act_offset = offset + size;
	}
	return act_offset;
}

long b3TIFF_Strip::b3WriteData(b3FileAbstract * out, long act_offset)
{
	act_offset = b3TIFF::b3TAFwriteGap(out, act_offset);

	out->b3Write(buffer, size);
	b3TIFF::b3LogTIFF("  S:   %6ld - %6ld :             %6ld\n",
		offset, act_offset, size);
	act_offset += size;
	return act_offset;
}

/*************************************************************************
**                                                                      **
**                            b3TIFF                                    **
**                                                                      **
*************************************************************************/

b3TIFF::b3TIFF() : b3Link<b3TIFF>(sizeof(b3TIFF), CLASS_TIFF_HEAD)
{
#if THISPROCESSOR == INTEL
	head.TypeCPU     = TIFF_INTEL;
#else
	head.TypeCPU     = TIFF_MOTOROLA;
#endif
	head.VersionTIFF = 42;
	head.FirstTag    =  0;
	end              =  0;
	offset           =  0;
	dirs.b3InitBase(CLASS_TIFF_DIR);
}

b3TIFF::b3TIFF(struct HeaderTIFF * TIFF) : b3Link<b3TIFF>(sizeof(b3TIFF), CLASS_TIFF_HEAD)
{
	long         offset, Tags;
	b3_u08   *   Data;
	b3_u16   *   Shorts;
	b3TIFF_Dir * dirTIFF;

	// convert TIFF saved on other CPU type
#if THISPROCESSOR == INTEL
	if (TIFF->TypeCPU == TIFF_MOTOROLA)
	{
		b3ChangeTIFF(TIFF);
	}
#else
	if (TIFF->TypeCPU == TIFF_INTEL)
	{
		b3ChangeTIFF(TIFF);
	}
#endif

	dirs.b3InitBase(CLASS_TIFF_DIR);
	head   = *TIFF;

	offset = TIFF->FirstTag;
	while (offset != 0)
	{
		Data   = (b3_u08 *)TIFF;
		Data  += offset;
		Shorts = (b3_u16 *)Data;
		Tags   = b3Endian::b3Get16(Shorts);
		Data  += 2;

		// Allocate b3TIFF_Dir class
		b3LogTIFF("Try to allocate dir with %ld tags\n", Tags);
		try
		{
			dirTIFF  = new b3TIFF_Dir((char *)TIFF, (struct TagTIFF *)Data, Tags);
		}
		catch (...)
		{
			dirTIFF = nullptr;
		}
		if (dirTIFF == nullptr)
		{
			B3_THROW(b3TxException, B3_TX_ERROR);
		}
		dirs.b3Append(dirTIFF);

		Data   += (sizeof(struct TagTIFF) * Tags);
		offset  = b3Endian::b3Get32(Data);
	}
}

b3TIFF::~b3TIFF()
{
	b3TIFF_Dir * dirTIFF;

	while ((dirTIFF = dirs.First) != nullptr)
	{
		dirs.b3Remove(dirTIFF);
		delete dirTIFF;
	}
}

void b3TIFF::b3Traverse(
	b3Base<class T> * head,
	void (*func)(b3Base<class T> *, b3Link<class T> *, void *),
	void        *       ptr)
{
	b3TIFF_Dir * dTIFF, *dnTIFF;

	func(head, (b3Link<class T> *)this, ptr);
	for (dTIFF  = dirs.First;
		dTIFF != nullptr;
		dTIFF  = dnTIFF)
	{
		dnTIFF = dTIFF->Succ;
		dTIFF->b3Traverse((b3Base<class T> *)&dirs, func, ptr);
	}
}

long b3TIFF::b3OrgTags(long act_offset)
{
	offset      = act_offset;
	act_offset += sizeof(struct HeaderTIFF);

#if THISPROCESSOR == INTEL
	head.TypeCPU = TIFF_INTEL;
#else
	head.TypeCPU = TIFF_MOTOROLA;
#endif
	b3LogTIFF("Head:    %6ld # %4lx\n",
		offset, (long)head.TypeCPU);
	return act_offset;
}

void b3TIFF::b3OrgStrips()
{
	b3TIFF_Dir * dTIFF;

	dTIFF = dirs.First;
	if (dTIFF)
	{
		head.FirstTag = dTIFF->b3Offset();
	}
}

long b3TIFF::b3DirCount()
{
	b3TIFF_Dir * node;
	long          count = 0;

	for (node = dirs.First; node != nullptr; node = node->Succ)
	{
		count++;
	}
	return count;
}

void b3TIFF::b3Append(b3TIFF * tiff)
{
	b3TIFF_Dir * dTIFF;

	// Copy tiff's pages into this instance
	while ((dTIFF = tiff->dirs.First) != nullptr)
	{
		tiff->dirs.b3Remove(dTIFF);
		dirs.b3Append(dTIFF);
	}
}

void b3TIFF::b3Remove(b3TIFF * tiff)
{
	b3TIFF_Dir * dTIFF;

	// Copy first page of this instance
	// into tiff's range
	dTIFF = dirs.First;
	if (dTIFF)
	{
		dirs.b3Remove(dTIFF);
		tiff->dirs.b3Append(dTIFF);
	}
}

#if 0
static b3_bool GetIFW(
	void      *     TIFF,
	struct TagTIFF * DataTag)
{
	char  * Char  = (char *)TIFF;
	b3_bool image = false;
	long    value, size, index = 8, type, len, subtype, offset, i;
	time_t  tPoint;
	char    text[12];

	size  = b3Endian::b3Get32(&DataTag->Data[0]);
	Char += b3Endian::b3Get32(&DataTag->Data[1]);
	value = GetIntelLong(&Char[4]);

	b3LogTIFF("Imaging for Windows - additional tags\n");
	b3LogTIFF("%ld bits (%ld).\n", value == 0 ? 16L : 32L, value);

	do
	{
		type = GetIntelLong(&Char[index]);
		len  = GetIntelLong(&Char[index + 4]);

		// correct block type
		index += 8;
		b3LogTIFF("%8ld %8ld: ", type, len);

		switch (type)
		{
		case 2 :
		case 6 :
			// this is name block header
			strncpy(text, &Char[index], 8);
			text[8] = 0;
			if (value == 0)
			{
				len += 4;
			}

			// now name block data starts here
			b3LogTIFF("named block: %-8.8s", text);
			if (strcmp(text, "OiAnoDat") == 0)
			{
				long n, pos;

				if (!image)
				{
					pos = (value == 0 ? 4 : 8) + index + len;
					n   = (value == 0 ?
							GetIntelShort(&Char[index + len + 2]) :
							GetIntelLong(&Char[index + len + 4]));
					b3LogTIFF(" %ld pts.", n);
					for (i = 0; i < n; i++)
					{
						b3LogTIFF(" (%ld,%ld)",
							(long)GetIntelLong(&Char[pos]),
							(long)GetIntelLong(&Char[pos + 4]));
						pos += 8;
					}
				}
			}
			if (strcmp(text, "OiFilNam") == 0)
			{
				b3LogTIFF(" %s", &Char[index + len]);
			}
			if (strcmp(text, "OiDIB")    == 0)
			{
			}
			if (strcmp(text, "OiIndex")  == 0)
			{
				strncpy(text, &Char[index + len], 10);
				text[10] = 0;
				b3LogTIFF(" %s", text);
			}
			if (strcmp(text, "OiGroup")  == 0)
			{
				b3LogTIFF(" %s", &Char[index + len]);
			}
			if (strcmp(text, "OiAnText") == 0)
			{
				offset = (value + 1) << 3;
				b3LogTIFF(" %s", &Char[index + len + offset]);
			}
			b3LogTIFF("\n");
			len += GetIntelLong(&Char[index + 8]);
			break;

		case 5 :
			image = false;
			subtype = (value == 0 ?
					GetIntelShort(&Char[index]) :
					GetIntelLong(&Char[index]));
			switch (subtype)
			{
			case  1 :
				b3LogTIFF("image embedded  ");
				image = true;
				break;
			case  2 :
				b3LogTIFF("image reference ");
				image = true;
				break;
			case  3 :
				b3LogTIFF("straight line   ");
				break;
			case  4 :
				b3LogTIFF("freehand line   ");
				break;
			case  5 :
				b3LogTIFF("hollow rectangle");
				break;
			case  6 :
				b3LogTIFF("filled rectangle");
				break;
			case  7 :
				b3LogTIFF("typed text      ");
				break;
			case  8 :
				b3LogTIFF("text from file  ");
				break;
			case  9 :
				b3LogTIFF("text stamp      ");
				break;
			case 10 :
				b3LogTIFF("attach-a-note   ");
				break;
			case 12 :
				b3LogTIFF("form            ");
				break;

			default :
				b3LogTIFF("unknown subtype ");
			}
			tPoint = GetIntelLong(&Char[index + 104 + value * 8]);

			b3LogTIFF("      %s", ctime(&tPoint));
			break;

		default :
			b3LogTIFF("unknown type!\n");
			index = size;
			break;
		}
		index += len;
	}
	while (index < size);
	return true;
}
#endif

#if 0
static long WriteIFW(
	b3FileAbstract * out,
	void      *     ptr,
	struct TagTIFF * tag)
{
	char * data = (char *)ptr;
	long  count, size;

	count = out->b3Write(tag, sizeof(struct TagTIFF));
	size  = b3Endian::b3Get32(&tag->Data[0]);
	if (size > GetTIFFSize(tag))
	{
		data  += tag->Data[1];
		count += out->b3Write(data, size);
	}
	return count;
}
#endif

int b3TIFF_Entry::b3SortTags(
	const b3TIFF_Entry * a,
	const b3TIFF_Entry * b,
	const void     *     ptr B3_UNUSED)
{
	if (a->tag.Code < b->tag.Code)
	{
		return -1;
	}
	if (a->tag.Code > b->tag.Code)
	{
		return  1;
	}
	return 0;
}

void b3TIFF::b3TravRemIFW(
	b3Base<class T> * Head,
	b3Link<class T> * Node,
	void      *      ptr B3_UNUSED)
{
	b3TIFF_Entry * tag;

	switch (Node->b3GetClassType())
	{
	case CLASS_TIFF_TAG :
		tag = (b3TIFF_Entry *)Node;
		tag->b3RemoveIFW((b3Base<b3TIFF_Entry> *)Head);
		break;
	}
}

void b3TIFF::b3TravRemoveIFW()
{
	b3Traverse((b3Base<class T> *)&dirs, b3TravRemIFW, nullptr);
}

void b3TIFF::b3TravTIFF(
	b3Base<class T> * Head B3_UNUSED,
	b3Link<class T> * Node,
	void       *      ptr)
{
	b3TIFF    *    hTIFF;
	b3TIFF_Dir  *  dTIFF;
	b3TIFF_Entry * tTIFF;
	b3TIFF_Strip * sTIFF;
	long     *     offset = (long *)ptr;

	switch (Node->b3GetClassType())
	{
	// put correct CPU type into header
	case CLASS_TIFF_HEAD :
		hTIFF   = (b3TIFF *)Node;
		*offset = hTIFF->b3OrgTags(*offset);
		break;

	// sort directory structure (which contains tags) by type
	case CLASS_TIFF_DIR :
		dTIFF   = (b3TIFF_Dir *)Node;
		*offset = dTIFF->b3OrgTags(*offset);
		break;

	case CLASS_TIFF_STRIP :
		sTIFF   = (b3TIFF_Strip *)Node;
		*offset = sTIFF->b3OrgTags(*offset);
		break;

	case CLASS_TIFF_TAG :
		tTIFF   = (b3TIFF_Entry *)Node;
		*offset = tTIFF->b3OrgTags(*offset);
		break;
	}
}

void b3TIFF::b3TravOffset(
	b3Base<class T> * Head B3_UNUSED,
	b3Link<class T> * Node,
	void       *      ptr)
{
	b3TIFF    *    hTIFF;
	b3TIFF_Dir  *  dTIFF;
	b3TIFF_Entry * tTIFF;
	b3TIFF_Strip * sTIFF;
	long      *      offset = (long *)ptr;

	switch (Node->b3GetClassType())
	{
	case CLASS_TIFF_HEAD :
		hTIFF = (b3TIFF *)Node;
		hTIFF->b3OrgStrips();
		break;

	case CLASS_TIFF_DIR :
		dTIFF = (b3TIFF_Dir *)Node;
		dTIFF->b3OrgStrips();
		break;

	case CLASS_TIFF_TAG :
		tTIFF   = (b3TIFF_Entry *)Node;
		*offset = tTIFF->b3OrgStrips(*offset);
		break;

	case CLASS_TIFF_STRIP :
		sTIFF   = (b3TIFF_Strip *)Node;
		*offset = sTIFF->b3OrgStrips(*offset);
		break;
	}
}

void b3TIFF::b3Write(char * name)
{
	b3TIFF_Dir    *   dTIFF;
	b3TIFF_Entry   *  tTIFF;
	b3TIFF_Strip   *  sTIFF;
	b3File            out(name, B_WRITE);
	b3Base<b3TIFF>    root;
	long              act_offset = 0;

	// first remove containing TAF tags
	root.b3InitBase(CLASS_TIFF_HEAD);
	root.b3Append(this);

	// compute offsets for header, dirs, tags and data
	b3Traverse((b3Base<class T> *)&root, b3TravTIFF, &act_offset);
	b3LogTIFF("data offset: %ld\n", act_offset);

	// compute offsets for strips
	b3Traverse((b3Base<class T> *)&root, b3TravOffset, &act_offset);

	act_offset = 0;

	// writing TIFF header
	out.b3Write(&head, sizeof(head));
	b3LogTIFF("H:     %6ld - %6ld\n", offset, act_offset);
	act_offset += sizeof(struct HeaderTIFF);

	// writing dir and tag structure
	for (dTIFF  = dirs.First;
		dTIFF != nullptr;
		dTIFF  = dTIFF->Succ)
	{
		act_offset = dTIFF->b3WriteTags(&out, act_offset);
	}

	// writing tag data
	for (dTIFF  = dirs.First;
		dTIFF != nullptr;
		dTIFF  = dTIFF->Succ)
	{
		// writing tag buffers
		for (tTIFF  = dTIFF->b3FirstEntry();
			tTIFF != nullptr;
			tTIFF  = tTIFF->Succ)
		{
			act_offset = tTIFF->b3WriteData(&out, act_offset);
		}

		// writing strips
		for (sTIFF  = dTIFF->b3FirstStrip();
			sTIFF != nullptr;
			sTIFF  = sTIFF->Succ)
		{
			act_offset = sTIFF->b3WriteData(&out, act_offset);
		}
	}
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

#if 0

long b3Write(
	char * buffer_tiff,
	char * buffer_taf,
	long  sizeTIFF,
	long  sizeTAF,
	char * name)
{
	struct HeaderTIFF * header = (struct HeaderTIFF *)buffer_tiff;
	b3TIFF     *     newTIFF;
	b3TIFF_Dir   *   dirTIFF = null;
	b3Head           root;
	long               offset = 0;

	// get TIFF data structure (tags, stripes, etc. )
	newTIFF = new b3TIFF((struct HeaderTIFF *)buffer_tiff);
	if (newTIFF)
	{
		// firt remove containing TAF tags
		root.b3InitHead(CLASS_TIFF_HEAD);
		newTIFF->b3Node::b3Append(&root);
		newTIFF->b3Traverse(&root, travRemIFW, null);

		// then parse TAF file for tag inserting
#ifdef USE_TAF
		len = TAF_MAGIC_LEN(buffer_taf) + sizeof(long);
		buffer_taf += len;
		while ((code = b3Endian::b3Get32(buffer_taf)) != TAF_END)
		{
			// select new dir entry?
			if (code == TAF_SEPARATOR)
			{
				dirTIFF = (dirTIFF == null ?
						(struct DirTIFF *)newTIFF->dirs.First :
						(struct DirTIFF *)dirTIFF->node.Succ);
				buffer_taf += sizeof(long);
			}

			// allocate memory for tag structure to insert
			// in Blizzard III data structure
			tag     = (struct TagTIFF *)buffer_taf;
			tagTIFF = new b3TIFF_Entry;
			if (tagTIFF)
			{
				tagTIFF->node.ClassType = CLASS_TIFF_TAG;
				tagTIFF->node.Size      = sizeof(struct EntryTIFF);
				memcpy(&tagTIFF->tag, tag, sizeof(struct TagTIFF));
				size             = tagTIFF->tag.Data[0];
				buffer_taf      += sizeof(struct TagTIFF);
				tagTIFF->buffer  = buffer_taf;
				tagTIFF->size    = size;
				if (size > GetTIFFSize(&tagTIFF->tag))
				{
					buffer_taf += size;
				}
				BAppend(&dirTIFF->tags, &tagTIFF->node);
			}
		}
#endif

		newTIFF->b3Traverse(&root, travTIFF, &offset);
		b3LogTIFF("data offset: %ld\n", offset);

		newTIFF->b3Traverse(&root, travOffset, &offset);
		newTIFF->b3Write(name);
	}
	return 0;
}

#ifdef bla

static b3_bool CreateGrayTIFFPalette(
	struct Texture	*	Texture,
	struct HeaderTIFF	* TIFF,
	struct TagTIFF	*	DataTag)
{
	unsigned long * Palette, Color, i, Step, Colors, Value;

	Value = GetTIFFValue(TIFF, DataTag, 0);
	if (Value >= 2)
	{
		return true;
	}

	Colors = 1 << Texture->Planes;
	if (Colors > 256)
	{
		Texture->FileType = FT_ERR_HEADER;
		return (false);
	}
	Palette = (unsigned long *)AllocTextureMem(Texture, Colors * sizeof(long));
	if (Palette == null)
	{
		Texture->FileType = FT_ERR_MEM;
		return (false);
	}
	Texture->Palette = Palette;

	if (Value == 0L)
	{
		if (Colors == 2L)						/* white black*/
		{
			Palette[0] = 0x00ffffff;
			Palette[1] = 0x00000000;
			return (true);
		}
		Color	=  0x00ffffffL;
		Step	= -0x01010100L >> Texture->Planes;
	}
	else
	{
		if (Colors == 2L)						/* black white */
		{
			Palette[0] = 0x00000000;
			Palette[1] = 0x00ffffff;
			return (true);
		}
		Color	=  0L;
		Step	=  0x01010100L >> Texture->Planes;
	}

	for (i = 0; i < Colors; i++)					/* gray palette */
	{
		Palette[i] = Color;
		Color += Step;
	}
	return (true);
}

static b3_bool GetTIFFPalette(
	struct Texture * Texture,
	void      *     TIFF,
	struct TagTIFF * ColorTag)
{
	unsigned long * Palette;
	long           Colors, i, Index = 0;

	Colors  = b3Endian::b3Get32(&ColorTag->Data[0]) / 3;
	Palette = Texture->Palette = (unsigned long *)AllocTextureMem(Texture, Colors * 4);
	if (Palette == null)
	{
		Texture->FileType = FT_ERR_MEM;
		return (false);
	}

	for (i = 0; i < Colors; i++)		/* red */
	{
		Palette[i]  = (GetTIFFValue(TIFF, ColorTag, Index++) & 0xff00L) << 8 ;
	}
	for (i = 0; i < Colors; i++)		/* green */
	{
		Palette[i] |= (GetTIFFValue(TIFF, ColorTag, Index++) & 0xff00L);
	}
	for (i = 0; i < Colors; i++)		/* blue */
	{
		Palette[i] |= ((GetTIFFValue(TIFF, ColorTag, Index++) & 0xff00L) >> 8);
	}
	return (true);
}

static long UnCodeTIFF(
	struct Texture * Texture,
	void      *     TIFF,
	struct TagTIFF * OffsetTag,
	struct TagTIFF * SizeTag,
	long            RowsPerStrip,
	long            PixelSamples)
{
	unsigned long * LongData;
	b3_u08     *    Data;
	b3_u08     *    StripPtr;
	unsigned long   Bytes, Strip, Size, i, Value, OffsetSize;

	switch (Texture->Planes)
	{
	case  1 :
		Size = Texture->xSize + 7L & 0x7ffffff8;
		break;
	case  2 :
		Size = Texture->xSize + 3L & 0x7ffffffc;
		break;
	case  4 :
		Size = Texture->xSize + 1L & 0x7ffffffe;
		break;
	case  8 :
	default :
		Size = Texture->xSize;
		break;
	}
	Size *= Texture->ySize;
	if (PixelSamples == 3L)
	{
		Size *= 4L;
	}
	else if (PixelSamples != 1L)
	{
		Texture->FileType = FT_ERR_UNSUPP;
		return (0L);
	}
	Data = AllocTextureMem(Texture, Size);
	if (Data == null)
	{
		Texture->FileType = FT_ERR_MEM;
		return (0L);
	}
	Texture->Data = Data;
	OffsetSize    = b3Endian::b3Get32(&OffsetTag->Data[0]);

	switch (PixelSamples)
	{
	case 1:
		switch (Texture->Planes)
		{
		case 1 :
			for (Strip = 0; Strip < OffsetSize; Strip++)
			{
				Value     = GetTIFFValue(TIFF, OffsetTag, Strip);
				Bytes     = GetTIFFValue(TIFF, SizeTag,  Strip);
				StripPtr  = TIFF;
				StripPtr += Value;
				for (i = 0; i < Bytes; i++)
				{
					Data[0] = StripPtr[0];
					Data++;
					StripPtr++;
				}
			}
			return (ILBM);

		case 2 :
			for (Strip = 0; Strip < OffsetSize; Strip++)
			{
				Value     = GetTIFFValue(TIFF, OffsetTag, Strip);
				Bytes     = GetTIFFValue(TIFF, SizeTag,  Strip);
				StripPtr  = TIFF;
				StripPtr += Value;
				for (i = 0; i < Bytes; i++)
				{
					*Data++ = (StripPtr[0] & 0xc0 >> 3);
					*Data++ = (StripPtr[0] & 0x30 >> 2);
					*Data++ = (StripPtr[0] & 0x0c >> 1);
					*Data++ = (StripPtr[0] & 0x03);
					StripPtr++;
				}
			}
			return (PCX8);

		case 4 :
			for (Strip = 0; Strip < OffsetSize; Strip++)
			{
				Value     = GetTIFFValue(TIFF, OffsetTag, Strip);
				Bytes     = GetTIFFValue(TIFF, SizeTag,  Strip);
				StripPtr  = TIFF;
				StripPtr += Value;
				for (i = 0; i < Bytes; i++)
				{
					*Data++ = (StripPtr[0] & 0xf0 >> 4);
					*Data++ = (StripPtr[0] & 0x0f);
					StripPtr++;
				}
			}
			return (PCX8);

		case 8 :
			for (Strip = 0; Strip < OffsetSize; Strip++)
			{
				Value     = GetTIFFValue(TIFF, OffsetTag, Strip);
				Bytes     = GetTIFFValue(TIFF, SizeTag,  Strip);
				StripPtr  = TIFF;
				StripPtr += Value;
				for (i = 0; i < Bytes; i++)
				{
					Data[0] = StripPtr[0];
					Data++;
					StripPtr++;
				}
			}
			return (PCX8);
		}
		Texture->FileType = FT_ERR_UNSUPP;
		return (0L);

	case 3:
		if ((Texture->Planes / PixelSamples) != 8)
		{
			Texture->FileType = FT_ERR_UNSUPP;
			return (0L);
		}
		LongData = (unsigned long *)Data;
		for (Strip = 0; Strip < OffsetSize; Strip++)
		{
			Value     = GetTIFFValue(TIFF, OffsetTag, Strip);
			Bytes     = GetTIFFValue(TIFF, SizeTag,  Strip);
			StripPtr  = TIFF;
			StripPtr += Value;
			for (i = 0; i < Bytes; i += 3)
			{
				Value  = (unsigned long) * StripPtr++ << 16;
				Value |= (unsigned long) * StripPtr++ <<  8;
				Value |= (unsigned long) * StripPtr++;
				LongData[0] = Value;
				LongData++;
			}
		}
		return (RGB8);
	}
	Texture->FileType = FT_ERR_UNSUPP;
	return (0L);
}

long ParseTIFF(
	const b3FileAbstract * out,
	struct HeaderTIFF  *  TIFF,
	unsigned long         Size)
{
	long             offset, i, Tags, Compression = 0;
	long             PixelSamples = 0xabadcafe;
	struct TagTIFF * ThisTag;
	b3_u08     *     Data;
	b3_u16     *     Shorts;
	unsigned long    sep = TAF_SEPARATOR;

	offset = TIFF->FirstTag;

	while (offset != 0)
	{
		out->b3Write(&sep, sizeof(sep));
		b3LogTIFF("new IFD at offset $%08lx\n", offset);

		Data   = (b3_u08 *)TIFF;
		Data  += offset;
		Shorts = (b3_u16 *)Data;
		Tags   = b3Endian::b3Get16(Shorts);
		Data  += 2;

		b3LogTIFF("%ld Tags at $%lx\n", Tags, Shorts);
		if (Tags > 0)
		{
			b3LogTIFF(" i type mode  data[0]  data[1] - mode string\n");
		}

		for (i = 0; i < Tags; i++)
		{
			ThisTag = (struct TagTIFF *)Data;
			b3LogTIFF("%2ld %4x %4x %08lx %08lx - %s\n", i + 1,
				ThisTag->Code & 0xffffL,
				ThisTag->Type & 0xffffL,
				ThisTag->Data[0], ThisTag->Data[1],
				GetModeString(ThisTag->Type & 0xffffL));

			switch (ThisTag->Code)
			{
			case TIFF_IFW1 :
				GetIFW(TIFF, ThisTag);
			case TIFF_IFW2 :
				WriteIFW(out, TIFF, ThisTag);
				break;
			}

			Data += sizeof(struct TagTIFF);
		}

		offset = b3Endian::b3Get32(Data);
	}
	return 0;
}

#endif

#endif
