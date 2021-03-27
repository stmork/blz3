/*
**
**	$Filename:	b3Tx.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - imaging routines (proto types)
**
**	(C) Copyright 2001 Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#pragma once

#ifndef B3_IMAGE_TXPOOL_H
#define B3_IMAGE_TXPOOL_H

#include "blz3/b3Config.h"
#include "blz3/base/b3SearchPath.h"
#include "blz3/image/b3Tx.h"

/**
 * This class provides a resource pool of images. This image pool is
 * thread safe.
 */
class B3_PLUGIN b3TxPool : public b3SearchPath
{
	b3Mutex          m_Mutex;
	b3Base<b3Tx>     m_Pool;

public:
	/**
	 * This constructor initializes this instance.
	 */
	b3TxPool();

	/**
	 * Ths destructor deinitialize this instance and frees all images.
	 */
	virtual ~b3TxPool();

	/**
	 * This method retrieves an image from the image list. If the image
	 * is already loaded and registered in this pool the image is returned
	 * otherwise the image is loaded, registered and then returned. The
	 * given filename is processed by the search path facility.
	 *
	 * @param name The image name to process.
	 * @return The loaded image.
	 * @see b3SearchPath
	 */
	b3Tx     *    b3LoadTexture(const char * name);

	/**
	 * This method tries to find the given image. If this filename is
	 * found in the list the image is returned otherwise null is returned.
	 * This method is needed by b3LoadTexture() to determine if an image
	 * load is necessary.
	 *
	 * @param name The image name to process.
	 * @return The loaded image.
	 */
	b3Tx     *    b3FindTexture(const char * name);

	/**
	 * This method returns the base of the image list.
	 *
	 * @return The base of the image list.
	 */
	b3Base<b3Tx> * b3GetTxHead();

	/**
	 * This method dumps the collected images for debugging purposes.
	 */
	void b3Dump();

	/**
	 * This method frees all collected images.
	 */
	void b3Free();

private:
	b3_bool       b3ReloadTexture(b3Tx * texture, const char * name = nullptr);
	b3Tx     *    b3FindTextureUnsafe(const char * name);
};

#endif
