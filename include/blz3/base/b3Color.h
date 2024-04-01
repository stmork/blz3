/*
**
**	$Filename:	b3Color.h $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Color handling
**
**	(C) Copyright 2005  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

#pragma once

#ifndef B3_BASE_COLOR_H
#define B3_BASE_COLOR_H

#include "blz3/b3Config.h"

/**
 * This class provides color handling. It uses modern command sets
 * like SSE if the compiler can generate this and the underlying cpu
 * architecture supports it.
 */
class B3_PLUGIN b3ColorBase
{
public:
	static constexpr b3_pkd_color B3_WHITE       = 0x00ffffff;
	static constexpr b3_pkd_color B3_BLACK       = 0x00000000;
	static constexpr b3_pkd_color B3_LIGHT_GREY  = 0x00c0c0c0;
	static constexpr b3_pkd_color B3_GREY        = 0x00808080;
	static constexpr b3_pkd_color B3_DARK_GREY   = 0x00404040;
	static constexpr b3_pkd_color B3_DARK_RED    = 0x00800000;
	static constexpr b3_pkd_color B3_RED         = 0x00ff0000;
	static constexpr b3_pkd_color B3_LIGHT_RED   = 0x00ff4040;
	static constexpr b3_pkd_color B3_DARK_GREEN  = 0x00008000;
	static constexpr b3_pkd_color B3_GREEN       = 0x0000ff00;
	static constexpr b3_pkd_color B3_LIGHT_GREEN = 0x0040ff40;
	static constexpr b3_pkd_color B3_DARK_BLUE   = 0x00000080;
	static constexpr b3_pkd_color B3_BLUE        = 0x000000ff;
	static constexpr b3_pkd_color B3_LIGHT_BLUE  = 0x004040ff;
	static constexpr b3_pkd_color B3_YELLOW      = 0x00ffff00;
	static constexpr b3_pkd_color B3_MAGENTA     = 0x00ff00ff;
	static constexpr b3_pkd_color B3_CYAN        = 0x0000ffff;
	static constexpr b3_pkd_color B3_PEARL       = 0x00a4aeb7;
	static constexpr b3_pkd_color B3_MARKER      = 0x00ff1144;
	static constexpr b3_pkd_color B3_TRANSPARENT = 0xff000000;

	static constexpr b3_f32 COLOR_TOP_NIBBLE     =  15.0;
	static constexpr b3_f32 COLOR_TOP_BYTE       = 255.0;

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

	/**
	 * This method initializes a b3_color structure instance.
	 *
	 * @param color The other b3_color instance to copy.
	 * @param r The new red channel.
	 * @param g The new green channel.
	 * @param b The new blue channel.
	 * @param a The new alpha channel.
	 * @return The b3_color instance pointer.
	 */
	constexpr static b3_color * b3Init(b3_color * color,
		b3_f32 r = 0,
		b3_f32 g = 0,
		b3_f32 b = 0,
		b3_f32 a = 0)
	{
		color->a = a;
		color->r = r;
		color->g = g;
		color->b = b;

		return color;
	}
};

#ifdef BLZ3_USE_SSE
#include "blz3/base/b3ColorSSE.h"
#else
#include "blz3/base/b3ColorStd.h"
#endif

#endif
