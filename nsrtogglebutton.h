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

#ifndef NSRTOGGLEBUTTON_H
#define NSRTOGGLEBUTTON_H

#include <QPushButton>

#ifdef Q_OS_SYMBIAN
#  include "touchfeedback.h"
#endif

class NSRToggleButton : public QPushButton
{
	Q_OBJECT
public:
	explicit NSRToggleButton(QWidget *parent = 0);
	void setTogglePixmaps (const QPixmap& on_pxm, const QPixmap& off_pxm);
	void setChecked(bool checked);

private slots:
	void onClicked(bool checked);
	void checkStateChanged(bool checked);

private:
	QIcon		_icon_on;
	QIcon		_icon_off;
#ifdef Q_OS_SYMBIAN
	MTouchFeedback	*_iTouchFeedback;
#endif

};

#endif // NSRTOGGLEBUTTON_H
