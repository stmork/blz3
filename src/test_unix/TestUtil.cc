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
				key,map.b3Find(key));
		}
		delete indices;
		delete args;
	}
}

int main(int argc,char *argv[])
{
	b3HashMap<int,char *>  hash;
	b3_index               i;

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
	catch(b3HashException *e)
	{
		b3PrintF(B3LOG_NORMAL,"Exception caught correctly.\n");
	}
	b3Dump(hash,"Add with exception");

	hash.b3Clear();
	b3Dump(hash,"Clear");

	return 0;
}
