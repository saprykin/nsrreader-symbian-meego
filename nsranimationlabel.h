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

#ifndef NSRANIMATIONLABEL_H
#define NSRANIMATIONLABEL_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimerEvent>
#include <QPaintEvent>

class NSRAnimationLabel : public QLabel
{
	Q_OBJECT
public:
	NSRAnimationLabel (QWidget* parent);
	virtual ~NSRAnimationLabel();

public slots:
	void start( );
	void stop ();

protected:
	void timerEvent (QTimerEvent *ev);
	void paintEvent (QPaintEvent *ev);

private:
	void initData();

	QList<QPixmap *>	_pixmaps;
	int			_curIndex;
	bool			_isRunning;
	int			_timeId;
};

#endif // NSRANIMATIONLABEL_H
