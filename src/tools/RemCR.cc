/*
**
**      $Filename:      b3RemDepend.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Author$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - removing dependencies from makefile
**
**      (C) Copyright 2001  Steffen A. Mork
**          All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/system/b3File.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2001/07/08 12:30:07  sm
**	- New tool to remove nasty CR/LF from Windoze.
**	- Removing some nasty CR/LF with that new tool.
**
**	
*/

/*************************************************************************
**                                                                      **
**                        application routines                          **
**                                                                      **
*************************************************************************/

static b3_bool DoRemCR(const char *filename)
{
	b3File   in,out;
	b3_size  size_in;
	b3_size  size_out = 0;
	char    *buffer,*pos;
	b3_size  i;

	buffer = (char *)in.b3ReadBuffer(filename,size_in);
	if (buffer != null)
	{
		// Remove 0x0d
		for (i = 0;i < size_in;i++)
		{
			if (buffer[i] != 13) buffer[size_out++] = buffer[i];
		}

		// If the new size is smaller there were some 0x0d's
		if (size_out < size_in)
		{
			if (out.b3Open(filename,B_WRITE))
			{
				if (out.b3Write(buffer,size_out) < size_out)
				{
					fprintf (stderr,"Cannot write complete file %s.\n",filename);
				}
				out.b3Close();
			}
			else
			{
				fprintf (stderr,"Cannot open file for writing %s.\n",filename);
			}
		}
	}
	else
	{
		fprintf (stderr,"Cannot allocate buffer for %s.\n",filename);
	}

	return (buffer != null);
}

int main(int argc, char *argv[])
{
	b3_index i;

	for (i = 1;i < argc;i++)
	{
		DoRemCR(argv[i]);
	}
	return 0;
}
