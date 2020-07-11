/*
**
**	$Filename:	BodThumb.cc $
**	$Release:	Dortmund 2005 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Creating thumb images of Blizzard III objects
**
**	(C) Copyright 2005  Steffen A. Mork
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

#include "blz3/system/b3File.h"
#include "blz3/raytrace/b3BBox.h"
#include "blz3/raytrace/b3Factory.h"

/*************************************************************************
**                                                                      **
**                        routines                                      **
**                                                                      **
*************************************************************************/

class b3BBoxThumb
{
	b3Scene * m_Scene;
	b3BBox * m_BBox;
	b3World  m_World;
	b3Path   m_ObjectName;
	b3Path   m_ImageName;

public:
	b3BBoxThumb(const char * filename) : m_ObjectName(filename)
	{
		m_ImageName.b3RemoveExt(filename);
		m_ImageName.b3Append(".tga");
	}

	void b3Read()
	{
		b3File  file(m_ObjectName, B_READ);

		if(m_World.b3Read(&file) == B3_WORLD_OK)
		{
			b3PrintF(B3LOG_NORMAL, "%s -> %s\n",
				(const char *)m_ObjectName,
				(const char *)m_ImageName);

			b3Base<b3Item> * depot = m_World.b3GetHead();
			b3Base<b3Item>  base;
			b3_u32          level;

			m_BBox  = (b3BBox *)m_World.b3GetFirst();
			level = m_BBox->b3GetType();
			base.b3InitBase(depot->b3GetClass());

			b3BBox::b3Reorg(depot, &base, level, 1);

			m_Scene = b3Factory::b3CreateBBox(m_BBox, TRACEPHOTO_MORK);
			m_Scene->b3GetSuperSample();
		}
	}

	virtual ~b3BBoxThumb()
	{
		delete m_Scene;
	}

	void b3Create()
	{
		b3Display * display;
		b3Tx       tx;

		tx.b3AllocTx(120, 120, 24);
		display = new b3Display(&tx);
		m_Scene->b3Raytrace(display);
		tx.b3SaveImage(m_ImageName);
		delete display;
	}
};

int main(int argc, char * argv[])
{
	int   i;
	char * BLZ3_TEXTURES    = getenv("BLZ3_TEXTURES");
	char * HOME             = getenv("HOME");
	b3Path                textures;
	b3Path                pictures;

	if(HOME != null)
	{
		b3Dir::b3LinkFileName(textures, HOME, "Blizzard/Textures");
		b3Dir::b3LinkFileName(pictures, HOME, "Blizzard/Pictures");

		b3Scene::m_TexturePool.b3AddPath(textures);
		if(BLZ3_TEXTURES != null)
		{
			b3Scene::m_TexturePool.b3AddPath(BLZ3_TEXTURES);
		}
		b3Scene::m_TexturePool.b3AddPath(pictures);
	}
	b3RaytracingItems::b3Register();
	for(i = 1; i < argc; i++)
	{
		b3BBoxThumb thumb(argv[i]);

		try
		{
			thumb.b3Read();
			thumb.b3Create();
		}
		catch(b3WorldException & w)
		{
			b3PrintF(B3LOG_NORMAL, "Error parsing %s\n", argv[i]);
			b3PrintF(B3LOG_NORMAL, "Error code: %d\n", w.b3GetError());
			b3PrintF(B3LOG_NORMAL, "Error msg:  %s\n", w.b3GetErrorMsg());
		}
		catch(b3FileException & f)
		{
			b3PrintF(B3LOG_NORMAL, "File IO error using %s\n", argv[i]);
			b3PrintF(B3LOG_NORMAL, "Error code: %d\n", f.b3GetError());
			b3PrintF(B3LOG_NORMAL, "Error msg:  %s\n", f.b3GetErrorMsg());
		}
		catch(b3TxException & t)
		{
			b3PrintF(B3LOG_NORMAL, "Image error using %s\n", argv[i]);
			b3PrintF(B3LOG_NORMAL, "Error code: %d\n", t.b3GetError());
			b3PrintF(B3LOG_NORMAL, "Error msg:  %s\n", t.b3GetErrorMsg());
		}
	}
}
