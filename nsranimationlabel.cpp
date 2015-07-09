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

#include "nsranimationlabel.h"

#include <QPainter>

NSRAnimationLabel::NSRAnimationLabel (QWidget *parent) :
	QLabel (parent),
	_curIndex (0),
	_isRunning (false),
	_timeId (0)
{
	initData ();
	setStyleSheet (QString ("QLabel {background-color: transparent;}"));
}

NSRAnimationLabel::~NSRAnimationLabel()
{
	if (_isRunning)
		stop ();

	while (!_pixmaps.isEmpty())
		delete _pixmaps.takeFirst();
}


void NSRAnimationLabel::start()
{
	if (_isRunning)
		return;

	_isRunning = true;
	_curIndex = 0;

	_timeId = startTimer (120);
}


void NSRAnimationLabel::stop()
{
	if (!_isRunning)
		return;

	_isRunning = false;

	killTimer (_timeId);
}

void NSRAnimationLabel::timerEvent (QTimerEvent *ev)
{
	if (ev->timerId() != _timeId || !_isRunning)
		return;

//	setPixmap (*(_pixmaps.at (_curIndex)));
	repaint();


	if (_curIndex < _pixmaps.count () - 1)
		++_curIndex;
	else
		_curIndex = 0;
}

void NSRAnimationLabel::paintEvent (QPaintEvent *ev)
{
	QLabel::paintEvent (ev);

	if (!_isRunning)
		return;

	QPainter painter (this);
	QPixmap *pxm = _pixmaps.at(_curIndex);

	painter.drawPixmap (width() / 2 - pxm->width() / 2,
			    height() / 2 - pxm->height() / 2,
			    *pxm);
}

void NSRAnimationLabel::initData ()
{
	setHidden (true);
	setAlignment (Qt::AlignHCenter | Qt::AlignVCenter);

	for (int i = 0; i < 10; ++i)
		_pixmaps.append (new QPixmap (":/icons/icons/spinner-" + QString::number (i + 1) + ".png"));
}
