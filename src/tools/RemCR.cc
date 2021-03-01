/*
**
**	$Filename:	b3RemCR.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Converting Windows style files to Un*x files.
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/b3Config.h"
#include "blz3/system/b3File.h"

/*************************************************************************
**                                                                      **
**                        application routines                          **
**                                                                      **
*************************************************************************/

static b3_bool DoRemCR(const char * filename)
{
	b3File   in, out;
	b3_size  size_in;
	b3_size  size_out = 0;
	char  *  buffer;
	b3_size  i;

	buffer = (char *)in.b3ReadBuffer(filename, size_in);
	if (buffer != nullptr)
	{
		// Remove 0x0d
		for (i = 0; i < size_in; i++)
		{
			if (buffer[i] != 13)
			{
				buffer[size_out++] = buffer[i];
			}
		}

		// If the new size is smaller there were some 0x0d's
		if (size_out < size_in)
		{
			if (out.b3Open(filename, B_WRITE))
			{
				if (out.b3Write(buffer, size_out) < size_out)
				{
					fprintf(stderr, "Cannot write complete file %s.\n", filename);
				}
				out.b3Close();
			}
			else
			{
				fprintf(stderr, "Cannot open file for writing %s.\n", filename);
			}
		}
	}
	else
	{
		fprintf(stderr, "Cannot allocate buffer for %s.\n", filename);
	}

	return (buffer != nullptr);
}

int main(int argc, char * argv[])
{
	b3_index i;

	for (i = 1; i < argc; i++)
	{
		DoRemCR(argv[i]);
	}
	return 0;
}
