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
**	Revision 1.5  2003/02/09 13:58:14  sm
**	- cleaned up file selection dialogs
**
**	Revision 1.4  2003/02/02 14:22:32  sm
**	- Added TGF import facility.
**	
**	Revision 1.3  2003/01/26 14:11:50  sm
**	- COB support integrated into Lines III
**	
**	Revision 1.2  2003/01/11 12:30:30  sm
**	- Some additional undo/redo actions
**	
**	Revision 1.1  2002/01/11 16:14:39  sm
**	- Fixed damaged b3Transform() by correcting used parameter vor
**	  b3MatrixMMul and the b3BBox::m_Matrix meber.
**	- Fixed Preview selection dialog.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III object load implementation       **
**                                                                      **
*************************************************************************/

b3_bool CB3SelectLoadObject::b3Select(b3Path &name,const char *reg_entry)
{
	b3Path    suggest;
	b3_bool   result;
	CWinApp  *app = AfxGetApp();
	CString   file_filter;
	CString   default_ext;

	// Make filename ready for use...
	strcpy(suggest,app->GetProfileString(CB3ClientString(),reg_entry,name));
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
		app->WriteProfileString(CB3ClientString(),reg_entry,name);
	}
	return result;
}

/*************************************************************************
**                                                                      **
**                        Blizzard III object load implementation       **
**                                                                      **
*************************************************************************/

const char *CB3SelectSaveObject::m_RegEntry = "file save.object";

b3_bool CB3SelectSaveObject::b3Select(b3Path &name,const char *box_name)
{
	CAppLinesApp *app = CB3GetLinesApp();
	CString       default_ext;
	CString       file_filter;
	CString       suggest;
	b3Path        filepath;
	b3Path        filename;
	b3_bool       result;

	suggest = app->GetProfileString(CB3ClientString(),m_RegEntry,name);
	b3Path::b3SplitFileName(suggest,filepath,null);
	b3Path::b3LinkFileName(filename,filepath,box_name);
	suggest = filename;
	file_filter.LoadString(IDS_OBJECT_FILTER);
	result = b3SaveDialog(suggest,default_ext,file_filter,name);
	if (result)
	{
		app->WriteProfileString(CB3ClientString(),m_RegEntry,name);
	}
	return result;
}

/*************************************************************************
**                                                                      **
**                        Caligari selection implementation             **
**                                                                      **
*************************************************************************/

const char *CB3SelectLoadCOB::m_RegEntry = "file load.caligari";

b3_bool CB3SelectLoadCOB::b3Select(b3Path &name)
{
	b3Path    suggest;
	b3_bool   result;
	CWinApp  *app = AfxGetApp();
	CString   file_filter;
	CString   default_ext;

	// Make filename ready for use...
	strcpy(suggest,app->GetProfileString(CB3ClientString(),m_RegEntry,name));
	file_filter.LoadString(IDS_COB_FILTER);

	CB3FileDialog   filedlg(
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
		app->WriteProfileString(CB3ClientString(),m_RegEntry,name);
	}
	return result;
}

/*************************************************************************
**                                                                      **
**                        ArCon selection implementation                **
**                                                                      **
*************************************************************************/

const char *CB3SelectLoadArcon::m_RegEntry = "file load.arcon";

b3_bool CB3SelectLoadArcon::b3Select(b3Path &name)
{
	b3Path    suggest;
	b3_bool   result;
	CWinApp  *app = AfxGetApp();
	CString   file_filter;
	CString   default_ext;

	// Make filename ready for use...
	strcpy(suggest,app->GetProfileString(CB3ClientString(),m_RegEntry,name));
	file_filter.LoadString(IDS_ARCON_FILTER);

	CB3FileDialog   filedlg(
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
		app->WriteProfileString(CB3ClientString(),m_RegEntry,name);
	}
	return result;
}
