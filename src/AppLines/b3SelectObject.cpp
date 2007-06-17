/*
**
**	$Filename:	b3SelectObject.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
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

#include "AppLinesInclude.h"

#include "b3SelectObject.h"
#include "b3ImagePreviewFileDlg.h"

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
	suggest.b3Format("%s",app->GetProfileString(CB3ClientString(),reg_entry,name));
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
		name.b3Format("%s",filedlg.GetPathName());
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
	result = b3Path::b3SaveDialog(suggest,default_ext,file_filter,name);
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
	suggest.b3Format("%s",app->GetProfileString(CB3ClientString(),m_RegEntry,name));
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
		name.b3Format("%s",filedlg.GetPathName());
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
	suggest.b3Format("%s",app->GetProfileString(CB3ClientString(),m_RegEntry,name));
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
		name.b3Format("%s",filedlg.GetPathName());
		app->WriteProfileString(CB3ClientString(),m_RegEntry,name);
	}
	return result;
}
