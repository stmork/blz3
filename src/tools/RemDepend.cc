
#include "blz3/system/b3File.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>

static b3_bool DoRemDepend(const char *filename)
{
	b3File   in;
	b3_size  size_in;
	b3_size  size_out = 0;
	char    *buffer,*pos;
	b3_size  i;

	buffer = (char *)in.b3ReadBuffer(filename,size_in);
	if (buffer != null)
	{
		pos = strstr(buffer,"# DO NOT DELETE");

		// Remove trailing spaces
		if (pos != null)
		{
			size_out = (off_t)pos - (off_t)buffer;
		}
		else
		{
			size_out = size_in;
		}

		// Remove trailing white spaces
		while ((size_out > 0) && isspace(buffer[size_out-1]))
		{
			size_out--;
		}

		// Append a line feed
		buffer[size_out] = 0;
		strcat(buffer,"\n");
		size_out = strlen(buffer);
	}
	else
	{
		fprintf (stderr,"Cannot allocate buffer for %s.\n",filename);
	}

	if (size_out > 0)
	{
//		printf ("%d\n",size_out);
		truncate (filename,size_out);
	}
	else
	{
		fprintf (stderr,"Cannot open %s for reading.\n",filename);
	}
	return (buffer != null);
}

int main(int argc, char *argv[])
{
	b3_index i;

	for (i = 1;i < argc;i++)
	{
		DoRemDepend(argv[i]);
	}
	return 0;
}
