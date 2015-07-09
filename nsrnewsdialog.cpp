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

#include "nsrnewsdialog.h"
#include "nsrsettings.h"
#include "nsrlanguage.h"
#include "ui_nsrnewsdialog.h"

NSRNewsDialog::NSRNewsDialog(QWidget *parent) :
	NSRTouchDialog(parent),
        ui(new Ui::NSRNewsDialog)
{
	const NSRLanguageData *lang = NSRLanguage::instance()->getLanguage();

	QWidget *widget = new QWidget (this);
	ui->setupUi (widget);

	setTitleIcon (QPixmap(":/icons/icons/nsrlogo-title.png"));
	setTitle (lang->newsTitle);
	setContentsWidget (widget);

	QString style = "QLabel { "
			"background-color: white; "
			"border: 1px solid rgbs(180, 180, 180, 255); "
			"border-right:none; "
			"border-left:none; "
			"border-top:none; "
			"color: rgb(255, 0, 0); "
#ifdef Q_OS_SYMBIAN
			"font: bold 16px; "
#else
			"font-size: 21px; "
#endif
			"}";

	QString style2 = "QLabel { "
			 "background-color: white; "
			 "border: 1px solid rgbs(180, 180, 180, 255); "
			 "border-right:none; "
			 "border-left:none; "
			 "border-top:none; "
			 "color: rgb(0, 0, 0); "
#ifdef Q_OS_SYMBIAN
			 "font: bold 16px; "
#else
			 "font-size: 21px; "
#endif
			 "}";

	ui->welcomeLabel->setStyleSheet (style);
	ui->newsTitleLabel->setStyleSheet (style2);
	ui->newsLabel1->setStyleSheet (style2);

	ui->welcomeLabel->setText ((lang->newsWelcome + " NSR Reader %1!").arg (NSRSettings::getVersion()));
	ui->newsTitleLabel->setText (lang->newsTitleLabel);
	ui->newsLabel1->setText (lang->newsFeature1);
}

NSRNewsDialog::~NSRNewsDialog()
{
	delete ui;
}
