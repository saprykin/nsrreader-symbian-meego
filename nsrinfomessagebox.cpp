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

#include "nsrinfomessagebox.h"
#include "ui_nsrinfomessagebox.h"
#include "nsrlanguage.h"

#include <QPainter>

NSRInfoMessageBox::NSRInfoMessageBox (QWidget		*parent,
				      const QString&	title,
				      const QString&	text) :
	QWidget	(parent),
	ui (new Ui::NSRInfoMessageBox)
{
	const NSRLanguageData *lang = NSRLanguage::instance()->getLanguage ();

	ui->setupUi(this);
	ui->msgTitle->setText(title);
	ui->msgLabel->setText(text);
	ui->cancelButton->setVisible(false);
	ui->cancelButton->setText (lang->mainCancel);
}

NSRInfoMessageBox::~NSRInfoMessageBox()
{
	delete ui;
}

void NSRInfoMessageBox::showMessage()
{
	QEventLoop loop;

	show ();

	connect (ui->okButton, SIGNAL (clicked ()), &loop, SLOT (quit ()));
	connect (ui->cancelButton, SIGNAL (clicked ()), &loop, SLOT (quit ()));

	loop.exec ();
	close ();
}

void NSRInfoMessageBox::paintEvent(QPaintEvent *ev)
{
	Q_UNUSED (ev);

	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void NSRInfoMessageBox::on_okButton_clicked()
{
}

void NSRInfoMessageBox::on_cancelButton_clicked()
{
}
