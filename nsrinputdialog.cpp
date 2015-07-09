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

#include "nsrinputdialog.h"
#include "ui_nsrinputdialog.h"
#include "nsrlanguage.h"

#include <QPainter>
#include <QInputContext>

NSRInputDialog::NSRInputDialog (QWidget *parent) :
	QWidget (parent),
	ui (new Ui::NSRInputDialog),
	_exitReason (EXIT_REASON_CANCEL),
	_focusSpy (false)
{
	const NSRLanguageData *lang = NSRLanguage::instance()->getLanguage ();

	ui->setupUi (this);
	ui->inputEdit->installEventFilter (this);
	ui->cancelButton->setText (lang->mainCancel);
}

NSRInputDialog::~NSRInputDialog ()
{
	delete ui;
}

void NSRInputDialog::paintEvent (QPaintEvent *ev)
{
	Q_UNUSED (ev);

	QStyleOption opt;
	opt.init (this);
	QPainter p (this);
	style()->drawPrimitive (QStyle::PE_Widget, &opt, &p, this);
}

void NSRInputDialog::execDialog ()
{
	show ();
	ui->inputEdit->setFocus ();
	_focusSpy = true;

	_loop.exec ();
	close ();
}

void NSRInputDialog::setPasswordMode (bool isPass)
{
	ui->inputEdit->setEchoMode (isPass ? QLineEdit::Password : QLineEdit::Normal);
}

void NSRInputDialog::setInputMode (NSRInputDialog::InputType mode)
{
	switch (mode) {
	case INPUT_TYPE_NUMERIC:
		ui->inputEdit->setInputMethodHints (Qt::ImhDigitsOnly |
						    Qt::ImhNoAutoUppercase |
						    Qt::ImhNoPredictiveText);
		break;
	case INPUT_TYPE_ALPHABETIC:
	default:
		ui->inputEdit->setInputMethodHints (Qt::ImhNoAutoUppercase |
						    Qt::ImhNoPredictiveText);
		break;
	}
}

void NSRInputDialog::setInputText (const QString &text)
{
	ui->inputEdit->setText (text);
}

void NSRInputDialog::setPromptText (const QString &text)
{
	ui->promptLabel->setText (text);
}

QString NSRInputDialog::getInputString () const
{
	return ui->inputEdit->text ();
}

NSRInputDialog::ExitReason NSRInputDialog::getExitReason () const
{
	return _exitReason;
}

bool NSRInputDialog::eventFilter (QObject *obj, QEvent *ev)
{
	if (obj != ui->inputEdit)
		return QWidget::eventFilter (obj, ev);

	if (ev->type () != QEvent::FocusIn &&
	    ev->type () != QEvent::FocusOut &&
	    ev->type () != QEvent::KeyPress)
		return false;

	QInputContext *inputContext = qApp->inputContext ();

	if (inputContext == NULL)
		return false;

	if (ev->type () == QEvent::FocusIn) {
		QEvent request (QEvent::RequestSoftwareInputPanel);

		inputContext->filterEvent (&request);
		ui->inputEdit->setAttribute (Qt::WA_InputMethodEnabled, true);
		inputContext->setFocusWidget (ui->inputEdit);
	} else if (ev->type () == QEvent::FocusOut) {
		QEvent request (QEvent::CloseSoftwareInputPanel);

		inputContext->filterEvent (&request);
		inputContext->reset ();
	} else {
		QKeyEvent *keyEv = (QKeyEvent *) ev;

		if (keyEv->key () == Qt::Key_Return)
			on_okButton_clicked ();
	}

	return false;
}

void NSRInputDialog::mousePressEvent (QMouseEvent *ev)
{
	if (childAt (ev->pos ()) != ui->inputEdit && _focusSpy)
		ui->inputEdit->clearFocus ();
}

void NSRInputDialog::on_okButton_clicked ()
{
	_exitReason = EXIT_REASON_OK;
	_loop.quit ();
}

void NSRInputDialog::on_cancelButton_clicked ()
{
	_exitReason = EXIT_REASON_CANCEL;
	_loop.quit ();
}
