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

#ifndef NSRLISTSELECTOR_H
#define NSRLISTSELECTOR_H

#include <QWidget>
#include <QEventLoop>
#include <QModelIndex>

#include "nsrlistmodel.h"

namespace Ui {
class NSRListSelector;
}

class NSRListSelector : public QWidget
{
	Q_OBJECT
	
public:
	explicit NSRListSelector(QWidget *parent = 0);
	~NSRListSelector();

	void execDialog ();
	void setPromptText (const QString& text);
	void setItems (const QStringList& list);
	int getSelectedIndex () const;

protected:
	void paintEvent (QPaintEvent *ev);
	
private slots:
	void on_cancelButton_clicked ();
	void onItemClicked (const QModelIndex& idx);

private:
	Ui::NSRListSelector	*ui;
	QEventLoop		_loop;
	NSRListModel		*_model;
	int			_selectedIdx;
};

#endif // NSRLISTSELECTOR_H
