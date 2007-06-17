/*
**
**	$Filename:	b3VectorEdit.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Group CB3FloatEdits to a vector group
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Lines III includes                            **
**                                                                      **
*************************************************************************/

#include "b3SystemInclude.h"
#include "blz3/system/b3VectorEdit.h"
#include "blz3/system/b3App.h"

/*************************************************************************
**                                                                      **
**                        CB3VectorGroup implementation                 **
**                                                                      **
*************************************************************************/

CB3VectorGroup::CB3VectorGroup()
{
	m_Vector = null;
	m_xCtrl  = null;
	m_yCtrl  = null;
	m_zCtrl  = null;
}

void CB3VectorGroup::b3Read(const char *title)
{
	CB3GetApp()->b3ReadProfileVector(title,m_Vector);
};

void CB3VectorGroup::b3Write(const char *title)
{
	CB3GetApp()->b3WriteProfileVector(title,m_Vector);
};

/*************************************************************************
**                                                                      **
**                        CB3PosGroup implementation                    **
**                                                                      **
*************************************************************************/

void CB3VectorGroup::b3Init(
	b3_vector    *vector,
	CB3FloatEdit *xCtrl,
	CB3FloatEdit *yCtrl,
	CB3FloatEdit *zCtrl)
{
	if (m_Vector == null)
	{
		m_Vector = vector;
		m_xCtrl  = xCtrl;
		m_yCtrl  = yCtrl;
		m_zCtrl  = zCtrl;
		m_xCtrl->b3SetDigits(5,3);
		m_yCtrl->b3SetDigits(5,3);
		m_zCtrl->b3SetDigits(5,3);
	}
}

void CB3PosGroup::b3DDX(CDataExchange *pDX)
{
	B3_ASSERT(m_xCtrl  != null);
	B3_ASSERT(m_yCtrl  != null);
	B3_ASSERT(m_zCtrl  != null);
	B3_ASSERT(m_Vector != null);
	m_xCtrl->b3DDX(pDX,m_Vector->x);
	m_yCtrl->b3DDX(pDX,m_Vector->y);
	m_zCtrl->b3DDX(pDX,m_Vector->z);
}

void CB3PosGroup::b3Update()
{
	m_Vector->x = m_xCtrl->b3GetPos();
	m_Vector->y = m_yCtrl->b3GetPos();
	m_Vector->z = m_zCtrl->b3GetPos();
}

/*************************************************************************
**                                                                      **
**                        CB3DirGroup implementation                    **
**                                                                      **
*************************************************************************/

CB3DirGroup::CB3DirGroup()
{
	m_Base    = null;
	m_lenCtrl = null;
}

void CB3DirGroup::b3Init(
	b3_vector    *base,
	b3_vector    *vector,
	CB3FloatEdit *xCtrl,
	CB3FloatEdit *yCtrl,
	CB3FloatEdit *zCtrl,
	CB3FloatEdit *lenCtrl)
{
	CB3VectorGroup::b3Init(vector,xCtrl,yCtrl,zCtrl);
	if (m_Base == null)
	{
		m_Base    = base;
		m_lenCtrl = lenCtrl;
		m_lenCtrl->b3SetMin(0.0001);
		m_lenCtrl->b3SetDigits(5,3);
	}
}

void CB3DirGroup::b3DDX(CDataExchange *pDX, int mode)
{
	if (pDX->m_bSaveAndValidate)
	{
		b3Update(mode);
	}
	else
	{
		b3Set(mode);
	}
}

void CB3DirGroup::b3Set(int mode)
{
	b3_vector pos;

	// Init values
	switch ((b3_dir_mode)mode)
	{
	case B3_DIRMODE_DIR:
		m_xCtrl->b3SetPos(m_Vector->x);
		m_yCtrl->b3SetPos(m_Vector->y);
		m_zCtrl->b3SetPos(m_Vector->z);
		break;
	case B3_DIRMODE_POS:
		b3Vector::b3Add(m_Base,m_Vector,&pos);
		m_xCtrl->b3SetPos(pos.x);
		m_yCtrl->b3SetPos(pos.y);
		m_zCtrl->b3SetPos(pos.z);
		break;
	}
	m_lenCtrl->b3SetPos(b3Vector::b3Length(m_Vector));
}

void CB3DirGroup::b3Update(int mode)
{
	b3_vector pos;

	switch ((b3_dir_mode)mode)
	{
	case B3_DIRMODE_DIR:
		m_Vector->x = m_xCtrl->b3GetPos();
		m_Vector->y = m_yCtrl->b3GetPos();
		m_Vector->z = m_zCtrl->b3GetPos();
		break;
	case B3_DIRMODE_POS:
		pos.x = m_xCtrl->b3GetPos();
		pos.y = m_yCtrl->b3GetPos();
		pos.z = m_zCtrl->b3GetPos();
		b3Vector::b3Sub(&pos,m_Base,m_Vector);
		break;
	}
	m_lenCtrl->b3SetPos(b3Vector::b3Length(m_Vector));
}

void CB3DirGroup::b3UpdateLen(int mode)
{
	b3Vector::b3Normalize(m_Vector,m_lenCtrl->m_Value);
	b3Set(mode);
}
