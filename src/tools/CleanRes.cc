/*
**
**	$Filename:	CleanRes.cc $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Cleaning up resource include
**
**	(C) Copyright 2002  Steffen A. Mork
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
#include "blz3/base/b3List.h"

/*************************************************************************
**                                                                      **
**                        Typedefs and defines                          **
**                                                                      **
*************************************************************************/

static struct b3DefineType
{
	char *m_Type;
	int   m_Prio;
	int   m_Start;
	int   m_Last;
} define_types[] =
	{
		{ "IDP_OLE_INIT_FAILED",  1, 101 },
		{ "IDR_MAINFRAME",  1,   110 },
		{ "IDR_",  2,   111 },
		{ "IDB_",  3,   400 },
		{ "IDM_",  4,   500 },
		{ "IDD_",  5,   600 },
		{ "IDI_",  6,  1001 },
		{ "ID_",   7, 10000 },
		{ "IDC_",  8, 32000 },
		{ "IDS_",  9, 60000 },
		{ "_APS", 10,     0 }
	};

#define TYPE_MAX ((int)(sizeof(define_types) / sizeof(b3DefineType)))

enum b3DefineMode
{
	DM_START,
	DM_DEFINE,
	DM_NOP,
	DM_END
};

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

class b3Define : public b3Link<b3Define>
{
	FILE         *m_File;
	char          m_Define[1024];
	int           m_Value;
	int           m_Type;
	b3DefineMode  m_Mode;

public:
	b3Define(const b3_bool start) : b3Link<b3Define>(sizeof(b3Define))
	{
		m_Mode = (start ? DM_START : DM_END);
		m_File = stdout;
	}

	b3Define(const char *define,const int value) : b3Link<b3Define>(sizeof(b3Define))
	{
		strlcpy (m_Define,define,sizeof(m_Define));
		m_Value = value;
		m_File  = stdout;
		m_Type  = b3FindDefineType();
		m_Mode  = DM_DEFINE;
		if ((m_Type < TYPE_MAX) && (define_types[m_Type].m_Start == 0))
		{
			m_Mode = DM_NOP;
		}
	}

	static int b3GetDefineTypesLastValue(const char *define)
	{
		int type = b3FindDefineType(define);

		return type < TYPE_MAX ? define_types[type].m_Last : 0;
	}

	void b3Print()
	{
		switch(m_Mode)
		{
		case DM_START:
			fprintf(m_File,"//{{NO_DEPENDENCIES}}\r\n");
			fprintf(m_File,"// Microsoft Developer Studio generated include file.\r\n");
			fprintf(m_File,"// Used by AppLines.rc\r\n");
			fprintf(m_File,"//\r\n");
			break;
		case DM_DEFINE:
			fprintf(m_File,"#define %-31s %d\r\n",m_Define,m_Value);
			break;
		case DM_END:
			fprintf(m_File,"\r\n");
			fprintf(m_File,"// Next default values for new objects\r\n");
			fprintf(m_File,"// \r\n");
			fprintf(m_File,"#ifdef APSTUDIO_INVOKED\r\n");
			fprintf(m_File,"#ifndef APSTUDIO_READONLY_SYMBOLS\r\n");
			fprintf(m_File,"#define _APS_3D_CONTROLS                     1\r\n");
			fprintf(m_File,"#define _APS_NEXT_RESOURCE_VALUE        %d\r\n",b3GetDefineTypesLastValue("IDR_"));
			fprintf(m_File,"#define _APS_NEXT_COMMAND_VALUE         %d\r\n",b3GetDefineTypesLastValue("ID_"));
			fprintf(m_File,"#define _APS_NEXT_CONTROL_VALUE         %d\r\n",b3GetDefineTypesLastValue("IDC_"));
			fprintf(m_File,"#define _APS_NEXT_SYMED_VALUE           %d\r\n",b3GetDefineTypesLastValue("IDS_"));
			fprintf(m_File,"#endif\r\n");
			fprintf(m_File,"#endif\r\n");
			break;

		default:
			break;
		}
	}

	void b3Renumber(int &last)
	{
		if (m_Mode == DM_DEFINE)
		{
			if (Prev->m_Mode != DM_DEFINE)
			{
				last = define_types[m_Type].m_Start;
			}
			else
			{
				if (Prev->m_Type != m_Type)
				{
					last = define_types[m_Type].m_Start;
				}
			}
			m_Value = last;
			last++;
			if (m_Type < TYPE_MAX)
			{
				define_types[m_Type].m_Last = last;
			}
		}
	}

	int b3FindDefineType()
	{
		return b3FindDefineType(m_Define);
	}

	static int b3FindDefineType(const char *define)
	{
		int i,len;

		for (i = 0;i < TYPE_MAX;i++)
		{
			len = strlen(define_types[i].m_Type);
			if (strncmp(define,define_types[i].m_Type,len) == 0)
			{
				return i;
			}
		}
		return i;
	}

	static int b3SortFunc(b3Define *a,b3Define *b,const void *ptr)
	{
		int a_type = a->m_Type;
		int b_type = b->m_Type;
		int diff;

		if ((a->m_Mode != DM_DEFINE) || (b->m_Mode != DM_DEFINE))
		{
			return a->m_Mode - b->m_Mode;
		}

		if ((a_type < TYPE_MAX) && (b_type < TYPE_MAX))
		{
			diff = define_types[a_type].m_Prio - define_types[b_type].m_Prio;
			if (diff == 0)
			{
				diff = strcmp(
						   &a->m_Define[strlen(define_types[a_type].m_Type)],
						   &b->m_Define[strlen(define_types[b_type].m_Type)]);
			}
		}
		else
		{
			diff = a_type - b_type;
			if (diff == 0)
			{
				diff = strcmp(a->m_Define,b->m_Define);
			}
		}
		return diff;
	}
};

static b3_bool b3ReadResource(const char *filename,b3Base<b3Define> &defines)
{
	FILE *in;
	char  line[2048];
	char  define[1024];
	int   value;

	in = fopen(filename,B3_TREAD);
	if (in != null)
	{
		defines.b3Append(new b3Define(true));
		while (fgets(line,sizeof(line),in) != null)
		{
			if (sscanf(line,"#define %s %d\n",define,&value) == 2)
			{
				defines.b3Append(new b3Define(define,value));
			}
		}
		defines.b3Append(new b3Define(false));

		fclose(in);
	}
	return in != null;
}

int main(int argc,char *argv[])
{
	b3Base<b3Define>  defines;
	b3Define         *define;
	int               last;

	for (int i = 1;i < argc;i++)
	{
		for (int k = 0;k < TYPE_MAX;k++)
		{
			define_types[k].m_Last = 0;
		}
		b3ReadResource(argv[i],defines);
		defines.b3Sort(&b3Define::b3SortFunc);
		B3_FOR_BASE(&defines,define)
		{
			define->b3Renumber(last);
			define->b3Print();
		}
		defines.b3Free();
	}
	return EXIT_SUCCESS;
}
