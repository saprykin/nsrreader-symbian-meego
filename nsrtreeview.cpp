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

#include "nsrtreeview.h"

#include <QScrollBar>
#include <QCommonStyle>

NSRTreeView::NSRTreeView (QWidget *parent) :
	QTreeView (parent)
{
	setVerticalScrollMode (QAbstractItemView::ScrollPerPixel);
	_scroller = new NSRKineticScroller (this);
	_scroller->setScrollerObject (this);

	connect (_scroller, SIGNAL (firstClick (QPoint)),
		 this, SLOT (onMouseClick (QPoint)));
}

QPoint NSRTreeView::getCurrentPosition ()
{
	return QPoint (horizontalScrollBar()->value (),
		       verticalScrollBar()->value ());
}

QPoint NSRTreeView::getMaximumPosition ()
{
	return QPoint (horizontalScrollBar()->maximum (),
		       verticalScrollBar()->maximum ());
}

void NSRTreeView::setPosition (const QPoint &pos)
{
	horizontalScrollBar()->setValue (pos.x ());
	verticalScrollBar()->setValue (pos.y ());
}

void NSRTreeView::subscribeForEvents (QObject *obj)
{
	viewport()->installEventFilter (obj);
}

void NSRTreeView::unsubscribeOfEvents (QObject *obj)
{
	viewport()->removeEventFilter (obj);
}

QPoint NSRTreeView::mapFromGlobalCursor (const QPoint &pos)
{
	return viewport()->mapFromGlobal (pos);
}

void NSRTreeView::onMouseClick (const QPoint &pos)
{
	selectionModel()->select (indexAt (pos),
				  QItemSelectionModel::SelectCurrent);
}
