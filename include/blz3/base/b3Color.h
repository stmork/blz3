/*
**
**      $Filename:      b3Color.h $
**      $Release:       Dortmund 2003 $
**      $Revision$
**      $Date$
**      $Author$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Color handling
**
**      (C) Copyright  Steffen A. Mork
**          All Rights Reserved
**
**
**
**
*/

#ifndef B3_BASE_COLOR_H
#define B3_BASE_COLOR_H

#include "blz3/b3Config.h"

#define B3_WHITE       ((b3_pkd_color)0x00ffffff)
#define B3_LIGHT_GREY  ((b3_pkd_color)0x00c0c0c0)
#define B3_GREY        ((b3_pkd_color)0x00808080)
#define B3_DARK_GREY   ((b3_pkd_color)0x00404040)
#define B3_BLACK       ((b3_pkd_color)0x00000000)
#define B3_DARK_RED    ((b3_pkd_color)0x00800000)
#define B3_RED         ((b3_pkd_color)0x00ff0000)
#define B3_LIGHT_RED   ((b3_pkd_color)0x00ff4040)
#define B3_DARK_GREEN  ((b3_pkd_color)0x00008000)
#define B3_GREEN       ((b3_pkd_color)0x0000ff00)
#define B3_LIGHT_GREEN ((b3_pkd_color)0x0040ff40)
#define B3_DARK_BLUE   ((b3_pkd_color)0x00000080)
#define B3_BLUE        ((b3_pkd_color)0x000000ff)
#define B3_LIGHT_BLUE  ((b3_pkd_color)0x004040ff)
#define B3_YELLOW      ((b3_pkd_color)0x00ffff00)
#define B3_MAGENTA     ((b3_pkd_color)0x00ff00ff)
#define B3_CYAN        ((b3_pkd_color)0x0000ffff)
#define B3_PEARL       ((b3_pkd_color)0x00a4aeb7)
#define B3_MARKER      ((b3_pkd_color)0x00ff1144)
#define B3_TRANSPARENT ((b3_pkd_color)0xff000000)

/**
 * This class provides color handling. It uses modern command sets
 * like SSE if the compiler can generate this and the underlying cpu
 * architecture supports it.
 */
class B3_PLUGIN b3ColorBase
{
public:
	/**
	 * This enum references the seperate color channels.
	 */
	enum b3_color_index
	{
		A = 0, //!< The alpha channel.
		R,     //!< The red channel.
		G,     //!< The green channel.
		B      //!< The blue channel.
	};
};

#ifdef HAVE_SSE
#include "blz3/base/b3ColorSSE.h"
#else
#include "blz3/base/b3ColorStd.h"
#endif

#endif
