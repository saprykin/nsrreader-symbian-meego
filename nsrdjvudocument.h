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

#ifndef NSRDJVUDOCUMENT_H
#define NSRDJVUDOCUMENT_H

#include <QObject>

#include "nsrabstractdocument.h"

#ifdef Q_OS_SYMBIAN
#  include "fbs.h"
#endif

#include <libdjvu/ddjvuapi.h>
#include <libdjvu/miniexp.h>

enum NSRDjVuErrorType {
	NSR_DJVU_ERROR_NONE	= 0,
	NSR_DJVU_ERROR_FILENAME	= 1,
	NSR_DJVU_ERROR_OTHER	= 2
};

struct NSRDjVuError {
	NSRDjVuErrorType	type;
	QString			text;
};

class NSRDjVuDocument : public NSRAbstractDocument
{
	Q_OBJECT
public:
	NSRDjVuDocument(const QString& file, QObject *parent = 0);
	~NSRDjVuDocument();
	int getNumberOfPages() const;
	void renderPage(int page);
	QPixmap getCurrentPage();
	bool isValid() const;
	int getMaxZoom();
	int getMinZoom();
	QString getText();
	void setZoom(int zoom);

private:
	void handleEvents (ddjvu_context_t* context, bool wait, NSRDjVuError *error);
	void handleMessage (const ddjvu_message_t *msg, NSRDjVuError *error);
	void waitForMessage (ddjvu_context_t* context, ddjvu_message_tag_t message, NSRDjVuError *error);
	QSize getPageSize (int page);

#ifdef Q_OS_SYMBIAN
	CFbsBitmap		*_iBitmap;
#endif
	ddjvu_context_t		*_context;
	ddjvu_document_t	*_doc;
	ddjvu_page_t		*_page;
	ddjvu_format_t		*_format;
	ddjvu_render_mode_t	_renderMode;
	bool			_readyForLoad;
	QSize			_cachedPageSize;
	int			_cachedMinZoom;
	int			_cachedMaxZoom;
	int			_cachedResolution;
	int			_pageCount;
	char			*_imgData;
	QSize			_imgSize;
	QString			_text;
};

#endif // NSRDJVUDOCUMENT_H
