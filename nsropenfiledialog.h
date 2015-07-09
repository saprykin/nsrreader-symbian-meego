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

#ifndef NSROPENFILEDIALOG_H
#define NSROPENFILEDIALOG_H

#include "nsrdirmodel.h"

#include <QWidget>
#include <QFileSystemModel>
#include <QEventLoop>

namespace Ui {
class NSROpenFileDialog;
}

class NSROpenFileDialog : public QWidget
{
	Q_OBJECT
	
public:
	explicit NSROpenFileDialog (QWidget *parent = 0);
	virtual ~NSROpenFileDialog ();

	void setRootPath (const QString& path);
	void setNameFilters (const QStringList& filters);
	void execDialog ();
	QString getSelectedFile () const;

protected:
	void paintEvent (QPaintEvent *ev);

private slots:
	void onItemClicked (const QModelIndex& index);
	void onFileSelected ();
	void onDirChanged (const QString& dir);
	void on_cancelButton_clicked ();

private:
	Ui::NSROpenFileDialog	*ui;
	QEventLoop		_loop;
	NSRDirModel		*_model;
};

#endif // NSROPENFILEDIALOG_H
