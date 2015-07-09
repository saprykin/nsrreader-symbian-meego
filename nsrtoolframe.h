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

#ifndef NSRTOOLFRAME_H
#define NSRTOOLFRAME_H

#include <QFrame>

#ifdef Q_OS_SYMBIAN
#  include "touchfeedback.h"
#endif

class NSRToolFrame : public QFrame
{
    Q_OBJECT
public:
	explicit NSRToolFrame(QWidget *parent = 0);
	~NSRToolFrame();

signals:
	void gotoRequested();
	void aboutRequested();
	void preferencesRequested();
	void zoomToRequested();

public slots:
	void onExitClicked();
	void onPrefsClicked();
	void onGotoClicked();
	void onAboutClicked();

#ifdef Q_OS_SYMBIAN
private:
	MTouchFeedback	*_iTouchFeedback;
#endif
};

#endif // NSRTOOLFRAME_H
