/*
**
**	$Filename:	b3ColorField.h $
**	$Release:	Dortmund 1998 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - color field control
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_COLORFIELD_H
#define B3_SYSTEM_COLORFIELD_H

#include "blz3/b3Config.h"
#include "blz3/base/b3Color.h"

#define UL_2_COLORREF(c) (RGB(((c) & 0xff0000) >> 16,((c) & 0x00ff00) >> 8,(c) & 0x0000ff))
#define COLORREF_2_UL(r) ((b3_pkd_color)(\
	(((r) & 0xff0000) >> 16) |\
	 ((r) & 0x00ff00) |\
	(((r) & 0x0000ff) << 16)))

class B3_PLUGIN CB3ColorField : public CStatic
{
	b3_pkd_color m_Color;
public:
	     CB3ColorField();
	void b3SetColor(b3_pkd_color);

protected:
	//{{AFX_MSG(CB3ColorField)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class B3_PLUGIN CB3LineWidth : public CStatic
{
	b3_pkd_color m_Color;
	b3_res       m_Width;
public:
	       CB3LineWidth();
	b3_res b3SetColor(b3_pkd_color line_color,b3_res line_width);

protected:
	//{{AFX_MSG(CB3LineWidth)
	afx_msg void OnPaint();
	//}}AFX_MSG
 	DECLARE_MESSAGE_MAP()
};

class B3_PLUGIN CB3FontField : public CStatic
{
	b3_pkd_color  m_fgColor;
	b3_pkd_color  m_bgColor;
	LOGFONT      *m_Font;
public:
	        CB3FontField();
	void    b3SetColor(b3_pkd_color fgColor,b3_pkd_color bgColor,LOGFONT *desc);
	b3_bool b3IsEmpty();

protected:
	//{{AFX_MSG(CB3FontField)
	afx_msg void OnPaint();
	//}}AFX_MSG
 	DECLARE_MESSAGE_MAP()
};

enum b3_hsv_mode
{
	B3_HSV_H,
	B3_HSV_S,
	B3_HSV_V
};

extern b3_bool b3HSVtoRGB(b3_f64 h,b3_f64 s,b3_f64 v,b3_f64 &r,b3_f64 &g,b3_f64 &b);

class B3_PLUGIN CB3HSVField : public CStatic
{
	b3_hsv_mode m_Mode;
	b3_f64      m_H,m_S,m_V;

public:
	     CB3HSVField();
	void b3SetHSV(b3_hsv_mode mode,b3_f64 h,b3_f64 s,b3_f64 v);

protected:
	//{{AFX_MSG(CB3HSVField)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	static inline COLORREF b3ComputeColorHSV(
		b3_f64 angle,b3_f64 distance,b3_f64 volume);
	static b3_bool b3HSVtoRGB(
		b3_f64  h,b3_f64  s,b3_f64  v,
		b3_f64 &r,b3_f64 &g,b3_f64 &b);
};

#endif
