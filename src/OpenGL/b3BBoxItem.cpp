/*
**
**	$Filename:	b3BBoxItem.cpp $
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

#include "b3BBoxItem.h"

QB3BBoxItem::QB3BBoxItem(b3BBox * bbox) : QB3AbstractItem(bbox, B3_TYPE_BBOX)
{
	setIcon(iconOf(bbox));
	setText(QString::fromLatin1(bbox->b3GetName()));
}

void QB3BBoxItem::update()
{
	setIcon(iconOf(*this));
}

unsigned QB3BBoxItem::taxonomyOf(const b3BBox * bbox)
{
	unsigned taxonomy = BBOX_EMPTY;

	if (bbox->b3IsActive())
	{
		taxonomy |= bbox_taxonomy::BBOX_ACTIVATED;
	}
	if (!bbox->b3GetBBoxHead()->b3IsEmpty())
	{
		taxonomy |= BBOX_BBOX_SUB;
	}
	if (!bbox->b3GetShapeHead()->b3IsEmpty())
	{
		taxonomy |= BBOX_SHAPE_SUB;
	}
	if (bbox->b3IsExpanded())
	{
		taxonomy |= BBOX_EXPANDED;
	}
	return taxonomy;
}

const QIcon & QB3BBoxItem::iconOf(const b3BBox * bbox)
{
	static const QIcon bbox_taxonomy_map[]
	{
		QIcon(":/treeview/BBox_empty"),        // BBOX_EMPTY
		QIcon(":/treeview/BBox_empty"),        // BBOX_ACTIVATED
		QIcon(":/treeview/BBox_sub"),          // BBOX_BBOX_SUB
		QIcon(":/treeview/BBox_sub"),          // BBOX_BBOX_SUB | BBOX_ACTIVATED
		QIcon(":/treeview/BBox_Shape"),        // BBOX_SHAPE_SUB
		QIcon(":/treeview/BBox_Shape_sel"),    // BBOX_SHAPE_SUB | BBOX_ACTIVATED
		QIcon(":/treeview/BBox_Shape_sub"),    // BBOX_SHAPE_SUB | BBOX_BBOX_SUB
		QIcon(":/treeview/BBox_Shape_sub_sel") // BBOX_SHAPE_SUB | BBOX_BBOX_SUB | BBOX_ACTIVATED
	};

	const unsigned   taxonomy = taxonomyOf(bbox);

	return bbox_taxonomy_map[taxonomy & BBOX_ICON_MASK];
}
