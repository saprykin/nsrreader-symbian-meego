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

#ifndef NSRKINETICSCROLLER_H
#define NSRKINETICSCROLLER_H

#include <QObject>
#include <QPoint>
#include <QTime>
#include <QList>

#include "iscrollable.h"

struct TrackingPoint {
	TrackingPoint (const QTime& t, const QPoint& p) :
		time (t),
		point (p)
	{}

	QTime	time;
	QPoint	point;
};

class NSRKineticScroller : public QObject
{
	Q_OBJECT

public:
	explicit NSRKineticScroller (QObject *parent = 0);
	virtual ~NSRKineticScroller ();
	void setScrollerObject (IScrollable *obj);
	void setScrollEnabled (bool enabled);
	void stopMoving();
	void stopMovingSilent();

protected:
	bool eventFilter (QObject *obj, QEvent *ev);
	void timerEvent(QTimerEvent *ev);

signals:
	void scrollStarted();
	void scrollChanged();
	void scrollStopped();
	void doubleClicked(const QPoint& pos);
	void prevPageRequest();
	void nextPageRequest();
	void firstClick (const QPoint& pos);

private:
	void startMoving();

	IScrollable		*_scrollObject;
	bool			_isPressed;
	bool			_isTracking;
	int			_actionsToIgnore;
	QPoint			_velocity;
	QPoint			_lastPosition;
	QPoint			_lastVelocity;
	int			_timerId;
	bool			_xScroll;
	bool			_yScroll;
	bool			_canGoPrevPage;
	bool			_canGoNextPage;
	bool			_isScrollEnabled;
	QList<TrackingPoint>	_trackingPoints;
	int			_trackingIndex;
	QTime			_lastClickTime;
	QPoint			_lastClickPos;
};

#endif // NSRKINETICSCROLLER_H
