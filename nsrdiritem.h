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

#ifndef NSRDIRITEM_H
#define NSRDIRITEM_H

#include <QString>

class NSRDirItem
{
public:
	NSRDirItem (const QString& name, const QString& fullName, bool isDir);

	QString getName () const {
		return _name;
	}
	void setName (const QString& name) {
		_name = name;
	}

	QString getFullName () const {
		return _fullName;
	}
	void setFullName (const QString& name) {
		_fullName = name;
	}

	bool isDir () const {
		return _isDir;
	}
	void setDir (bool isDir) {
		_isDir = isDir;
	}

private:
	QString	_name;
	QString	_fullName;
	bool	_isDir;
};

#endif // NSRDIRITEM_H
