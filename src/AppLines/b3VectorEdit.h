/*
**
**	$Filename:	b3VectorEdit.h $
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

#ifndef B3_VECTOR_EDIT_H
#define B3_VECTOR_EDIT_H

#include "blz3/b3Config.h"
#include "blz3/system/b3App.h"
#include "blz3/system/b3FloatEdit.h"
#include "blz3/base/b3Matrix.h"

class CB3PosGroup
{
protected:
	CB3FloatEdit *m_xCtrl;
	CB3FloatEdit *m_yCtrl;
	CB3FloatEdit *m_zCtrl;
	b3_vector    *m_Vector;

public:
	     CB3PosGroup();
	void b3DDX(CDataExchange *pDX);
	void b3Init(b3_vector *vector,CB3FloatEdit *xCtrl,CB3FloatEdit *yCtrl,CB3FloatEdit *zCtrl);
	void b3Update();
	void b3Read(const char *title);
	void b3Write(const char *title);
};

class CB3DirGroup : public CB3PosGroup
{
protected:
	CB3FloatEdit *m_lenCtrl;
	b3_vector    *m_Base;

public:
	enum b3_dir_mode
	{
		B3_DIRMODE_DIR,
		B3_DIRMODE_POS
	};

public:
	     CB3DirGroup();
	void b3Init(b3_vector *base,b3_vector *vector,CB3FloatEdit *xCtrl,CB3FloatEdit *yCtrl,CB3FloatEdit *zCtrl,CB3FloatEdit *lenCtrl);
	void b3Set(int mode);
	void b3Update(int mode);
	void b3UpdateLen(int mode);
};

#endif
