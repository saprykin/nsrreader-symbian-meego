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

#include "nsrimagecontainer.h"
#include "nsrlanguage.h"

#include <QPainter>
#include <QGesture>
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>

NSRImageContainer::NSRImageContainer(QWidget *parent) :
	QWidget (parent),
	_currentPos (0, 0),
	_lastPinchFactor (1.0),
	_useBetaPixmap (false),
	_backgroundColor (Qt::black),
	_maxZoomFactor (1),
	_isStartMode (false)
{
	_horizontalBar = new QScrollBar (this);
	_horizontalBar->setOrientation(Qt::Horizontal);
	_horizontalBar->hide();
	_horizontalBar->setStyleSheet("QScrollBar:horizontal {border: none; background: transparent; height: 10px;}"
				     "QScrollBar::handle:horizontal {background: rgba(40, 40, 40, 220); min-width: 50px;"
					  "border-width: 1px; border-color: rgba(235, 235, 235, 240);"
					  "border-style: solid; border-radius: 5px;}"
				     "QScrollBar::add-line:horizontal {background: transparent; width: 0px; border: none;}"
				     "QScrollBar::sub-line:horizontal {background: transparent; width: 0px; border: none;}"
				     "QScrollBar:up-arrow:horizontal, QScrollBar::down-arrow:horizontal {"
					  "width: 0px; height: 0px; background: none;}"
				     "QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {background: transparent;}");

	_verticalBar = new QScrollBar (this);
	_verticalBar->setOrientation(Qt::Vertical);
	_verticalBar->hide();
	_verticalBar->setStyleSheet("QScrollBar:vertical {border: none; background: transparent; width: 10px;}"
				     "QScrollBar::handle:vertical {background: rgba(40, 40, 40, 220); min-height: 50px;"
					  "border-width: 1px; border-color: rgba(235, 235, 235, 240);"
					  "border-style: solid; border-radius: 5px;}"
				     "QScrollBar::add-line:vertical {background: transparent; height: 0px; border: none;}"
				     "QScrollBar::sub-line:vertical {background: transparent; height: 0px; border: none;}"
				     "QScrollBar:up-arrow:vertical, QScrollBar::down-arrow:vertical {"
					  "width: 0px; height: 0px; background: none;}"
				     "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background: transparent;}");

	grabGesture(Qt::PinchGesture);

	_scroller = new NSRKineticScroller (this);
	_scroller->setScrollerObject(this);

	connect (_scroller, SIGNAL (scrollStarted()), this, SLOT (onScrollStarted()));
	connect (_scroller, SIGNAL (scrollChanged()), this, SLOT (onScrollChanged()));
	connect (_scroller, SIGNAL (scrollStopped()), this, SLOT (onScrollStopped()));
	connect (_scroller, SIGNAL (prevPageRequest()), this, SIGNAL (prevPageRequest()));
	connect (_scroller, SIGNAL (nextPageRequest()), this, SIGNAL (nextPageRequest()));
	connect (_scroller, SIGNAL (doubleClicked(QPoint)), this, SIGNAL (doubleClicked(QPoint)));

#ifdef Q_WS_X11
	QPainter painter;
	const NSRLanguageData*	lang = NSRLanguage::instance()->getLanguage();

	int mw = QApplication::desktop()->width ();
	int mh = QApplication::desktop()->height ();
	int offset = mh / 9;

	_startPixmap = QPixmap (QApplication::desktop()->size ());
	_startPixmap.fill (Qt::black);

	painter.begin (&_startPixmap);
	painter.setPen (QPen (Qt::white));
	QFont font ("Courier New", 24);
	font.setBold (true);
	painter.setFont (font);
	painter.drawText (0,
			  offset,
			  mw,
			  offset + mh / 12,
			  Qt::AlignHCenter,
			  lang->startTitle);
	offset += (mh / 12);
	font.setBold (false);
	font.setPointSize (19);
	painter.setFont (font);
	painter.drawText (0,
			  offset,
			  mw,
			  offset + mh / 10,
			  Qt::AlignHCenter |
			  Qt::AlignVCenter |
			  Qt::TextWordWrap,
			  lang->startTip1);
	offset += mh / 10;
	painter.drawText (0,
			  offset,
			  mw,
			  offset + mh / 10,
			  Qt::AlignHCenter |
			  Qt::AlignVCenter |
			  Qt::TextWordWrap,
			  lang->startTip2);
	offset += mh / 10;
	painter.drawText (0,
			  offset,
			  mw,
			  offset + mh / 12,
			  Qt::AlignHCenter |
			  Qt::AlignVCenter |
			  Qt::TextWordWrap,
			  lang->startTip3);
	offset += mh / 12;
	painter.drawText (0,
			  offset,
			  mw,
			  offset + mh / 8,
			  Qt::AlignHCenter |
			  Qt::AlignVCenter |
			  Qt::TextWordWrap,
			  lang->startTip4);
	painter.end ();
#endif
}

