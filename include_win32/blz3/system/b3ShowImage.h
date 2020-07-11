/*
**
**	$Filename:	b3ShowImage.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - displaying an image inside a control
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

#ifndef B3_SYSTEM_SHOWIMAGE_H
#define B3_SYSTEM_SHOWIMAGE_H

#include "blz3/b3Config.h"
#include "blz3/system/b3Bitmap.h"
#include "blz3/image/b3Tx.h"

class CB3ShowImage : public CStatic
{
	DECLARE_DYNAMIC(CB3ShowImage)
protected:
	CRect         m_Rect;
	CB3BitmapDDB  m_DDB;
	b3Tx          m_Tx;
	b3_coord      m_xPos;
	b3_coord      m_yPos;
	b3_res        m_xSize;
	b3_res        m_ySize;
public:
	CB3ShowImage();
	b3_bool b3Load(const char * name);

	void    b3Aspect(b3Tx * srcTx,
		b3_coord & xPos, b3_coord & yPos,
		b3_res  & xSize, b3_res  & ySize,
		b3_bool keep_aspect = true);

	b3_bool b3Update(b3_bool filtered, b3_bool keep_aspect = true);
	void    b3Copy(b3Tx * original);

	operator b3Tx * ();

	// Generated message map functions
protected:
	//{{AFX_MSG(CB3ShowImage)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif
