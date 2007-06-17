/*
**
**	$Filename:	b3DocManager.h $
**	$Release:	Dortmund 2006 $
**	$Revision: 1448 $
**	$Date: 2006-06-28 19:07:36 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Document manager
**
**	(C) Copyright 2006  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_DOCMANAGER_H
#define B3_DOCMANAGER_H

#include "stdafx.h"

class CB3DocManager : public CDocManager
{
	DECLARE_DYNAMIC(CB3DocManager)

public:
	virtual BOOL DoPromptFileName(
		CString      &fileName,
		UINT          nIDSTitle,
		DWORD         lFlags,
		BOOL          bOpenFileDialog,
		CDocTemplate *pTemplate)
	{
#if 0
		return CDocManager::DoPromptFileName(fileName,nIDSTitle,lFlags,bOpenFileDialog,pTemplate);
#else
		CString title;
		CString strFilter;
		CString strDefault;
		CString suggest = fileName;
		CString allFilter;
		BOOL    result;

		VERIFY(title.LoadString(nIDSTitle));

		if (pTemplate != NULL)
		{
			CString filterName,filterExt;

			ASSERT_VALID(pTemplate);
			pTemplate->GetDocString(filterName,CDocTemplate::filterName);
			pTemplate->GetDocString(filterExt,CDocTemplate::filterExt);
			strFilter += (filterName + "|*" + filterExt + "|");
		}
		else
		{
			// do for all doc template
			POSITION pos = m_templateList.GetHeadPosition();
			CString filterName,filterExt;

			while (pos != NULL)
			{
				CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);

				ASSERT_VALID(pTemplate);
				pTemplate->GetDocString(filterName,CDocTemplate::filterName);
				pTemplate->GetDocString(filterExt,CDocTemplate::filterExt);
				strFilter += (filterName + "|*" + filterExt + "|");
			}
		}

		// append the "*.*" all files filter
		allFilter.LoadString(AFX_IDS_ALLFILTER);
		strFilter += (allFilter + "|*.*||");

		CB3FileDialog dlgFile(bOpenFileDialog,"",suggest,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | lFlags,strFilter);
		dlgFile.m_ofn.lpstrTitle = title;

		result = dlgFile.DoModal() == IDOK;
		if (result)
		{
			fileName = dlgFile.GetPathName();
		}
		return result;
#endif
	}
};

#endif
