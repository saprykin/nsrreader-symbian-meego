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

#include "nsrscrollarea.h"

#include <QPainter>

NSRScrollArea::NSRScrollArea (QWidget *parent) :
	QScrollArea (parent),
	_xScroll (false),
	_yScroll (false)
{
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setFrameStyle(QFrame::NoFrame | QFrame::Plain);
	setWidgetResizable(true);
	setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

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
}

NSRScrollArea::~NSRScrollArea()
{
}

void NSRScrollArea::mousePressEvent(QMouseEvent *e)
{
	QScrollArea::mousePressEvent(e);

	_mousePos = e->pos();
}

void NSRScrollArea::mouseMoveEvent(QMouseEvent *e)
{
	QScrollArea::mouseMoveEvent(e);

	QPoint diff = e->pos() - _mousePos;
	QPoint scrollPoint (horizontalScrollBar()->value(), verticalScrollBar()->value());
	QPoint origPos = scrollPoint;

	if (_xScroll || qAbs (diff.x()) > 25) {
		scrollPoint.setX (scrollPoint.x() - diff.x());
		_mousePos.setX (e->pos().x());

		if (!_xScroll) {
			_xScroll = true;
			onScrollStarted();
		}
	}

	if (_yScroll || qAbs (diff.y()) > 25) {
		scrollPoint.setY (scrollPoint.y() - diff.y());
		_mousePos.setY (e->pos().y());

		if (!_yScroll) {
			onScrollStarted();
			_yScroll = true;
		}
	}

	if (scrollPoint != origPos) {
		verticalScrollBar()->setValue(scrollPoint.y());
		horizontalScrollBar()->setValue(scrollPoint.x());
		onScrollChanged();
	}
}

void NSRScrollArea::mouseReleaseEvent(QMouseEvent *e)
{
	QScrollArea::mouseReleaseEvent(e);

	_yScroll = false;
	_xScroll = false;

	onScrollStopped();
}

void NSRScrollArea::updateScrollBars()
{
	if (widget()->width() <= width())
		_horizontalBar->hide();

	if (widget()->height() <= height())
		_verticalBar->hide();

	_horizontalBar->setRange(horizontalScrollBar()->minimum(),
				 horizontalScrollBar()->maximum());
	_horizontalBar->setPageStep(width());
	_verticalBar->setRange(verticalScrollBar()->minimum(),
			       verticalScrollBar()->maximum());
	_verticalBar->setPageStep(height());

	_horizontalBar->resize(width() - _verticalBar->width(), 10);
	_horizontalBar->move(QPoint (0, height() - _horizontalBar->height()));

	_verticalBar->resize(10, height() - _horizontalBar->height());
	_verticalBar->move(QPoint (width() - _verticalBar->width(), 0));
}

void NSRScrollArea::onScrollStarted()
{
	updateScrollBars();

	if (widget()->width() > width())
		_horizontalBar->show();

	if (widget()->height() > height())
		_verticalBar->show();
}

void NSRScrollArea::onScrollChanged()
{
	_horizontalBar->setValue(horizontalScrollBar()->value());
	_verticalBar->setValue(verticalScrollBar()->value());
}

void NSRScrollArea::onScrollStopped()
{
	_horizontalBar->hide();
	_verticalBar->hide();
}

void NSRScrollArea::resizeEvent(QResizeEvent *ev)
{
	QScrollArea::resizeEvent(ev);
	updateScrollBars();
}
