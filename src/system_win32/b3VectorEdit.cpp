/*
**
**	$Filename:	b3VectorEdit.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
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

#include "b3VectorEdit.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2002/02/27 20:14:51  sm
**	- Added stencil creation for creating simple shapes.
**	- Fixed material creation.
**	- Cleaned up some files.
**
**
*/

/*************************************************************************
**                                                                      **
**                        CB3PosGroup implementation                    **
**                                                                      **
*************************************************************************/

CB3PosGroup::CB3PosGroup()
{
	m_Vector = null;
	m_xCtrl  = null;
	m_yCtrl  = null;
	m_zCtrl  = null;
}

void CB3PosGroup::b3Init(b3_vector *vector,CB3FloatEdit *xCtrl,CB3FloatEdit *yCtrl,CB3FloatEdit *zCtrl)
{
	if (m_Vector == null)
	{
		m_Vector = vector;
		m_xCtrl  = xCtrl;
		m_yCtrl  = yCtrl;
		m_zCtrl  = zCtrl;
	}
}

void CB3PosGroup::b3Read(const char *title)
{
	CB3GetApp()->b3ReadProfileVector(title,m_Vector);
};

void CB3PosGroup::b3Write(const char *title)
{
	CB3GetApp()->b3WriteProfileVector(title,m_Vector);
};

void CB3PosGroup::b3Set(b3_bool init)
{
	if (init)
	{
		m_xCtrl->b3SetDigits(5,3);
		m_yCtrl->b3SetDigits(5,3);
		m_zCtrl->b3SetDigits(5,3);
	}
	m_xCtrl->b3SetValue(m_Vector->x);
	m_yCtrl->b3SetValue(m_Vector->y);
	m_zCtrl->b3SetValue(m_Vector->z);
}

void CB3PosGroup::b3Update()
{
	m_Vector->x = m_xCtrl->m_Value;
	m_Vector->y = m_yCtrl->m_Value;
	m_Vector->z = m_zCtrl->m_Value;
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

void CB3DirGroup::b3Init(b3_vector *base,b3_vector *vector,CB3FloatEdit *xCtrl,CB3FloatEdit *yCtrl,CB3FloatEdit *zCtrl,CB3FloatEdit *lenCtrl)
{
	CB3PosGroup::b3Init(vector,xCtrl,yCtrl,zCtrl);
	if (m_Base == null)
	{
		m_lenCtrl = lenCtrl;
		m_Base    = base;
	}
}

void CB3DirGroup::b3Set(int mode,b3_bool init)
{
	b3_vector pos;

	// Set edit characteristics
	if (init)
	{
		m_lenCtrl->b3SetMin(0.0001);
		m_lenCtrl->b3SetDigits(5,3);
		m_xCtrl->b3SetDigits(5,3);
		m_yCtrl->b3SetDigits(5,3);
		m_zCtrl->b3SetDigits(5,3);
	}

	// Init values
	switch ((b3_dir_mode)mode)
	{
	case B3_DIRMODE_DIR:
		m_xCtrl->b3SetValue(m_Vector->x);
		m_yCtrl->b3SetValue(m_Vector->y);
		m_zCtrl->b3SetValue(m_Vector->z);
		break;
	case B3_DIRMODE_POS:
		b3Vector::b3Add(m_Base,m_Vector,&pos);
		m_xCtrl->b3SetValue(pos.x);
		m_yCtrl->b3SetValue(pos.y);
		m_zCtrl->b3SetValue(pos.z);
		break;
	}
	m_lenCtrl->b3SetValue(b3Vector::b3Length(m_Vector));
}

void CB3DirGroup::b3Update(int mode)
{
	b3_vector pos;

	switch ((b3_dir_mode)mode)
	{
	case B3_DIRMODE_DIR:
		m_Vector->x = m_xCtrl->m_Value;
		m_Vector->y = m_yCtrl->m_Value;
		m_Vector->z = m_zCtrl->m_Value;
		break;
	case B3_DIRMODE_POS:
		pos.x = m_xCtrl->m_Value;
		pos.y = m_yCtrl->m_Value;
		pos.z = m_zCtrl->m_Value;
		b3Vector::b3Sub(&pos,m_Base,m_Vector);
		break;
	}
	m_lenCtrl->b3SetValue(b3Vector::b3Length(m_Vector));
}

void CB3DirGroup::b3UpdateLen(int mode)
{
	b3Vector::b3Normalize(m_Vector,m_lenCtrl->m_Value);
	b3Set(mode);
}
