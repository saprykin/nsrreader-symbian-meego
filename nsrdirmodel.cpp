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

#include "nsrdirmodel.h"

#include <QFileInfoList>
#include <QIcon>

NSRDirModel::NSRDirModel (QObject *parent) :
	QAbstractItemModel (parent)
{
}

NSRDirModel::~NSRDirModel ()
{
	cleanList ();
}

int NSRDirModel::rowCount (const QModelIndex &parent) const
{
	if (!parent.isValid ())
		return _list.count ();
	else
		return 0;
}

int NSRDirModel::columnCount (const QModelIndex &parent) const
{
	Q_UNUSED (parent);

	return 1;
}

QVariant NSRDirModel::headerData (int section, Qt::Orientation orientation, int role) const
{
	Q_UNUSED (section);
	Q_UNUSED (orientation);
	Q_UNUSED (role);

	return QVariant ("");
}

QVariant NSRDirModel::data (const QModelIndex &index, int role) const
{
	NSRDirItem *item;

	if (!index.isValid () || index.row () > _list.count () - 1)
		return QVariant ();

	if (index.column () > 0)
		return QVariant ();

	item = _list.at (index.row ());

	if (role == Qt::DisplayRole)
		return QVariant (item->getName ());
	else if (role == Qt::DecorationRole) {
		if (item->isDir () && item->getName () != "..")
			return QVariant (QIcon (QString::fromUtf8(":/icons/icons/open.png")));
		else if (!item->isDir())
			return QVariant (QIcon (QString::fromUtf8(":/icons/icons/document.png")));
		else
			return QVariant ();
	} else
		return QVariant ();
}

QModelIndex NSRDirModel::index (int row, int column, const QModelIndex &parent) const
{
	if (row > _list.count () - 1 || column > 0)
		return QModelIndex ();

	if (parent.isValid ())
		return QModelIndex ();

	return createIndex (row, column);
}

QModelIndex NSRDirModel::parent (const QModelIndex &child) const
{
	Q_UNUSED (child);

	return QModelIndex ();
}

Qt::ItemFlags NSRDirModel::flags (const QModelIndex &index) const
{
	if (!index.isValid ())
	    return 0;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void NSRDirModel::setRootPath (const QString &path)
{
	QFileInfo info (path);

	if (!info.exists ())
		return;

	_dir.setPath (path);

	updateDataFromDir ();

	emit currentPathChanged (_dir.absolutePath ());
}

void NSRDirModel::setNameFilters (const QStringList &filters)
{
	_filters = filters;

	updateDataFromDir ();
}

void NSRDirModel::itemClicked (const QModelIndex &index)
{
	if (!index.isValid ())
		return;

	if (index.row () > _list.count () - 1)
		return;

	if (!_list.at(index.row ())->isDir ()) {
		_selectedFile = _list.at(index.row ())->getFullName ();

		emit fileSelected ();
	} else {
		if (_list.at(index.row ())->getName () == "..") {
			_dir.cdUp ();
			setRootPath (_dir.absolutePath ());
		} else
			setRootPath (_list.at(index.row ())->getFullName ());
	}
}

QString NSRDirModel::getCurrentPath () const
{
	return _dir.absolutePath ();
}

void NSRDirModel::updateDataFromDir ()
{
	QFileInfoList		entries;
	QList<NSRDirItem *>	list;
	int			count;

	entries = _dir.entryInfoList (QStringList (), QDir::Dirs, QDir::Name);
	count = entries.count ();

	for (int i = 0; i < count; ++i) {
		if (entries.at(i).fileName () == ".")
			continue;

		list.append (new NSRDirItem (entries.at(i).fileName (),
					     entries.at(i).absoluteFilePath(),
					     true));
	}

	entries = _dir.entryInfoList (_filters, QDir::Files, QDir::Name);
	count = entries.count ();

	for (int i = 0; i < count; ++i) {
		if (entries.at(i).fileName () == ".")
			continue;

		list.append (new NSRDirItem (entries.at(i).fileName (),
					     entries.at(i).absoluteFilePath(),
					     false));
	}

	if (list.isEmpty ())
		list.append (new NSRDirItem ("..",
					     _dir.path () + "/..",
					     true));

	emit layoutAboutToBeChanged ();

	cleanList ();
	_list = list;

	emit layoutChanged ();
}

void NSRDirModel::cleanList ()
{
	while (!_list.isEmpty ())
		delete _list.takeFirst ();
}
