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

#ifndef NSRTREEVIEW_H
#define NSRTREEVIEW_H

#include "iscrollable.h"
#include "nsrkineticscroller.h"

#include <QTreeView>

class NSRTreeView : public QTreeView, public IScrollable
{
	Q_OBJECT
public:
	explicit NSRTreeView (QWidget *parent = 0);

	/* IScrollable iface */
	QPoint getCurrentPosition ();
	QPoint getMaximumPosition ();
	void setPosition (const QPoint& pos);
	void subscribeForEvents (QObject *obj);
	void unsubscribeOfEvents (QObject *obj);
	QPoint mapFromGlobalCursor (const QPoint& pos);

private slots:
	void onMouseClick (const QPoint& pos);

private:
	NSRKineticScroller	*_scroller;
};

#endif // NSRTREEVIEW_H
