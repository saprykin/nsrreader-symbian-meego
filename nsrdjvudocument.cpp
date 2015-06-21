#include "nsrdjvudocument.h"

#include <math.h>

#include <QString>

/* DjVu stuff for text extraction */
#define S(n,s) (n = miniexp_symbol(s))

static bool
miniexp_get_int (miniexp_t &r, int &x)
{
  if (! miniexp_numberp(miniexp_car(r)))
    return false;
  x = miniexp_to_int(miniexp_car(r));
  r = miniexp_cdr(r);
  return true;
}

static bool
miniexp_get_rect_from_points (miniexp_t &r, QRect &rect)
{
	int x1, y1, x2, y2;

	if (!(miniexp_get_int (r, x1) && miniexp_get_int (r, y1) &&
	      miniexp_get_int (r, x2) && miniexp_get_int (r, y2)))
		return false;

	if (x2 < x1 || y2 < y1)
		return false;

	rect.setCoords (x1, y1, x2, y2);
	return true;
}

static void
flatten_hiddentext_sub (miniexp_t p, minivar_t &d)
{
	QRect		rect;
	miniexp_t	type = miniexp_car (p);
	miniexp_t	r = miniexp_cdr (p);

	if (miniexp_symbolp (type) && miniexp_get_rect_from_points (r, rect)) {
		if (miniexp_stringp (miniexp_car(r)))
			d = miniexp_cons (p, d);
		else {
			while (miniexp_consp (r)) {
				flatten_hiddentext_sub (miniexp_car (r), d);
				r = miniexp_cdr (r);
			}

			d = miniexp_cons (type, d);
		}
	}
}

/*
 * Output list contains
 * - terminals of the hidden text tree
 *   (keyword x1 y1 x2 y2 string)
 * - or a keyword symbol indicating the need for a separator
 *   page,column,region,para,line,word
 */
static miniexp_t
flatten_hiddentext (miniexp_t p)
{
	minivar_t d;

	flatten_hiddentext_sub (p, d);
	d = miniexp_reverse (d);

	return d;
}

NSRDjVuDocument::NSRDjVuDocument(const QString& file, QObject *parent) :
	NSRAbstractDocument (file, parent),
#ifdef Q_OS_SYMBIAN
	_iBitmap (NULL),
#endif
	_context (NULL),
	_doc (NULL),
	_page (NULL),
	_renderMode (DDJVU_RENDER_COLOR),
	_readyForLoad (false),
	_cachedPageSize (QSize (0, 0)),
	_cachedMinZoom (25),
	_cachedMaxZoom (100),
	_cachedResolution (72),
	_pageCount (0),
	_imgData (NULL)
{
	NSRDjVuError error;

	error.type = NSR_DJVU_ERROR_NONE;
	_context = ddjvu_context_create ("nsrreader");

	if (_context == NULL)
		return;

	ddjvu_cache_set_size (_context, NSR_DOCUMENT_MAX_HEAP / 10);
	_format = ddjvu_format_create (DDJVU_FORMAT_BGR24, 0, 0);

	if (_format == NULL) {
		ddjvu_context_release (_context);
		_context = NULL;
		return;
	}

	ddjvu_format_set_row_order (_format, 1);

	_doc = ddjvu_document_create_by_filename (_context, file.toUtf8().data(), true);

	if (_doc == NULL) {
		ddjvu_context_release (_context);
		ddjvu_format_release (_format);
		_context = NULL;
		_format = NULL;
		return;
	}

	waitForMessage(_context, DDJVU_DOCINFO, &error);

	if (error.type != NSR_DJVU_ERROR_NONE) {
		ddjvu_document_release (_doc);
		ddjvu_context_release (_context);
		ddjvu_format_release (_format);
		_doc = NULL;
		_context = NULL;
		_format = NULL;
		return;
	}

	error.type = NSR_DJVU_ERROR_NONE;

	if (ddjvu_document_decoding_error (_doc))
		handleEvents(_context, true, &error);

	if (error.type != NSR_DJVU_ERROR_NONE) {
		ddjvu_document_release (_doc);
		ddjvu_context_release (_context);
		ddjvu_format_release (_format);
		_doc = NULL;
		_context = NULL;
		_format = NULL;
		return;
	}

	_pageCount = ddjvu_document_get_pagenum (_doc);
#ifdef Q_OS_SYMBIAN
	_iBitmap = new CFbsBitmap ();
#endif
}

