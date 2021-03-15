/*
**
**	$Filename:	b3CameraItem.h $
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

#pragma once

#ifndef QB3BBOXITEM_H
#define QB3BBOXITEM_H

#include <QStandardItem>

#include <blz3/raytrace/b3BBox.h>

Q_DECLARE_METATYPE(b3BBox *)

class QB3BBoxItem : public QStandardItem
{
	enum bbox_taxonomy
	{
		BBOX_EMPTY     = 0,
		BBOX_ACTIVATED = 1,
		BBOX_BBOX_SUB  = 2,
		BBOX_SHAPE_SUB = 4,
		BBOX_EXPANDED  = 8,
		BBOX_ICON_MASK = BBOX_ACTIVATED | BBOX_BBOX_SUB | BBOX_SHAPE_SUB
	};

public:
	explicit QB3BBoxItem(b3BBox * bbox);

	void update();

	inline operator b3BBox * () const
	{
		return data().value<b3BBox *>();
	}

private:
	static unsigned taxonomyOf(const b3BBox * bbox);
	static const QIcon & iconOf(const b3BBox * bbox);
};

#endif // QB3BBOXITEM_H
