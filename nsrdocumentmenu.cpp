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

#include "nsrdocumentmenu.h"
#include "nsrlanguage.h"
#include "ui_nsrdocumentmenu.h"

NSRDocumentMenu::NSRDocumentMenu(QWidget *parent) :
	QWidget (parent),
	ui (new Ui::NSRDocumentMenu)
{
	ui->setupUi (this);
#ifdef Q_OS_SYMBIAN
	_iTouchFeedback = MTouchFeedback::Instance ();
#endif

	QString style =  "QPushButton { "
			 "background-color: rgba(0, 0, 0, 200); "
			 "color: rgba(255, 255, 255, 255); "
			 "border: none; "
#ifdef Q_OS_SYMBIAN
			 "font-size: 18px; "
#else
			 "font-size: 21px; "
#endif
			 "text-align: left; "
			 "padding-left: 30px; "
			 "} "
			 "QPushButton:pressed:hover { "
			 "background-color: rgb(0, 140, 205); "
			 "}";

	setStyleSheet (style);

	const NSRLanguageData *lang = NSRLanguage::instance()->getLanguage();

	ui->fitToWidthButton->setText (lang->contextFitToWidth);
	ui->zoomToButton->setText (lang->contextZoomTo);
	ui->rotateLeftButton->setText (lang->contextRotateLeft);
	ui->rotateRightButton->setText (lang->contextRotateRight);

#ifdef Q_WS_X11
	setMinimumWidth (240);
#endif
}

NSRDocumentMenu::~NSRDocumentMenu()
{
	delete ui;
}

void NSRDocumentMenu::on_fitToWidthButton_clicked()
{
#ifdef Q_OS_SYMBIAN
	_iTouchFeedback->InstantFeedback(ETouchFeedbackBasic);
#endif
	hide();
	emit fitToWidthRequested();
}

void NSRDocumentMenu::on_rotateLeftButton_clicked()
{
#ifdef Q_OS_SYMBIAN
	_iTouchFeedback->InstantFeedback(ETouchFeedbackBasic);
#endif
	hide();
	emit rotateLeftRequested();
}

void NSRDocumentMenu::on_rotateRightButton_clicked()
{
#ifdef Q_OS_SYMBIAN
	_iTouchFeedback->InstantFeedback(ETouchFeedbackBasic);
#endif
	hide();
	emit rotateRightRequested();
}

void NSRDocumentMenu::on_zoomToButton_clicked()
{
#ifdef Q_OS_SYMBIAN
	_iTouchFeedback->InstantFeedback(ETouchFeedbackBasic);
#endif
	hide();
	emit zoomToRequested();
}
