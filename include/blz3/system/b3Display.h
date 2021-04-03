/*
**
**	$Filename:	b3Display.h   $
**	$Release:	Dortmund 2001 $
**	$Revision:	1.07
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Display Hardware Routines
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#pragma once

#ifndef B3_SYSTEM_DISPLAY_H
#define B3_SYSTEM_DISPLAY_H

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/b3Config.h"
#include "blz3/system/b3Memory.h"
#include "blz3/system/b3Exception.h"
#include "blz3/base/b3List.h"
#include "blz3/image/b3Tx.h"

/*************************************************************************
**                                                                      **
**                        typedefs...                                   **
**                                                                      **
*************************************************************************/

/**
 * This enumeration lists display error codes.
 */
enum b3_display_error
{
	B3_DISPLAY_ERROR = -1,   //!< General display error.
	B3_DISPLAY_OK    =  0,   //!< Display is OK.
	B3_DISPLAY_MEMORY,       //!< Out of memory.
	B3_DISPLAY_NO_COLORMAP,  //!< Colormap error
	B3_DISPLAY_OPEN          //!< Unable to open a window.
};

typedef b3Exception<b3_display_error, 0x445350> b3DisplayException;

/**
 * This class handles one single display row.
 */
class B3_PLUGIN b3Row : public b3Link<b3Row>, public b3Mem
{
protected:
	const b3_res     m_xSize;  //!< The width of the row.

public:
	const b3_coord   m_y;      //!< The y positition of the row.
	b3_color    *    m_buffer; //!< The color buffer of this row.

public:
	/**
	 * This constructor initializes the row for the given width and y position.
	 *
	 * \param y The vertical position.
	 * \param xSize The row width.
	 */
	b3Row(const b3_coord y, const b3_res xSize);

	/**
	 * This constructor initializes the internal color puffer with the given
	 * pixel.
	 *
	 * \param y The vertical position.
	 * \param xSize The row width.
	 * \param *buffer The initial color buffer.
	 */
	b3Row(const b3_coord y, const b3_res xSize, b3_color * buffer);

	/**
	 * This destrucotr does nothing.
	 */
	virtual ~b3Row() = default;
};

/**
 * This class simulates a frame buffer for storing a true color image.
 */
class B3_PLUGIN b3Display : public b3Mem
{
	b3_bool               m_OwnTx;

protected:
	b3_res                m_xMax;        //!< The display width.
	b3_res                m_yMax;        //!< The display height.
	b3_res                m_depth;       //!< The real color depth.
	b3_color       *      m_Buffer;      //!< The color back buffer.
	b3Mutex               m_Mutex;       //!< A mutex for thread safe display.
	b3Tx         *        m_Tx;          //!< An image for save control.

public:
	/**
	 * This constructor initializes the display.
	 */
	b3Display();

	/**
	                * This constructor initializes a display with a default size.
	 * The window bar contains the given title.
	 *
	                * \param *title The window title.
	                */
	b3Display(const char * title);

	/**
	 * This constructor opens a display with the resolution of the
	 * given image.
	 *
	 * \param *image The image to display.
	 */
	b3Display(b3Tx * image);

	/**
	                * This constructor initializes a display with the given resolution
	 * and the given title.
	 *
	                * \param xSize The new x resolution.
	                * \param ySize The new y resolution.
	                * \param *title The new window title.
	                */
	b3Display(
		const b3_res  xSize,
		const b3_res  ySize,
		const char  * title = nullptr);
	/**
	 * This destructor deinitializes the display.
	 */
	virtual              ~b3Display();

	/**
	 * This method retrieves the display resolution.
	 *
	 * \param &xSize A reference where the x size is stored.
	 * \param &ySize A reference where the y size is stored.
	 */
	virtual void b3GetRes(b3_res & xSize, b3_res & ySize)
	{
		xSize = m_xMax;
		ySize = m_yMax;
	}

	/**
	 * This method puts a pixel on the display at the given coordinates.
	 *
	 * \param x The x coordinate.
	 * \param y The y coordinate.
	 * \param pixel The pixel color.
	 */
	virtual void b3PutPixel(const b3_coord x, const b3_coord y, const b3_color & pixel)
	{
		B3_ASSERT(m_Buffer != nullptr);
		if ((x >= 0) && (x < m_xMax) && (y >= 0) && (y < m_yMax))
		{
			m_Buffer[y * m_xMax + x] = pixel;
		}
	}

#if 0
	/**
	 * This method returns the pixel color at a given coordinate.
	 *
	 * \param x The x coordinate.
	 * \param y The y coordinate.
	 * \return  The color at the given coordinates.
	 */
	virtual b3_color b3GetPixel(const b3_coord x, const b3_coord y)
	{
		B3_ASSERT(m_Buffer != nullptr);
		return
			((x >= 0) && (x < m_xMax) && (y >= 0) && (y < m_yMax)) ?
			m_Buffer[y * m_xMax + x] :
			0;
	}
#endif

	/**
	 * This method checks whether an cancel event occured. This could be pressing
	 * the excape key or closing a window.
	 *
	 * \param x The x coordinate to print.
	 * \param y The y coordinate to print.
	 * \return True if an cancel event occured.
	 */
	virtual inline b3_bool b3IsCancelled(
		const b3_coord x B3_UNUSED,
		const b3_coord y B3_UNUSED)
	{
		return false;
	}

	/**
	 * This method waits until an close event occures.
	 */
	virtual void b3Wait()
	{
	}

	/**
	 * This method puts an entire row. The y coordinate is stored in the row.
	 *
	 * \param *row The row to put on the display.
	 */
	virtual void          b3PutRow(const b3Row * row);

	/**
	 * This method puts an entire image on the display.
	 *
	 * \param *tx The image to display.
	 */
	virtual void          b3PutTx(b3Tx * tx);

	/**
	 * This method safes the display content to the given file name.
	 *
	 * \param *filename The image file name.
	 * \return True on success.
	 */
	virtual b3_bool       b3SaveImage(const char * filename);

private:
	void  b3Init(const b3_res xSize, const b3_res ySize, const char * title);
};

#endif
