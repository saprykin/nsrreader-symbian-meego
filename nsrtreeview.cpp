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
