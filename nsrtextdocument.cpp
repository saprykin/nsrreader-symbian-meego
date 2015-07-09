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

#include "nsrtextdocument.h"

#include <math.h>

#include <QFile>
#include <QFileInfo>
#include <QDataStream>
#include <QTextCodec>
#include <QDebug>

#define NSR_TEXT_PAGE_SIZE	5120
#define NSR_TEXT_MIN_ZOOM	25
#define NSR_TEXT_MAX_ZOOM	400

NSRTextDocument::NSRTextDocument (const QString &file, QObject *parent) :
	NSRAbstractDocument (file, parent),
	_pagesCount (0)
{
	QFileInfo info (file);

	setTextOnly (true);

	_pagesCount = (int) (ceil ((double) info.size() / NSR_TEXT_PAGE_SIZE) + 0.5);
}

NSRTextDocument::~NSRTextDocument ()
{
}

int NSRTextDocument::getNumberOfPages () const
{
	return _pagesCount;
}

void NSRTextDocument::renderPage (int page)
{
	if (page < 1 || page > getNumberOfPages())
		return;

	if (!isValid())
		return;

	_text = QString ();

	QFile	data (getDocumentPath());

	if (data.open(QFile::ReadOnly)) {
		QDataStream	in (&data);
		QByteArray	ba, bn;
		int		strPos;
		int		bytesRead;

		ba.resize(NSR_TEXT_PAGE_SIZE + NSR_TEXT_PAGE_SIZE / 2);
		in.device()->seek((page - 1) * NSR_TEXT_PAGE_SIZE);

		if ((bytesRead = in.readRawData(ba.data(), NSR_TEXT_PAGE_SIZE + NSR_TEXT_PAGE_SIZE / 2)) == -1) {
			data.close();
			return;
		}

		ba.truncate(bytesRead);
		bn = ba.left(NSR_TEXT_PAGE_SIZE);

		/* Complete last word */
		if (ba.size() > NSR_TEXT_PAGE_SIZE) {
			strPos = NSR_TEXT_PAGE_SIZE;
			while (strPos < ba.size() && !QChar(ba.at(strPos)).isSpace())
				bn.append(ba.at(strPos++));
		}

		_text = QTextCodec::codecForName(getEncoding().toAscii())->toUnicode(bn);

		if (!_text.isEmpty() && page > 1) {
			/* Remove previous semi-full words and spaces */
			strPos = -1;
			for (int i = 0; i < _text.size() / 2; ++i)
				if (_text.at(i).isSpace()) {
					while (_text.at(++i).isSpace() && i < _text.size() / 2);
					strPos = i;
					break;
				}

			if (strPos != -1)
				_text = _text.right (_text.size() - strPos);
		}

		data.close();
	}
}

QPixmap NSRTextDocument::getCurrentPage ()
{
	return QPixmap();
}

bool NSRTextDocument::isValid () const
{
	QFileInfo info (getDocumentPath());

	return info.exists();
}

int NSRTextDocument::getMaxZoom ()
{
	return NSR_TEXT_MAX_ZOOM;
}

int NSRTextDocument::getMinZoom ()
{
	return NSR_TEXT_MAX_ZOOM;
}

void NSRTextDocument::setTextOnly (bool textOnly)
{
	/* This is only a text document */
	Q_UNUSED (textOnly);
}

QString NSRTextDocument::getText ()
{
	return _text;
}


