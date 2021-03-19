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
	} m_Type = B3_TYPE_USER;

	enum b3_item_role
	{
		B3_ITEM_USERROLE = Qt::UserRole + 1
	} m_Role = B3_ITEM_USERROLE;

public:
	QB3AbstractItem(T * item, b3_item_type type) : m_Type(type)
	{
		setData(QVariant::fromValue(item), B3_ITEM_USERROLE);
	}

	inline operator T * () const
	{
		return data((int)B3_ITEM_USERROLE).value<T *>();
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

	static inline QModelIndexList match(QStandardItemModel * model, T * item)
	{
		return model->match(
				model->index(0, 0),
				B3_ITEM_USERROLE,
				QVariant::fromValue(item),
				1,
				Qt::MatchExactly | Qt::MatchRecursive);
	}

	static inline QB3AbstractItem<T> * find(QStandardItemModel * model, T * item)
	{
		const QModelIndexList & list = match(model, item);

		return list.isEmpty() ? nullptr : (QB3AbstractItem<T> *)model->itemFromIndex(list.first());
	}
};

#endif // QB3ABSTRACTITEM_H
