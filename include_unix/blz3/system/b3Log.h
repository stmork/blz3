/*
**
**	$Filename:	b3Log.c $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - logging routines (proto types)
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_LOG_H
#define B3_SYSTEM_LOG_H

#include "blz3/b3Types.h"

typedef enum
{
	B3LOG_NONE   =  0,
	B3LOG_NORMAL = 10,
	B3LOG_DEBUG  = 20,
	B3LOG_FULL   = 30
} b3_log_level;

extern void    b3Log_SetLevel(const b3_log_level debug_limit);
extern b3_bool b3CheckLevel  (const b3_log_level debug_limit);
extern void    b3Log_GetFile (      char *debug_file);
extern bool    b3Log_SetFile (const char *debug_file);
extern void    b3PrintT      (const char *comment = 0);
extern void    b3PrintF      (const b3_log_level debug_level,const char *format,...);


#endif
