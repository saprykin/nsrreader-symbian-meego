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

#include "nsraboutdialog.h"
#include "nsrsettings.h"
#include "nsrlanguage.h"
#include "ui_nsraboutdialog.h"

NSRAboutDialog::NSRAboutDialog (QWidget *parent) :
    NSRTouchDialog (parent),
    ui (new Ui::NSRAboutDialog)
{
	const NSRLanguageData *lang = NSRLanguage::instance()->getLanguage();

	QWidget *widget = new QWidget (this);
	ui->setupUi (widget);

	setTitleIcon (QPixmap(":/icons/icons/about.png"));
	setTitle (NSRLanguage::instance()->getLanguage()->frameAbout);
	setContentsWidget (widget);

	ui->developerLabel->setText (lang->aboutDeveloperName);
	ui->contactsLabel->setText (lang->aboutContacts + " <nsr.reader@gmail.com>");
	ui->iconsCreditLabel->setText (lang->aboutIconsNote + " Stuttgart pack, Axialis Team");
	ui->licenseLabel->setText (lang->aboutLicense);
	ui->facebookLabel->setText (lang->aboutFacebookNote);
	ui->versionLabel->setText (QString("NSR Reader %1").arg(NSRSettings::getVersion()));

	QString style = "QLabel {"
			"background-color: white; "
			"border: 1px solid rgba(180, 180, 180, 255); "
			"border-right: none; "
			"border-left: none; "
			"border-top: none; "
			"color: rgb(0, 0, 0); "
#ifdef Q_OS_SYMBIAN
			"font-size: 16px; "
			"font-weight: bold; "
#else
			"font-size: 22px; "
#endif
			"}";

	ui->developerLabel->setStyleSheet (style);
	ui->contactsLabel->setStyleSheet (style);
	ui->iconsCreditLabel->setStyleSheet (style);
	ui->facebookLabel->setStyleSheet (style);
	ui->versionLabel->setStyleSheet (style);
}

NSRAboutDialog::~NSRAboutDialog ()
{
	delete ui;
}
