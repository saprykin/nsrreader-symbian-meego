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

#ifndef NSRTEXTDOCUMENT_H
#define NSRTEXTDOCUMENT_H

#include "nsrabstractdocument.h"

#include <QMap>

class NSRTextDocument : public NSRAbstractDocument
{
public:
	NSRTextDocument(const QString& file, QObject *parent = 0);
	~NSRTextDocument();

	int getNumberOfPages () const;
	void renderPage (int page);
	QPixmap getCurrentPage ();
	bool isValid ()	const;
	int getMaxZoom ();
	int getMinZoom ();
	void setTextOnly (bool textOnly);
	QString getText ();

private:
	QString			_text;
	int			_pagesCount;
};

#endif // NSRTEXTDOCUMENT_H
