/*
**
**      $Filename:	scale.cc $
**      $Release:	Dortmund 2003 $
**      $Revision$
**      $Date$
**      $Author$
**      $Developer:	Steffen A. Mork $
**
**      Blizzard III - Preparing images for home page gallery
**
**      (C) Copyright 2003  Steffen A. Mork
**          All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/b3Config.h"
#include "blz3/system/b3Dir.h"
#include "blz3/system/b3File.h"
#include "blz3/base/b3Array.h"
#include "blz3/base/b3FileList.h"
#include "blz3/image/b3Tx.h"

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

#define DEST_IMG_SIZE 100.0f
#define ROW_SIZE        4
#define TITLE         "Bildersammlung";

class b3Site
{
	const char  * m_Title;
	b3Path        m_Source;
	b3Path        m_Dest;
	b3Path        m_Small;
	b3Path        m_Normal;
	b3FileList    m_List;
	int           m_Num;

public:
	b3Site(const char * source, const char * dest)
		: m_Source(source), m_Dest(dest)
	{
		b3Dir::b3LinkFileName(m_Small, dest, "small");
		b3Dir::b3LinkFileName(m_Normal, dest, "normal");

		b3Dir::b3MkDir(m_Dest);
		b3Dir::b3MkDir(m_Small);
		b3Dir::b3MkDir(m_Normal);

		m_List.b3CreateList(source);
		m_List.b3Sort();

		m_Num = 0;
		m_Title = TITLE;
	}

	void b3Dump()
	{
		b3FileEntry * entry;
		b3Path       index, name;
		FILE    *    file;
		b3_bool      row;

		for(entry = m_List.b3First(); entry != null; entry = entry->Succ)
		{
			b3LoadImage(entry);
		}

		b3Path::b3LinkFileName(index, m_Dest, "index.html");

		file = fopen(index, "wt");
		if(file != null)
		{
			b3PrintF(B3LOG_NORMAL, "Writing %s\n", (const char *)index);
			fprintf(file,
				"<html>\n"
				"<head>\n"
				"<title>%s</title>\n"
				"</head>\n"
				"\n"
				"<body background=\"/sm/pictures/MORKNetBack.JPG\">\n", m_Title);
			fprintf(file,
				"</body>\n"
				"</html>\n");
			fprintf(file,
				"<table><tr>\n"
				"<td width=\"200\" valign=\"top\">\n"
				"<img SRC=\"/sm/pictures/HOME.gif\" ALT=\"[MORKNet Logo]\" HEIGHT=\"150\" WIDTH=\"150\">\n"
				"</td>\n"
				"\n"
				"<td width=\"500\" valign=\"top\">\n");

			fprintf(file,
				"<center>\n"
				"<h1>%s</h1>\n"
				"</center>\n", m_Title);
			fprintf(file, "<table>\n");
			for(int i = 0; i < m_Num; i++)
			{
				row = (i % ROW_SIZE) == 0;
				b3ImgName(name, i);
				if(row)
				{
					if(i > 0)
					{
						fprintf(file, "</tr>\n");
					}
					fprintf(file, "<tr>\n");
				}
				fprintf(file,
					"<td align=\"center\" valign=\"center\">\n"
					"  <a href=\"normal/%s\"><img src=\"small/%s\"></a>\n"
					"</td>\n",
					(const char *)name, (const char *)name);
			}
			fprintf(file, "</tr>\n</table>\n");

			fprintf(file,
				"</td></tr></table>\n");
			fclose(file);
		}
		else
		{
			b3PrintF(B3LOG_NORMAL, "Cannot write %s\n", (const char *)index);
		}
	}

	void setTitle(const char * title)
	{
		m_Title = title;
	}

private:
	static inline void b3ImgName(char * name, int num)
	{
		snprintf(name, B3_FILESTRINGLEN, "img_%04d.%s", num, b3Tx::b3GetExt(FT_JPEG));
	}

	b3_bool b3CopyImage(b3Tx * image, const char * full_normal)
	{
		const char * source = image->b3Name();
		b3Path  ext;

		ext.b3ExtractExt(source);
		if(b3Tx::b3GetFileType(ext) == FT_JPEG)
		{
			b3File  src;
			b3_u08 * buffer;
			b3_size size;

			buffer = src.b3ReadBuffer(source, size);
			if(buffer != null)
			{
				b3File  dst(full_normal, B_WRITE);

				dst.b3Write(buffer, size);
			}
			else
			{
				return false;
			}
			b3PrintF(B3LOG_NORMAL, "   %s (copied %d bytes)\n", (const char *)full_normal, size);
		}
		else
		{
#ifdef HAVE_LIBJPEG
			if(image->b3SaveJPEG(full_normal) != B3_OK)
			{
				return false;
			}
#else
			return false;
#endif
			b3PrintF(B3LOG_NORMAL, "   %s (save)\n", (const char *)full_normal);
		}
		return true;
	}

	b3_bool b3LoadImage(b3FileEntry * entry)
	{
		b3Path  source(m_Source);
		b3Tx    normal, small;
		b3Path  name, full_small, full_normal;
		b3_res  xSize, ySize;
		b3_f64  scale;
		b3_bool result = false;

		source.b3LinkFileName(entry->b3Name(), null);
		b3PrintF(B3LOG_NORMAL, "F: %s\n", (const char *)source);
		if(normal.b3LoadImage(source) == B3_OK)
		{
			b3ImgName(name, m_Num);
			b3Path::b3LinkFileName(full_normal, m_Normal, name);

			if(b3CopyImage(&normal, full_normal))
			{
				b3Path::b3LinkFileName(full_small, m_Small, name);
				scale = DEST_IMG_SIZE / (normal.xSize < normal.ySize ?
						normal.ySize :
						normal.xSize);
				xSize = (b3_res)(scale * normal.xSize);
				ySize = (b3_res)(scale * normal.ySize);

				b3PrintF(B3LOG_NORMAL, "   %s - %dx%d - %3.3f\n", (const char *)full_small, xSize, ySize, scale);
				if(small.b3AllocTx(xSize, ySize, 24))
				{
					try
					{
						small.b3ScaleToGrey(&normal);
#ifdef HAVE_LIBJPEG
						result = small.b3SaveJPEG(full_small) == B3_OK;
#else
						result = false;
#endif
					}
					catch(b3TxException & t)
					{
						b3PrintF(B3LOG_NORMAL, "Error code: %d\n", t.b3GetError());
						b3PrintF(B3LOG_NORMAL, "Error msg:  %s\n", t.b3GetErrorMsg());
					}
				}
				else
				{
					b3PrintF(B3LOG_NORMAL, "Cannot allocate small image!\n");
				}
			}
			else
			{
				b3PrintF(B3LOG_NORMAL, "Cannot copy image!\n");
			}
		}
		else
		{
			b3PrintF(B3LOG_NORMAL, "Cannot load image!\n");
		}

		if(result)
		{
			m_Num++;
		}
		else
		{
			remove(full_small);
			remove(full_normal);
		}
		return result;
	}
};

int main(int argc, char * argv[])
{
	b3Path      dir;
	const char * title = TITLE;

	switch(argc)
	{
	default:
	case 3:
		title = argv[2];
	case 2:
		dir = argv[1];
		break;

	case 1:
		dir = ".";
		break;
	}

	b3Site site(dir, "/tmp/im");
	site.setTitle(title);
	site.b3Dump();

	return EXIT_SUCCESS;
}
