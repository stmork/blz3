#include "blz3/system/b3Log.h"
#include "blz3/system/b3Mem.h"

void b3TestMem()
{
	void  *ptr1,*ptr2;
	b3Mem  mem;

	ptr1 = mem.b3Alloc (1000);
	ptr2 = mem.b3Alloc (1000);
	b3PrintF (B3LOG_NORMAL,"ptr1: %8lx\n",ptr1);
	b3PrintF (B3LOG_NORMAL,"ptr2: %8lx\n",ptr2);
	mem.b3Free(ptr1);
	b3PrintF (B3LOG_NORMAL,"ptr1 freed...\n");
	mem.b3Free(NULL);
	b3PrintF (B3LOG_NORMAL,"NULL-Pointer freed...\n");
	mem.b3Free();
	b3PrintF (B3LOG_NORMAL,"whole node freed...\n");
}
