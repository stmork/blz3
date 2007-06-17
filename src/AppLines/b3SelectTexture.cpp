/*
**
**	$Filename:	b3SelectTexture.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
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

#include "AppLinesInclude.h"
#include "b3SelectTexture.h"
#include "b3ImagePreviewFileDlg.h"

/*************************************************************************
**                                                                      **
**                        CB3SelectLoadTexture implementation           **
**                                                                      **
*************************************************************************/

const char *CB3SelectLoadTexture::m_RegEntry = "file load.image";

b3_bool CB3SelectLoadTexture::b3Select(b3Tx **tx,char *name)
{
	b3Path    suggest,fullname;
	b3_bool   result;
	b3Tx     *ptr;
	CWinApp  *app = AfxGetApp();
	CString   file_filter;
	CString   default_ext;

	// Make filename ready for use...
	if ((*tx) != null)
	{
		ptr = *tx;
		if (!ptr->b3IsLoaded())
		{
			ptr = null;
		}
	}
	else
	{
		ptr = null;
	}

	if (ptr != null)
	{
		suggest.b3Correct(ptr->b3Name());
	}
	else
	{
		suggest.b3Format("%s",app->GetProfileString(CB3ClientString(),m_RegEntry,""));
	}

	file_filter.LoadString(IDS_TEXTURE_FILTER);

#if 1
	CB3ImagePreviewFileDlg   filedlg(
#else
	CFileDialog   filedlg(
#endif
		true, // Use file open dialog
		default_ext, // default extension
		suggest, // file name suggestion to save
		OFN_HIDEREADONLY,	// flags
		file_filter, // File extension filter
		app->m_pMainWnd); // parent

	// Ask for name...
	result = (filedlg.DoModal() == IDOK);
	if (result)
	{
		fullname.b3Format("%s",filedlg.GetPathName());
		app->WriteProfileString(CB3ClientString(),m_RegEntry,fullname);
		b3Scene::m_TexturePool.b3CutName(fullname,name);
		b3Scene::b3CheckTexture(tx,name);
	}
	return result;
}

/*************************************************************************
**                                                                      **
**                        CB3SelectSaveTexture implementation           **
**                                                                      **
*************************************************************************/

const char *CB3SelectSaveTexture::m_RegEntry = "file save.image";

b3_bool CB3SelectSaveTexture::b3Select(b3Path &name,const char *tx_name)
{
	CAppLinesApp *app = CB3GetLinesApp();
	CString       default_ext;
	CString       file_filter;
	CString       suggest;
	b3_bool       result;
	b3_bool       registry = false;

	if (strlen(tx_name) > 0)
	{
		suggest = tx_name;
	}
	else
	{
		suggest = app->GetProfileString(CB3ClientString(),m_RegEntry,name);
		registry = true;
	}
	name.b3Format("%s",(const char *)suggest);
	file_filter.LoadString(IDS_SAVE_IMAGE_FILTER);
	result = b3Path::b3SaveDialog(suggest,default_ext,file_filter,name);
	if (result && registry)
	{
		app->WriteProfileString(CB3ClientString(),m_RegEntry,name);
	}
	return result;
}

const char *CB3SelectSaveTexture::b3GetLastFilename(char *filename)
{
	strcpy(filename,AfxGetApp()->GetProfileString(CB3ClientString(),m_RegEntry,""));
	return filename;
}

