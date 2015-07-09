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

#ifndef NSRINPUTDIALOG_H
#define NSRINPUTDIALOG_H

#include <QWidget>
#include <QEventLoop>

namespace Ui {
class NSRInputDialog;
}

class NSRInputDialog : public QWidget
{
	Q_OBJECT
	
public:
	enum InputType {
		INPUT_TYPE_ALPHABETIC	= 1,
		INPUT_TYPE_NUMERIC	= 2
	};

	enum ExitReason {
		EXIT_REASON_OK		= 1,
		EXIT_REASON_CANCEL	= 2
	};

	explicit NSRInputDialog (QWidget *parent = 0);
	~NSRInputDialog ();

	void paintEvent (QPaintEvent *ev);
	void execDialog ();

	void setPasswordMode (bool isPass);
	void setInputMode (InputType mode);
	void setInputText (const QString& text);
	void setPromptText (const QString& text);
	QString getInputString () const;
	ExitReason getExitReason () const;

protected:
	bool eventFilter (QObject *obj, QEvent *ev);
	void mousePressEvent (QMouseEvent *ev);
	
private slots:
	void on_okButton_clicked ();
	void on_cancelButton_clicked ();

private:
	Ui::NSRInputDialog	*ui;
	ExitReason		_exitReason;
	QEventLoop		_loop;
	bool			_focusSpy;
};

#endif // NSRINPUTDIALOG_H
