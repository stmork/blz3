/*
**
**	$Filename:	b3TxPool.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Texture pool
**
**	(C) Copyright 2001  Steffen A. Mork
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

#include "blz3/image/b3TxPool.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.21  2002/02/17 21:25:06  sm
**	- Introduced CSG
**	  o Heavily reorganized shape inheritance
**	  o New file b3CSGShape added
**
**	Revision 1.20  2002/01/01 13:50:22  sm
**	- Fixed some memory leaks:
**	  o concerning triangle shape and derived spline shapes
**	  o concerning image pool handling. Images with windows
**	    path weren't found inside the image pool requesting
**	    further image load.
**	
**	Revision 1.19  2001/12/01 17:48:42  sm
**	- Added raytraced image saving
**	- Added texture search path configuration
**	- Always drawing fulcrum and view volume. The
**	  depth buffer problem persists
**	
**	Revision 1.18  2001/11/11 14:07:15  sm
**	- Adjusted b3Path to Un*x port.
**	
**	Revision 1.17  2001/11/11 11:51:21  sm
**	- Added image select feature
**	- Cleaned up scene dialog (Now ready to improve it)
**	- some b3Path fixes
**	
**	Revision 1.16  2001/11/01 09:43:11  sm
**	- Some image logging cleanups.
**	- Texture preparing now in b3Prepare().
**	- Done some minor fixes.
**	
**	Revision 1.15  2001/10/26 18:37:14  sm
**	- Creating search path support
**	- Splitting image pool support and image loading into
**	  their own area.
**	- Fixed JPEG to support b3Tx::b3AllocTx()
**	
**	Revision 1.14  2001/10/25 17:41:32  sm
**	- Documenting stencils
**	- Cleaning up image parsing routines with using exceptions.
**	- Added bump mapping
**	
**	Revision 1.13  2001/10/23 15:50:31  sm
**	- Now parsing PCX4 correctly
**	- Found TGA parsing bug.
**	- Correcting path following behaviour.
**	
**	Revision 1.12  2001/10/19 14:46:57  sm
**	- Rotation spline shape bug found.
**	- Major optimizations done.
**	- Cleanups
**	
**	Revision 1.11  2001/10/18 14:48:26  sm
**	- Fixing refracting problem on some scenes with glasses.
**	- Fixing overlighting problem when using Mork shading.
**	- Finxing some memory leaks (espacially b3TriangleRefs)
**	- Adding texture support to conditions (stencil mapping).
**	  Now conditions are ready to work compatible with
**	  Blizzard II.
**	
**	Revision 1.10  2001/10/17 14:46:02  sm
**	- Adding triangle support.
**	- Renaming b3TriangleShape into b3Triangles and introducing
**	  new b3TriangleShape as base class. This results in
**	  source file renaming, too.
**	- Fixing soft shadow bug.
**	- Only scene loading background image when activated.
**	- Fixing LDC spline initialization.
**	- Converting Windows paths into right paths on Un*x
**	
**	Revision 1.9  2001/10/15 14:45:08  sm
**	- Materials are accessing textures now.
**	- Created image viewer "bimg3"
**	
**	Revision 1.8  2001/10/13 15:48:53  sm
**	- Minor image loading corrections
**	
**	Revision 1.7  2001/10/13 15:35:32  sm
**	- Adding further image file format support.
**	
**	Revision 1.6  2001/10/13 09:20:49  sm
**	- Adding multi image file format support
**	
**	Revision 1.5  2001/10/11 16:06:33  sm
**	- Cleaning up b3BSpline with including isolated methods.
**	- Cleaning up endian conversion routines and collecting into
**	  b3Endian
**	- Cleaning up some datatypes for proper display in Together.
**	
**	Revision 1.4  2001/10/10 17:52:24  sm
**	- Texture loading (only reading into memory) running.
**	- Raytracing without OpenGL must be possible!
**	
**	Revision 1.3  2001/10/09 20:47:01  sm
**	- some further texture handling.
**	
**	Revision 1.2  2001/10/07 20:41:32  sm
**	- Updating MSVC project status
**	
**	Revision 1.1  2001/10/07 20:17:27  sm
**	- Prepared texture support.
**	- Noise procedures added.
**	- Added bump and material support.
**	- Added soft shadows.
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        Texture maintainance                          **
**                                                                      **
*************************************************************************/

b3TxPool::b3TxPool()
{
	m_Pool.b3InitBase(CLASS_TEXTURE);
}

b3TxPool::~b3TxPool()
{
	m_Mutex.b3Lock();
	B3_DELETE_BASE(&m_Pool,tx);
	m_Mutex.b3Unlock();
}

b3Base<b3Tx> *b3TxPool::b3GetTxHead()
{
	return &m_Pool;
}

b3_bool b3TxPool::b3ReloadTexture (b3Tx *Texture,const char *Name) /* 30.12.94 */
{
	b3Path    FullName;
	b3_bool   result = false;

	result = b3IsValid(Name,FullName);

	// Check result of texture load
	if (result)
	{
		Texture->b3LoadImage(FullName);
		b3PrintF(B3LOG_DEBUG,"IMG POOL # Image \"%s\" loaded.\n",
			Texture->b3Name());
	}
	else
	{
		Texture->b3Name(Name);
		b3PrintF (B3LOG_DEBUG,"IMG POOL # Image \"%s\" not available!\n",
			Texture->b3Name());
	}
	return result;
}

b3Tx *b3TxPool::b3FindTexture(const char *ParamName)
{
	b3Path      Name;
	b3Tx       *tx;
	const char *txName;
	b3_size     txLen,nameLen,diff;

	Name.b3Correct(ParamName);
	B3_FOR_BASE(&m_Pool,tx)
	{
		txName  = tx->b3Name();
		txLen   = strlen(txName);
		nameLen = strlen(Name);
		diff    = txLen - nameLen;
		if (strcmp(&txName[diff >= 0 ? diff : 0],Name) == 0)
		{
			return tx;
		}
	}
	return tx;
}

b3Tx *b3TxPool::b3LoadTexture(const char *Name) /* 06.12.92 */
{
	b3Tx *tx;

	// find existing texture
	b3PrintF(B3LOG_DEBUG,"IMG POOL # Image \"%s\" to load.\n",Name);
	tx = b3FindTexture(Name);
	if (tx == null)
	{
		// OK, create new texture
		tx = new b3Tx();

		// load data and insert in internal list
		b3ReloadTexture (tx,Name);
		m_Mutex.b3Lock();
		m_Pool.b3Append(tx);
		m_Mutex.b3Unlock();
	}
	else
	{
		b3PrintF (B3LOG_DEBUG,"IMG POOL # Image \"%s\" found.\n",
			tx->b3Name());
	}

	return tx;
}
