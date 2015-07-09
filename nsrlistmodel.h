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

#ifndef NSRLISTMODEL_H
#define NSRLISTMODEL_H

#include <QAbstractItemModel>
#include <QStringList>

class NSRListModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	explicit NSRListModel (QObject *parent = 0);
	virtual ~NSRListModel () {}

	int rowCount (const QModelIndex &parent = QModelIndex ()) const;
	int columnCount (const QModelIndex &parent = QModelIndex ()) const;
	QVariant headerData (int		section,
			     Qt::Orientation	orientation,
			     int		role = Qt::DisplayRole) const;
	QVariant data (const QModelIndex &index, int role) const;
	QModelIndex index (int row, int column, const QModelIndex &parent) const;
	QModelIndex parent (const QModelIndex &child) const;
	Qt::ItemFlags flags (const QModelIndex &index) const;

	void setModelData (const QStringList& list);

private:
	QStringList	_list;
};

#endif // NSRLISTMODEL_H