void NSRImageContainer::setPixmap(const QPixmap &pixmap)
{
	_pxm = pixmap;
	_cachedPixmap = pixmap.scaledToWidth(width());
	_currentPos = QPoint (0, 0);
	repaint();
}

void NSRImageContainer::paintEvent(QPaintEvent *ev)
{
	QPainter	painter;
	QRect		renderRect;
	QRect		targetRect;

	if (_isStartMode) {
		QPoint	pos;
		QRect	sourceRect;

		pos.setX (0);
		pos.setY ((_startPixmap.height () - height ()) / 2);

		sourceRect = QRect (ev->rect ());
		sourceRect.setTopLeft (sourceRect.topLeft () + pos);
		sourceRect.setBottomRight (sourceRect.bottomRight () + pos);

		painter.begin (this);
		painter.fillRect (ev->rect (), Qt::black);
		painter.drawPixmap (ev->rect (),
				    _startPixmap,
				    sourceRect);
		painter.end ();
		return;
	}

	if (_useBetaPixmap) {
		int rectWidth = qMin (_betaPxm.width(), width());
		int rectHeight = qMin (_betaPxm.height(), height());

		painter.begin(this);
		painter.fillRect (ev->rect(), _backgroundColor);

		if (!_betaPxm.isNull())
			painter.drawPixmap ((width() - rectWidth) / 2, (height() - rectHeight) / 2,
					    rectWidth, rectHeight,
					    _betaPxm);

		painter.end();
		return;
	}

	if (_pxm.isNull()) {
		painter.begin(this);
		painter.fillRect(ev->rect(), _backgroundColor);
		painter.end();
		return;
	}

	if (ev->rect() != rect() && _pxm.width() >= width() && _pxm.height() >= height()) {
		renderRect = QRect (_currentPos + ev->rect().topLeft(), ev->rect().size());
		targetRect = ev->rect();
	} else {
		int rectWidth = qMin (_pxm.width(), width());
		int rectHeight = qMin (_pxm.height(), height());

		renderRect = QRect (_currentPos, QSize (rectWidth, rectHeight));
		targetRect = QRect ((width() - rectWidth) / 2, (height() - rectHeight) / 2,
					  renderRect.width(), renderRect.height());
	}

	painter.begin (this);
	painter.fillRect (rect(), _backgroundColor);
#ifdef Q_OS_SYMBIAN
	painter.drawPixmap (targetRect, _pxm, renderRect);
#else
	/* For some reasons drawing part of huge pixmap is
	   extremely slow on MeeGo, so we need to create a copy
	   of region to be drawn */
	painter.drawPixmap (targetRect.topLeft (), _pxm.copy (renderRect));
#endif
	painter.end ();
}

void NSRImageContainer::scrollTo(const QPoint &pos)
{
	if (_pxm.isNull())
		return;

	QPoint newPos = pos;

	if (newPos.x() < 0)
		newPos.setX(0);
	else if (newPos.x() + width() > _pxm.width())
		newPos.setX(qMax (_pxm.width() - width(), 0));

	if (newPos.y() < 0)
		newPos.setY(0);
	else if (newPos.y() + height() > _pxm.height())
		newPos.setY(qMax (_pxm.height() - height(), 0));

	if (_currentPos == newPos)
		return;

	_currentPos = newPos;

	onScrollChanged();
	repaint();
}

void NSRImageContainer::updateScrollBars()
{
	if (_pxm.isNull() || _pxm.width() <= width())
		_horizontalBar->hide();

	if (_pxm.isNull() || _pxm.height() <= height())
		_verticalBar->hide();

	_horizontalBar->setRange(0, qMax (0, _pxm.width() - width()));
	_horizontalBar->setPageStep(width());

	_verticalBar->setRange(0, qMax (0, _pxm.height() - height()));
	_verticalBar->setPageStep(height());

	_horizontalBar->resize(width() - _verticalBar->width() - 5, 10);
	_horizontalBar->move(QPoint (5, height() - _horizontalBar->height()));

	_verticalBar->resize(10, height() - _horizontalBar->height() - 5);
	_verticalBar->move(QPoint (width() - _verticalBar->width(), 5));
}

