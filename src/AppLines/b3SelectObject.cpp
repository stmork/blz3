/*
**
**	$Filename:	b3SelectObject.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Selecting an object
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

#include "AppLines.h"
#include "b3SelectObject.h"
#include "b3ImagePreviewFileDlg.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2002/01/11 16:14:39  sm
**	- Fixed damaged b3Transform() by correcting used parameter vor
**	  b3MatrixMMul and the b3BBox::m_Matrix meber.
**	- Fixed Preview selection dialog.
**
**
*/

/*************************************************************************
**                                                                      **
**                        CB3SelectObject implementation                **
**                                                                      **
*************************************************************************/

b3_bool CB3SelectObject::b3Select(char *name)
{
	b3Path    suggest;
	b3_bool   result;
	CWinApp  *app = AfxGetApp();
	CString   file_filter;
	CString   default_ext;

	// Make filename ready for use...
	strcpy((char *)suggest,name);
	file_filter.LoadString(IDS_OBJECT_FILTER);

	CB3ObjectPreviewFileDlg   filedlg(
		true, // Use file save dialog
		default_ext, // default extension
		suggest, // file name suggestion to save
		OFN_HIDEREADONLY,	// flags
		file_filter, // File extension filter
		app->m_pMainWnd); // parent

	// Ask for name...
	result = (filedlg.DoModal() == IDOK);
	if (result)
	{
		strcpy(name,filedlg.GetPathName());
	}
	return result;
}
