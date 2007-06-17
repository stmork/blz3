/*
**
**      $Filename:      TestUtil.cc $
**      $Release:       Dortmund 2002 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Test routines for b3Hash
**
**      (C) Copyright 2002  Steffen A. Mork
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

#include "blz3/base/b3Hash.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.6  2005/05/31 19:06:24  sm
**	- Hash test fix.
**
**	Revision 1.5  2004/11/29 09:58:01  smork
**	- Changed exit states to correct defines.
**	- Added switch for disabling VBO in OpenGL renderer.
**	- Added switches for logging level in OpenGL renderer as in brt3.
**	
**	Revision 1.4  2002/08/09 13:20:20  sm
**	- b3Mem::b3Realloc was a mess! Now fixed to have the same
**	  behaviour on all platforms. The Windows method ::GlobalReAlloc
**	  seems to be broken:-(
**	- Introduced b3DirAbstract and b3PathAbstract classes
**	
**	Revision 1.3  2002/08/07 12:38:43  sm
**	- Modified exception definition. Exceptions are identified with
**	  a three character code to unify error codes. This is necessary
**	  to convert error codes into error messages inside applications.
**	- Added some additional b3Hash methods.
**	- Added -Wall compiler option to all C++ files.
**	- Removed some compiler warnings.
**	
**	Revision 1.2  2002/08/06 16:16:59  sm
**	- Added support for custom hash functions.
**	
**	Revision 1.1  2002/08/06 15:54:12  sm
**	- Introduced b3HashMap<key,object> class incl. test prog.
**	- Found some source files without header.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        implementation                                **
**                                                                      **
*************************************************************************/

static int b3ArraySorter(int *a,int *b)
{
	return *a - *b;
}

static b3_hash b3HashFunc(const int &key)
{
	return (key >> 1) % 11;
}

static void b3Dump(b3HashMap<int,char *> &map,const char *title)
{
	b3Array<int>    *indices = map.b3GetKeys();
	b3Array<char *> *args    = map.b3GetObjects();
	int              i;
	
	b3PrintF(B3LOG_NORMAL,"******************** %s\n",title);
	b3PrintF(B3LOG_NORMAL,"hash size:   %d\n",map.b3GetCount());

	if ((indices != null) && (args != null))
	{
		b3PrintF(B3LOG_NORMAL,"key size:    %d\n",indices->b3GetCount());
		b3PrintF(B3LOG_NORMAL,"object size: %d\n",args->b3GetCount());

		// If we would sort here the reference to the object array
		// would get lost:-(
		b3PrintF(B3LOG_NORMAL,"Dump by both arrays...\n");
		for (i = 0;i < indices->b3GetCount();i++)
		{
			b3PrintF(B3LOG_NORMAL,"%3d: %d -> %s\n",i,
				(*indices)[i],(*args)[i]);
		}

		// Sort here to hold reference from key to object right
		indices->b3Sort(&b3ArraySorter);
		b3PrintF(B3LOG_NORMAL,"Dump by array/find...\n");
		for (i = 0;i < indices->b3GetCount();i++)
		{
			int key = (*indices)[i];

			b3PrintF(B3LOG_NORMAL,"%3d: %d -> %s\n",i,
				key,*map.b3Find(key));
		}
		delete indices;
		delete args;
	}
}

int main(int argc,char *argv[])
{
	b3HashMap<int,char *>  hash;
	b3_index               i;

	hash.b3SetHashFunc(&b3HashFunc);
	for (i = 0;i < argc;i++)
	{
		hash.b3Add(i,argv[i]);
	}
	b3Dump(hash,"Init");
	
	hash.b3Remove(0);
	b3Dump(hash,"Remove");

	hash.b3Replace(0,argv[0]);
	b3Dump(hash,"Replace 1");

	hash.b3Replace(0,"Test replace");
	b3Dump(hash,"Replace 2");

	try
	{
		hash.b3Add(0,"Exception");
	}
	catch(b3HashException &e)
	{
		b3PrintF(B3LOG_NORMAL,"Exception caught correctly.\n");
	}
	b3Dump(hash,"Add with exception");

	hash.b3Clear();
	b3Dump(hash,"Clear");

	b3HashContainer<int,char *> *container = new b3HashContainer<int,char *>[argc];

	for (i = 0;i < argc;i++)
	{
		container[i].m_Key    = i;
		container[i].m_Object = argv[i];
	}	
	hash.b3Init(container,argc);
	b3Dump(hash,"Container init");

	delete [] container;
	return EXIT_SUCCESS;
}