void NSRImageContainer::onScrollStarted()
{
	updateScrollBars();

	if (_pxm.width() > width())
		_horizontalBar->show();

	if (_pxm.height() > height())
		_verticalBar->show();
}

void NSRImageContainer::onScrollChanged()
{
	_horizontalBar->setValue(_currentPos.x());
	_verticalBar->setValue(_currentPos.y());
}

void NSRImageContainer::onScrollStopped()
{
	_horizontalBar->hide();
	_verticalBar->hide();
}

void NSRImageContainer::setBetaPixmap(const QPixmap &pixmap)
{
	_betaPxm = pixmap;

	if (_useBetaPixmap)
		repaint();
}

void NSRImageContainer::setBetaPixmapEnabled(bool enabled)
{
	if (_useBetaPixmap == enabled)
		return;

	_useBetaPixmap = enabled;
	repaint();
}

void NSRImageContainer::resizeEvent(QResizeEvent *ev)
{
	QWidget::resizeEvent(ev);
	scrollTo(_currentPos);
	updateScrollBars();
	repaint();
}

bool NSRImageContainer::event(QEvent *ev)
{
	if (ev->type() == QEvent::Gesture)
		return gestureEvent(static_cast<QGestureEvent*>(ev));

	return QWidget::event(ev);
}

bool NSRImageContainer::gestureEvent(QGestureEvent *ev)
{
	if (QGesture *pinch = ev->gesture(Qt::PinchGesture))
		pinchTriggered(static_cast<QPinchGesture *>(pinch));

	return true;
}

void NSRImageContainer::pinchTriggered(QPinchGesture *gesture)
{
	_scroller->stopMoving();

	if (gesture->state() == Qt::GestureStarted) {
		pinchStarted();
	} else if (gesture->state() == Qt::GestureFinished) {
		pinchFinished();
	} else if (gesture->state() == Qt::GestureCanceled) {
		pinchCanceled();
	} else if (gesture->state() == Qt::GestureUpdated) {
		QPinchGesture::ChangeFlags changeTotalFlags = gesture->totalChangeFlags();
		double factor = gesture->property("totalScaleFactor").toDouble();

		if (factor > _maxZoomFactor)
			factor = _maxZoomFactor;

		if (changeTotalFlags & QPinchGesture::ScaleFactorChanged)
		    pinchUpdated (factor);
	}
}

void NSRImageContainer::pinchStarted()
{
	if (_pxm.isNull())
		return;

	_betaPxm = QPixmap (size());
	QPainter painter (&_betaPxm);
	render(&painter);

	_lastPinchFactor = 1.0;
	_scroller->setScrollEnabled(false);
	setBetaPixmapEnabled(true);
}

void NSRImageContainer::pinchFinished()
{
	if (_pxm.isNull())
		return;

	emit pinchZoomRequested (_lastPinchFactor, _lastPinchPoint);
	_scroller->setScrollEnabled(true);
}

void NSRImageContainer::pinchCanceled()
{
	_scroller->setScrollEnabled(true);
	setBetaPixmapEnabled(false);
}

