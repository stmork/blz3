/*
**
**	$Filename:	CB3ImageMultiDocTemplate.h $
**	$Release:	Dortmund 2001, 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Lines application
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_IMAGEMULTIDOCTEMPLATE_H
#define B3_IMAGEMULTIDOCTEMPLATE_H

#include "blz3/image/b3Tx.h"

class CB3ImageMultiDocTemplate : public CMultiDocTemplate
{
	DECLARE_DYNAMIC(CB3ImageMultiDocTemplate)

// Constructors
public:
	CB3ImageMultiDocTemplate(
		UINT           nIDResource,
		CRuntimeClass *pDocClass,
		CRuntimeClass *pFrameClass,
		CRuntimeClass *pViewClass) :
			CMultiDocTemplate(nIDResource,pDocClass,pFrameClass,pViewClass)
	{
	}
	
	virtual Confidence MatchDocType(
		LPCTSTR lpszPathName,
		CDocument*& rpDocMatch)
	{
		Confidence result;
		
		result = CMultiDocTemplate::MatchDocType(lpszPathName,rpDocMatch);
		if(result == yesAttemptForeign)
		{
			b3Path ext;

			ext.b3ExtractExt(lpszPathName);
			if (b3Tx::b3GetFileType(ext) != FT_UNKNOWN)
			{
				result = yesAttemptNative;
			}
		}
		return result;
	}
};

#endif
