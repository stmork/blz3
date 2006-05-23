/*
**
**	$Filename:	b3OceanWaveCtrl.h $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Control for wind speed
**
**	(C) Copyright 2006  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_OCEANWAVECTRL_H
#define B3_OCEANWAVECTRL_H

#pragma once
#include "blz3/b3Config.h"
#include "blz3/base/b3OceanWave.h"
#include "blz3/system/b3View.h"

class CB3OceanWaveCtrl :
	public CStatic
{
	b3OceanWave *m_Ocean;
	b3_f64       m_Factor;
	b3_bool      m_MouseCapture;
	b3Tx         m_Tx;
	b3_res       m_xSize;
	b3_res       m_ySize;
	CRect        m_Rect;
	CPoint       m_MidPoint;
	CPoint       m_LastPoint;
	CB3BitmapDDB m_DDB;

public:

	CB3OceanWaveCtrl(void)
	{
		m_Ocean        = null;
		m_MouseCapture = false;
		m_Factor       = 10.0;
	}

	virtual ~CB3OceanWaveCtrl(void)
	{
	}

	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();

public:
	void b3SetOcean(b3OceanWave *ocean);

private:
	void b3UpdateUI(CPoint *point = null);
};

#endif
