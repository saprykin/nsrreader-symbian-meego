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
