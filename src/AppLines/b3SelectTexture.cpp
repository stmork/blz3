/*
**
**	$Filename:	b3SelectTexture.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Selecting an image or texture
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
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
#include "b3SelectTexture.h"
#include "b3ImagePreviewFileDlg.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.5  2002/02/26 20:43:28  sm
**	- Moved creation dialogs into property sheets
**	- Added material creation dialog
**
**	Revision 1.4  2002/01/11 16:14:39  sm
**	- Fixed damaged b3Transform() by correcting used parameter vor
**	  b3MatrixMMul and the b3BBox::m_Matrix meber.
**	- Fixed Preview selection dialog.
**	
**	Revision 1.3  2002/01/10 20:18:54  sm
**	- CFileDlg runs but CB3ImagePreviewFileDlg not! I don't know
**	  what to do...
**	
**	Revision 1.2  2002/01/10 17:31:11  sm
**	- Some minor GUI updates.
**	- b3BBox::b3Transform() changes m_Matrix member.
**	- Added image selection with image preview.
**	
**	Revision 1.1  2001/11/11 11:51:21  sm
**	- Added image select feature
**	- Cleaned up scene dialog (Now ready to improve it)
**	- some b3Path fixes
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CB3SelectTexture implementation               **
**                                                                      **
*************************************************************************/

b3_bool CB3SelectTexture::b3Select(b3Tx **tx,char *name)
{
	b3Path    suggest,fullname;
	b3_bool   result;
	CWinApp  *app = AfxGetApp();
	CString   file_filter;
	CString   default_ext;

	// Make filename ready for use...
	if ((*tx) != null)
	{
		suggest.b3Correct((*tx)->b3Name());
	}
	else
	{
		suggest.b3Empty();
	}
	file_filter.LoadString(IDS_TEXTURE_FILTER);

#if 1
	CB3ImagePreviewFileDlg   filedlg(
#else
	CFileDialog   filedlg(
#endif
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
		strcpy(fullname,filedlg.GetPathName());
		texture_pool.b3CutName(fullname,name);
		b3CheckTexture(tx,name);
	}
	return result;
}
