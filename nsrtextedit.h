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

#ifndef NSRTEXTEDIT_H
#define NSRTEXTEDIT_H

#include <QTextEdit>
#include <QScrollBar>
#include <QMouseEvent>
#include <QDateTime>
#include <QGesture>

#include "iscrollable.h"
#include "nsrkineticscroller.h"

class NSRTextEdit : public QTextEdit, IScrollable
{
	Q_OBJECT
public:
	explicit NSRTextEdit (QWidget *parent = 0);
	virtual ~NSRTextEdit ();
	void scrollTo (const QPoint& pos);
	void resetZoom ();
	int getZoom ();
	void setZoom (int zoom);
	void zoomOut (int range = 1);
	void zoomIn (int range = 1);
	int getMinZoom () {return 25;}
	int getMaxZoom () {return 400;}
	void stopKineticScroll() {_scroller->stopMoving();}

	/* IScrollable interface */
	QPoint getCurrentPosition ();
	QPoint getMaximumPosition ();
	void setPosition (const QPoint& pos);
	void subscribeForEvents (QObject *obj);
	void unsubscribeOfEvents (QObject *obj);
	QPoint mapFromGlobalCursor (const QPoint& pos);

protected:
	void resizeEvent(QResizeEvent *ev);
	bool event (QEvent *ev);
	bool gestureEvent (QGestureEvent *ev);
	void pinchTriggered (QPinchGesture *gesture);
	void swipeTriggered (QSwipeGesture *gesture);

signals:
	void nextPageRequest();
	void prevPageRequest();
	void doubleClicked(QPoint pos);
	void zoomUpdated();

private slots:
	void onTextChanged();
	void onScrollStarted();
	void onScrollStopped();
	void onScrollChanged();

private:
	void updateScrollBars();

	void pinchStarted ();
	void pinchFinished ();
	void pinchCanceled ();
	void pinchUpdated (double zoomFactor);

	QScrollBar		*_verticalBar;
	QScrollBar		*_horizontalBar;
	int			_defaultFontSize;
	int			_startPinchZoom;
	NSRKineticScroller	*_scroller;
};

#endif // NSRTEXTEDIT_H