NSRDjVuDocument::~NSRDjVuDocument()
{
#ifdef Q_OS_SYMBIAN
	if (_iBitmap != NULL) {
		_iBitmap->Reset();
		delete _iBitmap;
	}
#endif

	if (_page != NULL)
		ddjvu_page_release (_page);

	if (_doc != NULL)
		ddjvu_document_release (_doc);

	if (_context != NULL)
		ddjvu_context_release (_context);

	if (_format != NULL)
		ddjvu_format_release (_format);

	if (_imgData != NULL)
		delete [] _imgData;
}


int NSRDjVuDocument::getNumberOfPages() const
{
	if (_doc == NULL)
		return 0;

	return ddjvu_document_get_pagenum (_doc);
}

bool NSRDjVuDocument::isValid() const
{
	return (_doc != NULL);
}

void NSRDjVuDocument::renderPage(int page)
{
	ddjvu_rect_t		prect;
	ddjvu_page_rotation_t	rot;
	int			tmp;
	double			resFactor;

	if (_doc == NULL || page > getNumberOfPages() || page < 1)
		return;

	_page = ddjvu_page_create_by_pageno (_doc, page - 1);

	while (!ddjvu_page_decoding_done (_page))
		handleEvents (_context, true, NULL);

	if (isTextOnly()) {
		QString		ans;
		miniexp_t	seps[7];
		miniexp_t	ptext;
		int		separator;

		S(seps[0], "page");
		S(seps[1], "column");
		S(seps[2], "region");
		S(seps[3], "para");
		S(seps[4], "line");
		S(seps[5], "word");
		S(seps[6], "char");

		while ((ptext = ddjvu_document_get_pagetext (_doc, page - 1, 0)) == miniexp_dummy)
			handleEvents (_context, true, NULL);

		ptext = flatten_hiddentext (ptext);
		separator = 6;

		while (miniexp_consp (ptext)) {
			QRect		rect;
			miniexp_t	r = miniexp_car (ptext);
			miniexp_t	type = r;

			ptext = miniexp_cdr (ptext);

			if (miniexp_consp (r)) {
				type = miniexp_car(r);
				r = miniexp_cdr(r);

				if (miniexp_symbolp (type) && miniexp_get_rect_from_points (r, rect)) {
					if (!ans.isEmpty()) {
						if (separator == 0)
							ans += "\n\f";
						else if (separator <= 4)
							ans += "\n";
						else if (separator <= 5)
							ans += " ";
					}
					separator = 6;
					ans += QString::fromUtf8 (miniexp_to_str (miniexp_car (r)));
				}
			}

			for (int s = separator - 1; s >= 0; s--)
				if (type == seps[s])
					separator = s;
		}

		ddjvu_miniexp_release (_doc, ptext);
		ddjvu_page_release (_page);

		_text		= processText (ans);
		_readyForLoad	= true;
		_page		= NULL;

		return;
	}

	int width = ddjvu_page_get_width (_page);
	int height = ddjvu_page_get_height (_page);

	switch (getRotation()) {
	case 0:
		rot = DDJVU_ROTATE_0;
		break;
	case 90:
	case -270:
		rot = DDJVU_ROTATE_270;
		tmp = width;
		width = height;
		height = tmp;
		break;
	case 180:
	case -180:
		rot = DDJVU_ROTATE_180;
		break;
	case 270:
	case -90:
		rot = DDJVU_ROTATE_90;
		tmp = width;
		width = height;
		height = tmp;
		break;
	default:
		rot = DDJVU_ROTATE_0;
		break;
	}

	ddjvu_page_set_rotation (_page, rot);

	_cachedPageSize = QSize (width, height);
	_cachedResolution = ddjvu_page_get_resolution (_page);

	resFactor = 72.0 / _cachedResolution;

	if (isZoomToWidth()) {
		double wZoom = ((double) getScreenWidth() / ((double) width * resFactor) * 100.0);
		setZoomSilent((int) wZoom);
	}

	if (getZoom() > getMaxZoom())
		setZoomSilent (getMaxZoom());
	else if (getZoom() < getMinZoom())
		setZoomSilent (getMinZoom());

	prect.w = (int)((double) width * getZoom() / 100.0 * resFactor);
	prect.h = (int)((double) height * getZoom() / 100.0 * resFactor);

#ifdef Q_OS_SYMBIAN
	TSize newSize (prect.w, prect.h);
#else
	QSize newSize (prect.w, prect.h);
#endif

	if (_imgData != NULL)
		delete [] _imgData;

#ifdef Q_OS_SYMBIAN
	if (newSize.iWidth * newSize.iHeight * 3 > NSR_DOCUMENT_MAX_HEAP) {
#else
	if (newSize.width () * newSize.height () * 3 > NSR_DOCUMENT_MAX_HEAP) {
#endif
		ddjvu_page_release (_page);

		_imgData = NULL;
		_page = NULL;

		return;
	}

#ifdef Q_OS_SYMBIAN
	_imgData = new char[newSize.iWidth * newSize.iHeight * 3];
	TInt rowSize = newSize.iWidth * 3;
#else
	_imgData = new char[newSize.width () * newSize.height () * 3];
	int rowSize = newSize.width () * 3;
#endif

	int result = ddjvu_page_render (_page, _renderMode, &prect, &prect, _format, rowSize, _imgData);

	if (!result)
		memset(_imgData, 0xFF, rowSize * prect.h);

	ddjvu_page_release (_page);

#ifdef Q_OS_SYMBIAN
	_imgSize = QSize (newSize.iWidth, newSize.iHeight);
#else
	_imgSize = newSize;
#endif
	_page = NULL;
	_readyForLoad = true;
}

int NSRDjVuDocument::getMaxZoom()
{
	if (_doc == NULL)
		return 0;

	if (_cachedPageSize == QSize (0, 0))
		return 200;

	/* Each pixel needs 3 bytes (RGB) of memory */
	double resFactor = 72.0 / _cachedResolution;
	double pageSize = _cachedPageSize.width() * _cachedPageSize.height() * 3  * resFactor / 2;
	_cachedMaxZoom = (int) (sqrt (NSR_DOCUMENT_MAX_HEAP * 72 * 72 / pageSize) / 72 * 100 + 0.5);

	if (_cachedMaxZoom > 400)
		_cachedMaxZoom = 400;

	return _cachedMaxZoom;
}

int NSRDjVuDocument::getMinZoom()
{
	if (_cachedPageSize == QSize (0, 0))
		return 25;

	/* Each pixel needs 3 bytes (RGB) of memory */
	double pageSize = _cachedPageSize.width() * _cachedPageSize.height()
			  * 3 * 72 / _cachedResolution;

	if (pageSize > NSR_DOCUMENT_MAX_HEAP)
		_cachedMinZoom = getMaxZoom();
	else
		_cachedMinZoom = (getMaxZoom() / 10) > 25 ? 25 : getMaxZoom() / 10;

	return _cachedMinZoom;
}

QPixmap NSRDjVuDocument::getCurrentPage()
{
	if (!_readyForLoad)
		return QPixmap();

	if (_imgData == NULL)
		return QPixmap ();

#ifdef Q_OS_SYMBIAN
	if (_iBitmap->SizeInPixels() != TSize (_imgSize.width(), _imgSize.height())) {
		_iBitmap->Reset();
		_iBitmap->Create (TSize (_imgSize.width(), _imgSize.height()), EColor16M);
	}

	_iBitmap->LockHeap();

	TInt rowSize = _iBitmap->DataStride();
	char *image = (char*) _iBitmap->DataAddress();

	for (int i = 0; i < _imgSize.height(); ++i)
		for (int j = 0; j < _imgSize.width() * 3; ++j) {
			if (isInvertedColors())
				*(image + rowSize * i + j) = 255 - *(_imgData + i * _imgSize.width() * 3 + j);
			else
				*(image + rowSize * i + j) = *(_imgData + i * _imgSize.width() * 3 + j);
		}

	_iBitmap->UnlockHeap();
#else
	QImage img (_imgSize, QImage::Format_RGB888);

	int rowSize = img.bytesPerLine();
	char *image = (char*) img.bits();

	for (int i = 0; i < _imgSize.height(); ++i)
		for (int j = 0; j < _imgSize.width(); ++j) {
			if (isInvertedColors()) {
				*(image + rowSize * i + j * 3) = 255 - *(_imgData + i * _imgSize.width() * 3 + j * 3 + 2);
				*(image + rowSize * i + j * 3 + 1) = 255 - *(_imgData + i * _imgSize.width() * 3 + j * 3 + 1);
				*(image + rowSize * i + j * 3 + 2) = 255 - *(_imgData + i * _imgSize.width() * 3 + j * 3);
			} else {
				*(image + rowSize * i + j * 3) = *(_imgData + i * _imgSize.width() * 3 + j * 3 + 2);
				*(image + rowSize * i + j * 3 + 1) = *(_imgData + i * _imgSize.width() * 3 + j * 3 + 1);
				*(image + rowSize * i + j * 3 + 2) = *(_imgData + i * _imgSize.width() * 3 + j * 3);
			}
		}
#endif

	delete [] _imgData;
	_imgData = NULL;
	_readyForLoad = false;

#ifdef Q_OS_SYMBIAN
	return QPixmap::fromSymbianCFbsBitmap(_iBitmap);
#else
	return QPixmap::fromImage (img);
#endif
}

QString NSRDjVuDocument::getText()
{
	if (!_readyForLoad)
		return NSRAbstractDocument::getText();

	_readyForLoad = false;

	if (_text.isEmpty())
		return NSRAbstractDocument::getText();
	else
		return _text;
}

void NSRDjVuDocument::handleEvents(ddjvu_context_t *context, bool wait, NSRDjVuError *error)
{
	const ddjvu_message_t *msg;

	if (context == NULL)
		return;

	if (wait)
		ddjvu_message_wait (context);

	while ((msg = ddjvu_message_peek (context))) {
		handleMessage (msg, error);
		ddjvu_message_pop (context);

		if (error != NULL && error->type != NSR_DJVU_ERROR_NONE)
			return;
	}
}

void NSRDjVuDocument::waitForMessage(ddjvu_context_t *context, ddjvu_message_tag_t message, NSRDjVuError *error)
{
	const ddjvu_message_t *msg;

	ddjvu_message_wait (context);

	while ((msg = ddjvu_message_peek (context)) && (msg->m_any.tag != message)) {
		handleMessage(msg, error);
		ddjvu_message_pop (context);

		if (error != NULL && error->type != NSR_DJVU_ERROR_NONE)
			return;
	}

	if (msg && msg->m_any.tag == message)
		ddjvu_message_pop (context);
}

void NSRDjVuDocument::handleMessage(const ddjvu_message_t *msg, NSRDjVuError *error)
{
	if (msg == NULL)
		return;

	switch (msg->m_any.tag) {
	case DDJVU_ERROR:
	{
		if (error != NULL) {
			if (msg->m_error.filename)
				error->type = NSR_DJVU_ERROR_FILENAME;
			else
				error->type = NSR_DJVU_ERROR_OTHER;

			error->text = QString (msg->m_error.message);
		}
	}
		break;
	default:
		if (error != NULL)
			error->type = NSR_DJVU_ERROR_NONE;
		break;
	}
}

QSize NSRDjVuDocument::getPageSize(int page)
{
	ddjvu_pageinfo_t	info;
	ddjvu_status_t		r;

	if (page < 1)
		return QSize (0, 0);

	while ((r = ddjvu_document_get_pageinfo (_doc, page - 1, &info)) < DDJVU_JOB_OK)
		handleEvents (_context, true, NULL);

	if (r >= DDJVU_JOB_FAILED)
		handleEvents (_context, true, NULL);

	return QSize (info.width * 72 / _cachedResolution, info.height * 72 / _cachedResolution);
}

void NSRDjVuDocument::setZoom(int zoom)
{
	NSRAbstractDocument::setZoom(zoom);
	NSRAbstractDocument::setZoomSilent(zoom / 72 * _cachedResolution);
}
