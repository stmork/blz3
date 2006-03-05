/*
**
**	$Filename:	b3ViewParam.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - View parameter handling
**
**	(C) Copyright 2001  Stefen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3SystemInclude.h"
#include "blz3/system/b3ViewParam.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2006/03/05 21:22:37  sm
**	- Added precompiled support for faster comiling :-)
**
**	Revision 1.2  2003/08/31 10:44:07  sm
**	- Further buffer overflow avoidments.
**	
**	Revision 1.1  2001/07/07 21:21:15  sm
**	- OK! Imported some display stuff using the CScrollView. After getting linked today
**	  it should possible to display real things tomorrow.
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III CB3ViewParam                     **
**                                                                      **
*************************************************************************/

CB3ViewParam::CB3ViewParam(const char *param_name) : b3Link<CB3ViewParam>(sizeof(CB3ViewParam))
{
	snprintf(m_Name,sizeof(m_Name),"%s",param_name);
}

char *CB3ViewParam::b3GetName()
{
	return m_Name;
}

CB3ViewParamBase::~CB3ViewParamBase()
{
	while (m_Params.First != null)
	{
		b3DeleteParam(m_Params.First);
	}
}

CB3ViewParam *CB3ViewParamBase::b3GetViewParam(const char *param_name,b3_bool &new_one)
{
	CB3ViewParam *param;

	new_one    = false;
	for (param = m_Params.First;param != null;param = param->Succ)
	{
		if (strcmp(param_name,param->b3GetName()) == 0)
		{
			return param;
		}
	}

	try
	{
		param   = new CB3ViewParam(param_name);
		new_one = true;
		m_Params.b3Append(param);
	}
	catch(...)
	{
		param = null;
	}
	return param;
}

void CB3ViewParamBase::b3DeleteParam(CB3ViewParam *param,b3_bool delete_reg_entry)
{
	HKEY    view_param_key;
	CString sub_key;

	if (param != null)
	{
		if (delete_reg_entry)
		{
			sub_key.Format("Software\\%s\\%s\\View Parameter",BLIZZARD3_REG_COMPANY,BLIZZARD3_REG_PRODUCT);
			if (::RegOpenKey(HKEY_CURRENT_USER,sub_key,&view_param_key) == ERROR_SUCCESS)
			{
				::RegDeleteValue(view_param_key,param->b3GetName());
				::RegCloseKey(view_param_key);
			}
		}
		m_Params.b3Remove(param);
		delete param;
	}
}

CB3ViewParam *CB3ViewParamBase::b3GetFirstParam()
{
	return m_Params.First;
}

b3_bool CB3ViewParamBase::b3LoadParams()
{
	HKEY           view_param_key;
	CWinApp       *app = AfxGetApp();
	CString        param_values;
	CString        sub_key;
	CB3ViewParam  *param;
	long           filtered;
	b3_index       index = 0;
	char           name[1024];
	b3_bool        new_one;
	unsigned long  type,nameSize;

	sub_key.Format("Software\\%s\\%s\\View Parameter",BLIZZARD3_REG_COMPANY,BLIZZARD3_REG_PRODUCT);
	if (::RegOpenKey(HKEY_CURRENT_USER,sub_key,&view_param_key) == ERROR_SUCCESS)
	{
		nameSize = sizeof(name);
		while (::RegEnumValue(view_param_key,index,name,&nameSize,null,&type,null,null) == ERROR_SUCCESS)
		{
			param = b3GetViewParam(name,new_one);
			if (param != null)
			{
				param_values = app->GetProfileString("View Parameter",param->b3GetName(),"");
				sscanf(param_values,"%ld %ld - %ld %ld %ld %ld - %ld - %ld %ld %lf %lf",
						&param->m_Mode,&filtered,
						&param->m_xWinPos,&param->m_yWinPos,&param->m_xWinSize,&param->m_yWinSize,
						&param->m_WinMode,
						&param->m_xImgPos,&param->m_yImgPos,&param->m_xImgScale,&param->m_yImgScale);
				param->m_Filtered = filtered != 0;
			}
			index++;
			nameSize = sizeof(name);
		}
		::RegCloseKey(view_param_key);
	}
	return true;
}

b3_bool CB3ViewParamBase::b3SaveParams()
{
	CWinApp        *app = AfxGetApp();
	CString         param_values;
	CB3ViewParam *param;

	for (param = m_Params.First;param != null;param = param->Succ)
	{
		param_values.Format("%ld %ld - %ld %ld %ld %ld - %ld - %ld %ld %lf %lf",
			param->m_Mode,param->m_Filtered,
			param->m_xWinPos,param->m_yWinPos,param->m_xWinSize,param->m_yWinSize,
			param->m_WinMode,
			param->m_xImgPos,param->m_yImgPos,param->m_xImgScale,param->m_yImgScale);
		app->WriteProfileString("View Parameter",param->b3GetName(),param_values);
	}
	return false;
}
