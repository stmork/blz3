#include "blz3/system/b3Log.h"
#include "blz3/system/b3Dir.h"

void b3TestDir()
{
	b3Dir        dir;
	b3Path       name;
	b3_path_type code;

	dir.b3OpenDir(".");
	name.b3Empty();
	while ((code = dir.b3DirNext(name)) != B3_NOT_EXISTANT)
	{
		switch (code)
		{
			case B3_TYPE_FILE :
				b3PrintF (B3LOG_NORMAL,"f: %s\n",(const char *)name);
				break;
			case B3_TYPE_DIR :
				b3PrintF (B3LOG_NORMAL,"d: %s\n",(const char *)name);
				break;
			default :
				b3PrintF (B3LOG_NORMAL,"?: %s\n",(const char *)name);
				break;
		}
		name.b3Empty();
	}
	dir.b3CloseDir ();
}
