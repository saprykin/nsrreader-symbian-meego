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

#include "nsrtogglebutton.h"

NSRToggleButton::NSRToggleButton(QWidget *parent) :
	QPushButton(parent)
{
	setCheckable(true);
	setIconSize(QSize (76, 32));

	setTogglePixmaps(QPixmap(":/icons/icons/toggle-on.png"),
			 QPixmap(":/icons/icons/toggle-off.png"));

	connect (this, SIGNAL (clicked(bool)), this, SLOT (onClicked(bool)));

#ifdef Q_OS_SYMBIAN
	_iTouchFeedback = MTouchFeedback::Instance();
#endif
}

void NSRToggleButton::setTogglePixmaps(const QPixmap &on_pxm, const QPixmap &off_pxm)
{
	_icon_on = QIcon (on_pxm);
	_icon_off = QIcon (off_pxm);

	if (isChecked())
		setIcon(_icon_on);
	else
		setIcon(_icon_off);
}

void NSRToggleButton::checkStateChanged(bool checked)
{
	if (checked)
		setIcon (_icon_on);
	else
		setIcon (_icon_off);
}

void NSRToggleButton::onClicked(bool checked)
{
#ifdef Q_OS_SYMBIAN
	_iTouchFeedback->InstantFeedback(ETouchFeedbackBasic);
#endif

	checkStateChanged(checked);
}

void NSRToggleButton::setChecked(bool checked)
{
	QPushButton::setChecked(checked);
	checkStateChanged(checked);
}
