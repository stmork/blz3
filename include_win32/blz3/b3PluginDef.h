/*
**
**	$Filename:      b3PluginDef.h $
**	$Release:       Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - class import/export to plugins
**
**	(C) Copyright  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

#ifndef B3_PLUGINDEF_H
#define B3_PLUGINDEF_H

#ifndef B3_PLUGIN
#	pragma warning(disable : 4273)
#	ifdef _B3_EXPORT
#		define B3_PLUGIN __declspec(dllexport)
#	else
#		define B3_PLUGIN __declspec(dllimport)
#	endif
#endif

#endif
