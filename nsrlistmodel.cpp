/*
 * Copyright (C) 2011-2015 Alexander Saprykin <xelfium@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA.
 */

#include "nsrlistmodel.h"

NSRListModel::NSRListModel(QObject *parent) :
	QAbstractItemModel(parent)
{
}

int NSRListModel::rowCount (const QModelIndex &parent) const
{
	Q_UNUSED (parent);

	return _list.count ();
}

int NSRListModel::columnCount (const QModelIndex &parent) const
{
	Q_UNUSED (parent);

	return 1;
}

QVariant NSRListModel::headerData (int section, Qt::Orientation orientation, int role) const
{
	Q_UNUSED (section);
	Q_UNUSED (orientation);
	Q_UNUSED (role);

	return QVariant ("");
}

QVariant NSRListModel::data (const QModelIndex &index, int role) const
{
	if (!index.isValid () || index.row () > _list.count () - 1)
		return QVariant ();

	if (index.column () > 0)
		return QVariant ();

	if (role == Qt::DisplayRole)
		return QVariant (_list.at (index.row ()));
	else
		return QVariant ();


}

QModelIndex NSRListModel::index (int row, int column, const QModelIndex &parent) const
{
	if (row > _list.count () - 1 || column > 0)
		return QModelIndex ();

	if (parent.isValid ())
		return QModelIndex ();

	return createIndex (row, column);
}

QModelIndex NSRListModel::parent (const QModelIndex &child) const
{
	Q_UNUSED (child);

	return QModelIndex ();
}

Qt::ItemFlags NSRListModel::flags (const QModelIndex &index) const
{
	if (!index.isValid ())
	    return 0;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void NSRListModel::setModelData (const QStringList &list)
{
	emit layoutAboutToBeChanged ();
	_list = list;
	emit layoutChanged ();
}
