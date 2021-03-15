/*
**
**	$Filename:	b3AbstractItem.h $
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

#ifndef QB3ABSTRACTITEM_H
#define QB3ABSTRACTITEM_H

#include <QStandardItem>

template<class T> class QB3AbstractItem : public QStandardItem
{
protected:
	enum b3_item_type
	{
		B3_TYPE_USER = QStandardItem::UserType,
		B3_TYPE_SCENE,
		B3_TYPE_CAMERA,
		B3_TYPE_LIGHT,
		B3_TYPE_BBOX
	} m_Type;

public:
	QB3AbstractItem(T * item, b3_item_type type) : m_Type(type)
	{
		setData(QVariant::fromValue(item));
	}

	inline operator T * () const
	{
		return data().value<T *>();
	}

	inline int type() const override
	{
		return m_Type;
	}

	inline virtual bool check()
	{
		return checkState() == Qt::Checked;
	}

	inline virtual void update()
	{
	}
};

#endif // QB3ABSTRACTITEM_H
