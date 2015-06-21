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
