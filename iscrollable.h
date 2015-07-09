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

#ifndef ISCROLLABLE_H
#define ISCROLLABLE_H

#include <QPoint>
#include <QObject>

class IScrollable
{
public:
	virtual QPoint getCurrentPosition ()			= 0;
	virtual QPoint getMaximumPosition ()			= 0;
	virtual void setPosition (const QPoint& pos)		= 0;
	virtual void subscribeForEvents (QObject *obj)		= 0;
	virtual void unsubscribeOfEvents (QObject *obj)		= 0;
	virtual QPoint mapFromGlobalCursor (const QPoint& pos)	= 0;
};

#endif // ISCROLLABLE_H
