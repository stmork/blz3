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
#include "blz3/raytrace/b3Scene.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.9  2005/01/23 19:54:06  sm
**	- Experimented with OpenGL settings for Linux Wine but there
**	  is no solution for Wine/Windows MDI applications to use OpenGL.
**	- Optimized precompiled header.
**
**	Revision 1.8  2003/08/31 10:44:07  sm
**	- Further buffer overflow avoidments.
**	
**	Revision 1.7  2003/02/22 17:21:32  sm
**	- Changed some global variables into static class members:
**	  o b3Scene::epsilon
**	  o b3Scene::m_TexturePool et. al.
**	  o b3SplineTemplate<class VECTOR>::bspline_errno
**	
**	Revision 1.6  2003/02/09 13:58:14  sm
**	- cleaned up file selection dialogs
**	
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
	result = b3SaveDialog(suggest,default_ext,file_filter,name);
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

