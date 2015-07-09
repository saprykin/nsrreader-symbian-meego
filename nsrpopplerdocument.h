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

#ifndef NSRPOPPLERDOCUMENT_H
#define NSRPOPPLERDOCUMENT_H

#include <QObject>
#include <QImage>

#ifdef Q_OS_SYMBIAN
#  include "fbs.h"
#endif

#include "nsrabstractdocument.h"
#include "poppler/poppler/PDFDoc.h"
#include "poppler/poppler/SplashOutputDev.h"
#include "poppler/poppler/TextOutputDev.h"
#include "poppler/poppler/OutputDev.h"
#include "poppler/splash/SplashTypes.h"
#include "poppler/splash/SplashBitmap.h"
#include "poppler/poppler/GlobalParams.h"

class NSRPopplerDocument : public NSRAbstractDocument
{
	Q_OBJECT
public:
	NSRPopplerDocument(const QString& file, QObject *parent = 0);
	~NSRPopplerDocument();
	int getNumberOfPages() const;
	void renderPage(int page);
	QPixmap getCurrentPage();
	bool isValid() const;
	int getMaxZoom();
	int getMinZoom();
	QString getText();
	void setPassword(const QString &passwd);

private:
	void createInternalDoc(QString passwd = QString());

#ifdef Q_OS_SYMBIAN
        CFbsBitmap	*_iBitmap;
#endif
	static int	_refcount;
	PDFDoc		*_doc;
	Catalog		*_catalog;
	Page		*_page;
	SplashOutputDev *_dev;
	int		_dpix;
	int		_dpiy;
	bool		_readyForLoad;
	QSize		_cachedPageSize;
	int		_cachedMinZoom;
	int		_cachedMaxZoom;
	QString		_text;
};

#endif // NSRPOPPLERDOCUMENT_H
