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

#ifndef NSRDOCUMENTMENU_H
#define NSRDOCUMENTMENU_H

#include <QWidget>

#ifdef Q_OS_SYMBIAN
#  include "touchfeedback.h"
#endif

namespace Ui {
class NSRDocumentMenu;
}

class NSRDocumentMenu : public QWidget
{
	Q_OBJECT

public:
	explicit NSRDocumentMenu(QWidget *parent = 0);
	~NSRDocumentMenu();

signals:
	void fitToWidthRequested();
	void rotateLeftRequested();
	void rotateRightRequested();
	void zoomToRequested();

private slots:
	void on_fitToWidthButton_clicked();
	void on_rotateLeftButton_clicked();
	void on_rotateRightButton_clicked();
	void on_zoomToButton_clicked();

private:
	Ui::NSRDocumentMenu	*ui;
#ifdef Q_OS_SYMBIAN
	MTouchFeedback		*_iTouchFeedback;
#endif
};

#endif // NSRDOCUMENTMENU_H
