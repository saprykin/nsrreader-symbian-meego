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

#include "nsrabstractdocument.h"

#include <QTextCodec>

#define NSR_DOCUMENT_ZOOM_STEP	25

NSRAbstractDocument::NSRAbstractDocument(const QString& file, QObject *parent) :
	QObject (parent),
	_docPath (file),
	_zoom (100),
	_screenWidth (360),
	_zoomToWidth (false),
	_textOnly (false),
	_invertedColors (false),
	_lastError (NSR_DOCUMENT_ERROR_NO),
	_encoding ("UTF-8"),
	_rotation (0)
{
}

NSRAbstractDocument::~NSRAbstractDocument()
{

}

int NSRAbstractDocument::ZoomIn()
{
	if (!isValid())
		return _zoom;

	if (_zoom == getMaxZoom())
		return _zoom;

	if (_zoom % getZoomStep() != 0)
		_zoom = ((_zoom / getZoomStep()) + 1) * getZoomStep();
	else
		_zoom += getZoomStep();

	if (_zoom > getMaxZoom())
		_zoom = getMaxZoom();

	_zoomToWidth = false;

	return _zoom;
}

int NSRAbstractDocument::ZoomOut()
{
	if (!isValid())
		return _zoom;

	if (_zoom == getMinZoom())
		return _zoom;

	if (_zoom % getZoomStep() != 0)
		_zoom = (_zoom / getZoomStep()) * getZoomStep();
	else
		_zoom -= getZoomStep();

	if (_zoom < getMinZoom())
		_zoom = getMinZoom();

	_zoomToWidth = false;

	return _zoom;
}

void NSRAbstractDocument::zoomToWidth(int screenWidth)
{
	if (screenWidth < 0)
		_zoomToWidth = false;
	else {
		_screenWidth = screenWidth;
		_zoomToWidth = true;
	}
}

void NSRAbstractDocument::rotateLeft()
{
	_rotation -= 90;

	if (_rotation == -360)
		_rotation = 0;
}

void NSRAbstractDocument::rotateRight()
{
	_rotation += 90;

	if (_rotation == 360)
		_rotation = 0;
}

void NSRAbstractDocument::setRotation(int angle)
{
	_rotation = angle < 0 ? -((-angle) % 360) : angle % 360;
}

void NSRAbstractDocument::setEncoding(const QString &enc)
{
	if (QTextCodec::codecForName(enc.toAscii()) == NULL)
		return;
	else
		_encoding = enc;
}

void NSRAbstractDocument::setZoom(int zoom)
{
	if (zoom > getMaxZoom())
		zoom = getMaxZoom();
	else if (zoom < getMinZoom())
		zoom = getMinZoom();

	_zoom = zoom;
	_zoomToWidth = false;
}

int NSRAbstractDocument::getZoomStep() const
{
	return NSR_DOCUMENT_ZOOM_STEP;
}

QString NSRAbstractDocument::processText(const QString &text)
{
	int		size;
	QString		buf;
	QRegExp		endSigns;

	size = text.length();
	endSigns = QRegExp("[\\.\\;\\.{3}\\)]");

	for (int i = 0; i < size; ++i) {
		/* Check for previous spaces */
		if (text.at(i) == ' ') {
			if (i > 0 && buf.size() > 0 &&
			    (buf.at(buf.size() - 1) == ' ' || buf.at(buf.size() - 1) == '\n'))
				continue;

		}

		/* Check for extra new lines */
		if (text.at(i) == '\n' && i < size - 1 && buf.size() > 0) {
			int nextIdx = i + 1;

			/* Skip foreward spaces */
			while (nextIdx < (size - 1) && text.at(nextIdx) == ' ')
				++nextIdx;

			if ((endSigns.indexIn(buf.at(buf.size() - 1)) == -1 || buf.at(buf.size() - 1).isSpace()) &&
			     text.at(nextIdx).isLower()) {

				if (buf.at(buf.size() - 1) != ' ')
					buf += ' ';

				continue;
			}
		}

		buf += text.at(i);

		if (text.at(i) == '\n')
			buf += "   ";
	}

	return buf;
}
