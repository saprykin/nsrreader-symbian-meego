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

#include "nsrkineticscroller.h"

#include <QEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QDebug>

NSRKineticScroller::NSRKineticScroller(QObject *parent) :
	QObject(parent),
	_scrollObject (NULL),
	_isPressed (false),
	_isTracking (false),
	_actionsToIgnore (0),
	_velocity (QPoint (0, 0)),
	_lastPosition (QPoint (0, 0)),
	_lastVelocity (QPoint (0, 0)),
	_xScroll (false),
	_yScroll (false),
	_canGoPrevPage (false),
	_canGoNextPage (false),
	_isScrollEnabled (true),
	_trackingIndex (0),
	_lastClickTime (QTime::currentTime())
{
}

NSRKineticScroller::~NSRKineticScroller() {
	if (_isTracking)
		stopMoving();
}

void NSRKineticScroller::setScrollerObject(IScrollable *obj)
{
	if (obj == NULL)
		return;

	if (_scrollObject != NULL) {
		if (_isTracking)
			stopMoving();

		_scrollObject->unsubscribeOfEvents(this);
		_actionsToIgnore = 0;
	}

	_scrollObject = obj;
	_scrollObject->subscribeForEvents(this);
}

bool NSRKineticScroller::eventFilter(QObject *obj, QEvent *ev)
{
	bool		isAction;
	bool		isMouseEvent;

	isAction = (ev->type() == QEvent::MouseButtonPress ||
		    ev->type() == QEvent::MouseButtonRelease);

	isMouseEvent = isAction ||
		       ev->type() == QEvent::MouseMove ||
		       ev->type() == QEvent::MouseButtonDblClick;

	if (_scrollObject == NULL || !isMouseEvent)
		return false;

	QMouseEvent *mouseEv = static_cast<QMouseEvent *> (ev);

	if (isAction && _actionsToIgnore-- > 0)
		return false;

	switch (ev->type()) {
	case QEvent::MouseButtonPress:
	{
		emit firstClick (mouseEv->pos());

		_lastPosition = mouseEv->pos();

		if (!_isTracking) {
			_lastClickTime = QTime::currentTime();
			_lastClickPos = mouseEv->pos();
		}

		if (!_isScrollEnabled)
			return false;

		if (_isTracking)
			stopMovingSilent();

		_isPressed = true;
		_xScroll = false;
		_yScroll = false;
		_canGoPrevPage = false;
		_canGoNextPage = false;
		_trackingPoints.clear();
		_trackingIndex = 0;

		if (_scrollObject->getCurrentPosition().x() + 10 > _scrollObject->getMaximumPosition().x())
			_canGoNextPage = true;

		if (_scrollObject->getCurrentPosition().x() - 10 < 0)
			_canGoPrevPage = true;
	}
		break;

	case QEvent::MouseButtonRelease:
	{
		if (!_isScrollEnabled)
			return false;

		if (!_isTracking && _isPressed) {
			QMouseEvent *pressEv = new QMouseEvent (QEvent::MouseButtonPress,
								mouseEv->pos(),
								Qt::LeftButton,
								Qt::LeftButton,
								Qt::NoModifier);

			QMouseEvent *releaseEv = new QMouseEvent (QEvent::MouseButtonRelease,
								  mouseEv->pos(),
								  Qt::LeftButton,
								  Qt::LeftButton,
								  Qt::NoModifier);

			_actionsToIgnore = 2;

			QApplication::postEvent(obj, pressEv);
			QApplication::postEvent(obj, releaseEv);

			emit scrollStopped();
		}

		_isPressed = false;

		if (_isTracking && (_canGoPrevPage || _canGoNextPage)) {
			int	msecs = 0;
			int	xdist = 0;
			int	ydist = 0;
			int	count = _trackingPoints.count();

			for (int i = 0; i < count; ++i) {
				if (i < count - 1) {
					msecs += _trackingPoints.at(i).time.msecsTo(_trackingPoints.at(i + 1).time);
					xdist += _trackingPoints.at(i).point.x() - _trackingPoints.at(i + 1).point.x();
					ydist += _trackingPoints.at(i).point.y() - _trackingPoints.at(i + 1).point.y();
				}
			}

			if (qAbs(ydist) >= qAbs (xdist))
				return true;

#ifdef Q_OS_SYMBIAN
			if  (_xScroll && msecs < 150 && qAbs(xdist) >= 55) {
#else
			if  (_xScroll && msecs < 300 && qAbs(xdist) >= 55) {
#endif
				if (_canGoPrevPage && xdist <= -55) {
					stopMoving();
					emit prevPageRequest();
				} else if (_canGoNextPage && xdist >= 55) {
					stopMoving();
					emit nextPageRequest();
				}
			}
		}
	}
		break;

	case QEvent::MouseMove:
	{
		if (!_isScrollEnabled)
			return false;

		QPoint diff = mouseEv->pos() - _lastPosition;

#ifdef Q_WS_X11
			diff *= 2;
#endif

		if (!_xScroll && qAbs (diff.x()) > 50)
			_xScroll = true;

		if (!_yScroll && qAbs (diff.y()) > 25)
			_yScroll = true;

		if (!_isTracking && (_yScroll || _xScroll)) {
			startMoving();
			emit scrollStarted();
		}

		if (_isTracking) {
			if (_trackingIndex < 5) {
				_trackingPoints.append(TrackingPoint (QTime::currentTime(), mouseEv->pos()));
				_trackingIndex++;
			} else {
				_trackingPoints.swap(0, 1);
				_trackingPoints.swap(1, 2);
				_trackingPoints.swap(2, 3);
				_trackingPoints.swap(3, 4);
				_trackingPoints.replace(4, TrackingPoint (QTime::currentTime(), mouseEv->pos()));
			}

			emit scrollChanged();
		}

	}
		break;

	case QEvent::MouseButtonDblClick:
	{
		if (!_isTracking) {
			QTime curTime = QTime::currentTime();

			if ((_lastClickTime.msecsTo (curTime) <= 400) &&
			    ((_lastClickPos - mouseEv->pos()).manhattanLength() < 30)) {
				emit doubleClicked(mouseEv->pos());
				break;
			}
		}
	}
		break;

	default:
		break;
	}

	return true;
}

void NSRKineticScroller::stopMoving()
{
	stopMovingSilent();
	emit scrollStopped();
}

void NSRKineticScroller::stopMovingSilent()
{
	if (!_isTracking)
		return;

	_isTracking = false;
	killTimer (_timerId);
}

void NSRKineticScroller::startMoving()
{
	if (_isTracking)
		return;

	_isTracking = true;
	_lastVelocity = _scrollObject->mapFromGlobalCursor(QCursor::pos());
	_timerId = startTimer(20);
}

void NSRKineticScroller::timerEvent (QTimerEvent *ev)
{
	Q_UNUSED (ev);

	if (_isPressed && _isTracking) {
		QPoint curPos = _scrollObject->mapFromGlobalCursor(QCursor::pos());
#ifdef Q_OS_SYMBIAN
		_velocity = curPos - _lastVelocity;
#else
		_velocity = 2 * (curPos - _lastVelocity);
#endif

		QPoint diff = curPos - _lastPosition;
		QPoint scrollPos = _scrollObject->getCurrentPosition();

		if (_xScroll) {
			scrollPos.setX (scrollPos.x() - diff.x());
			_lastPosition.setX(curPos.x());
		}

		if (_yScroll) {
			scrollPos.setY (scrollPos.y() - diff.y());
			_lastPosition.setY(curPos.y());
		}

		_scrollObject->setPosition(scrollPos);
		_lastVelocity = curPos;

		emit scrollChanged();
	} else if (!_isPressed && _isTracking) {
#ifdef Q_OS_SYMBIAN
		int xVelocity = qBound (-160, _velocity.x(), 160);
		int yVelocity = qBound (-160, _velocity.y(), 160);
#else
		int xVelocity = qBound (-320, _velocity.x(), 320);
		int yVelocity = qBound (-320, _velocity.y(), 320);
#endif

		QPoint scrollPoint = _scrollObject->getCurrentPosition();

		xVelocity = _xScroll ? (int) (0.9 * (qreal) xVelocity) : 0;
		yVelocity = _yScroll ? (int) (0.9 * (qreal) yVelocity) : 0;

		_xScroll = (xVelocity != 0);
		_yScroll = (yVelocity != 0);

		if (!_xScroll && !_yScroll)
		    stopMoving();

		_velocity = QPoint (xVelocity, yVelocity);

		if (scrollPoint.x() - xVelocity >= _scrollObject->getMaximumPosition().x() ||
		    scrollPoint.x() - xVelocity <= 0) {
			_xScroll = false;
			_velocity.setX(0);
		}

		if (scrollPoint.y() - yVelocity >= _scrollObject->getMaximumPosition().y() ||
		    scrollPoint.y() - yVelocity <= 0) {
			_yScroll = false;
			_velocity.setY(0);
		}

		scrollPoint.setX (scrollPoint.x() - xVelocity);
		scrollPoint.setY (scrollPoint.y() - yVelocity);

		_scrollObject->setPosition (scrollPoint);
	}
}

void NSRKineticScroller::setScrollEnabled(bool enabled)
{
	stopMoving();
	_isScrollEnabled = enabled;
}
