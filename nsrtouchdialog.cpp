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

#include "nsrtouchdialog.h"
#include "nsrlanguage.h"
#include "ui_nsrtouchdialog.h"

NSRTouchDialog::NSRTouchDialog(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::NSRTouchDialog), _widget (NULL)
{
	ui->setupUi(this);

	_closeBtn = new QPushButton (NSRLanguage::instance()->getLanguage()->touchClose, this);
	updateButtonPosition();

	connect (_closeBtn, SIGNAL (clicked ()), this, SLOT (onCloseRequest()));

#ifdef Q_OS_SYMBIAN
	_iTouchFeedback = MTouchFeedback::Instance();
#endif

	_closeBtn->setStyleSheet ("QPushButton {"
				  "background-color: qlineargradient(spread:pad, x1:0.5, y1:0, x2:0.5, y2:1, stop:0 rgba(80, 80, 80, 255), stop:1 rgba(0, 0, 0, 255));"
				  "color: rgb(255, 255, 255);"
				  "border-top: none;"
				  "border-left: 1px solid rgbs(0, 0, 0, 255);"
				  "border-bottom: 1px solid rgbs(80, 80, 80, 255);"
				  "border-right: 1px solid rgbs(0, 0, 0, 255);"
#ifdef Q_OS_SYMBIAN
				  "font-size: 18px;"
#else
				  "font-size: 21px;"
#endif
				  "border-top: 1px solid rgbs(150, 150, 150, 255);}"
				  "QPushButton:pressed:hover {"
				  "background-color: rgb(0, 140, 205);}");
}

NSRTouchDialog::~NSRTouchDialog()
{
	delete ui;
}

void NSRTouchDialog::resizeEvent(QResizeEvent *ev)
{
	updateButtonPosition();
	ev->accept();
}

void NSRTouchDialog::updateButtonPosition()
{
	_closeBtn->setFixedHeight(ui->titleLabel->height() * 3 / 4);

	_closeBtn->move(width() - _closeBtn->width() * 5 / 4,
			ui->titleLabel->height() / 2 - _closeBtn->height() / 2);
}

void NSRTouchDialog::setTitleIcon(const QPixmap &icon)
{
	ui->titleIcon->setPixmap(icon);
}

void NSRTouchDialog::setTitle(const QString &title)
{
	ui->titleLabel->setText(title);
}

void NSRTouchDialog::setContentsWidget(QWidget *widget)
{
	if (widget == NULL && _widget == NULL)
		return;

	if (_widget != NULL) {
		ui->verticalLayout->removeWidget(_widget);
		_widget = NULL;
	}

	if (widget != NULL) {
		ui->verticalLayout->insertWidget(1, widget);
		_widget = widget;
	}
}

void NSRTouchDialog::onCloseRequest()
{
#ifdef Q_OS_SYMBIAN
	_iTouchFeedback->InstantFeedback(ETouchFeedbackBasic);
#endif
	emit closeRequested();
}
