/*
**
**	$Filename:      b3PluginDef.h $
**	$Release:       Dortmund 2003 $
**	$Revision: 1431 $
**	$Date: 2006-05-27 15:32:22 +0200 (Sa, 27 Mai 2006) $
**	$Author: sm $
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - class import/export to plugins
**
**	(C) Copyright 2003  Steffen A. Mork
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

#ifndef B3_PLUGIN_EXPORT
#define B3_PLUGIN_EXPORT __declspec(dllexport)
#endif

#endif
