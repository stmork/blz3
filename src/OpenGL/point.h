/*
**
**	$Filename:	point.h $
**	$Release:	Dortmund 2001 - 2021 $
**
**	Blizzard III - The new Blizzard III raytracer
**
**      (C) Copyright 2001 - 2021  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#ifndef POINT_H_
#define POINT_H_

#include <cstdint>
#include "sc_types.h"

typedef struct
{
	sc::integer x;
	sc::integer y;
} SCT_point;

#endif
