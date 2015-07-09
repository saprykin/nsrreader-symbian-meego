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

#ifndef NSRINFOMESSAGEBOX_H
#define NSRINFOMESSAGEBOX_H

#include <QWidget>

namespace Ui {
class NSRInfoMessageBox;
}

class NSRInfoMessageBox : public QWidget
{
	Q_OBJECT
	
public:
	explicit NSRInfoMessageBox (QWidget		*parent,
				    const QString&	title,
				    const QString&	text);
	~NSRInfoMessageBox ();
	void showMessage ();

protected:
	void paintEvent (QPaintEvent *ev);
	
private slots:
	void on_okButton_clicked();
	void on_cancelButton_clicked();

private:
	Ui::NSRInfoMessageBox	*ui;
};

#endif // NSRINFOMESSAGEBOX_H