void NSRImageContainer::pinchUpdated (double zoomFactor)
{
	if (_pxm.isNull())
		return;

	const QPixmap	*displayPixmap = &_pxm;
	int		viewHeight = height();
	int		viewWidth = width();

	if (qAbs (_lastPinchFactor - zoomFactor) < 0.05)
		return;

	_lastPinchFactor = zoomFactor;

	QRect renderArea (getContentsPosition().x(), getContentsPosition().y(),
			  qMin (displayPixmap->width(), viewWidth),
			  qMin (displayPixmap->height(), viewHeight));

	int widthEx = (int) (renderArea.width() * (1 / zoomFactor - 1) / 2 + 0.5);
	int heightEx = (int) (renderArea.height() * (1 / zoomFactor - 1) / 2 + 0.5);

	renderArea.setLeft(qMax (renderArea.left() - widthEx, 0));
	renderArea.setRight(qMin (renderArea.right() + widthEx, displayPixmap->width()));
	renderArea.setTop(qMax (renderArea.top() - heightEx, 0));
	renderArea.setBottom(qMin (renderArea.bottom() + heightEx, displayPixmap->height()));

	if ((int) (renderArea.height() * zoomFactor + 0.5) < viewHeight){
		int hTopThreshold = (int) ((viewHeight - renderArea.height() * zoomFactor) / 2 + 0.5);
		hTopThreshold = (int) (hTopThreshold / zoomFactor + 0.5);
		int hBottomThreshold = hTopThreshold;

		if (renderArea.top() - displayPixmap->rect().top() <= hTopThreshold) {
			hBottomThreshold += hBottomThreshold - (renderArea.top() - displayPixmap->rect().top());
			hTopThreshold = hTopThreshold * 2 - hBottomThreshold;
		} else if (displayPixmap->rect().bottom() - renderArea.bottom() <= hBottomThreshold) {
			hTopThreshold += hTopThreshold - (displayPixmap->rect().bottom() - renderArea.bottom());
			hBottomThreshold = hBottomThreshold * 2 - hTopThreshold;
		}

		renderArea.setTop(renderArea.top() - hTopThreshold);
		renderArea.setBottom(renderArea.bottom() + hBottomThreshold);
	}

	if ((int) (renderArea.width() * zoomFactor + 0.5) < viewWidth) {
		int wLeftThreshold = (int) ((viewWidth - renderArea.width() * zoomFactor) / 2 + 0.5);
		wLeftThreshold = (int) (wLeftThreshold / zoomFactor + 0.5);
		int wRightThreshold = wLeftThreshold;

		if (renderArea.left() - displayPixmap->rect().left() <= wLeftThreshold) {
			wRightThreshold += wRightThreshold - (renderArea.left() - displayPixmap->rect().left());
			wLeftThreshold = wLeftThreshold * 2 - wRightThreshold;
		}

		renderArea.setLeft(renderArea.left() - wLeftThreshold);
		renderArea.setRight(renderArea.right() + wRightThreshold);
	}


	if (zoomFactor < 1.0 &&
			(displayPixmap->width() * zoomFactor >= viewWidth &&
			 displayPixmap->height() * zoomFactor >= viewHeight)) {
		double ratio = (double)displayPixmap->width()  / (double)_cachedPixmap.width();

		renderArea.setTop((int)(renderArea.top() / ratio + 0.5));
		renderArea.setLeft((int)(renderArea.left() / ratio + 0.5));
		renderArea.setRight((int)(renderArea.right() / ratio + 0.5));
		renderArea.setBottom((int)(renderArea.bottom() / ratio + 0.5));

		QPixmap pxm = _cachedPixmap.copy(renderArea);
		setBetaPixmap (pxm.scaled(renderArea.width() * ratio * zoomFactor,
					  renderArea.height() * ratio * zoomFactor));

		_lastPinchPoint = QPoint (renderArea.topLeft().x() * zoomFactor * ratio,
					  renderArea.topLeft().y()  * zoomFactor * ratio);
	} else {
		QPixmap pxm = displayPixmap->copy(renderArea);
		setBetaPixmap (pxm.scaled(pxm.width() * zoomFactor, pxm.height() * zoomFactor));

		_lastPinchPoint = QPoint (renderArea.topLeft().x() * zoomFactor,
					  renderArea.topLeft().y()  * zoomFactor);
	}
}

QPoint NSRImageContainer::getContentsPosition() const
{
	return _currentPos;
}

QPoint NSRImageContainer::getMaximumContentsPosition() const
{
	if (_pxm.isNull() || _useBetaPixmap)
		return QPoint (0, 0);

	return QPoint (_horizontalBar->maximum(), _verticalBar->maximum());
}

void NSRImageContainer::clearBetaPixmap()
{
	_betaPxm = QPixmap ();

	if (_useBetaPixmap)
		repaint ();
}

void NSRImageContainer::clearPixmap()
{
	_pxm = QPixmap ();

	if (!_useBetaPixmap)
		repaint ();
}

void NSRImageContainer::setBackgroundColor(const QColor &color)
{
	if (_backgroundColor == color)
		return;

	_backgroundColor = color;

	if (_useBetaPixmap) {
		if (_betaPxm.width() < width() || _betaPxm.height() < height())
			repaint();
	} else {
		if (_pxm.width() < width() || _pxm.height() < height())
			repaint();
	}
}

void NSRImageContainer::setStartMode (bool enable)
{
	if (_isStartMode == enable)
		return;

	_isStartMode = enable;

	if (_isStartMode)
		clearPixmap ();
}

/* IScrollable interface */
QPoint NSRImageContainer::getCurrentPosition()
{
	return getContentsPosition();
}

QPoint NSRImageContainer::getMaximumPosition()
{
	return getMaximumContentsPosition();
}

void NSRImageContainer::setPosition(const QPoint &pos)
{
	scrollTo(pos);
}

void NSRImageContainer::subscribeForEvents(QObject *obj)
{
	installEventFilter(obj);
}

void NSRImageContainer::unsubscribeOfEvents(QObject *obj)
{
	removeEventFilter(obj);
}

QPoint NSRImageContainer::mapFromGlobalCursor(const QPoint &pos)
{
	return mapFromGlobal(pos);
}

