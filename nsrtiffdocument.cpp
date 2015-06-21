#include "nsrtiffdocument.h"

#include <math.h>

NSRTIFFDocument::NSRTIFFDocument(const QString& file, QObject *parent) :
	NSRAbstractDocument (file, parent),
#ifdef Q_OS_SYMBIAN
	_iBitmap (NULL),
#endif
	_tiff (NULL),
	_readyForLoad (false),
	_pageCount (0),
	_cachedPage (0)
{
	if ((_tiff = TIFFOpen (file.toUtf8().data(), "r")) == NULL)
		return;

    _pageCount = 1;

	while (TIFFReadDirectory (_tiff))
		++_pageCount;

	if ((_pageCount > 0 && TIFFSetDirectory (_tiff, 0) != 0) || _pageCount == 0) {
		uint32 w, h;

		TIFFGetField (_tiff, TIFFTAG_IMAGEWIDTH, &w);
		TIFFGetField (_tiff, TIFFTAG_IMAGELENGTH, &h);

		_cachedPageSize = QSize (w, h);
	}

#ifdef Q_OS_SYMBIAN
	_iBitmap = new CFbsBitmap ();
#endif
}

NSRTIFFDocument::~NSRTIFFDocument()
{
	if (_tiff != NULL) {
		TIFFClose (_tiff);
		_tiff = NULL;
	}

#ifdef Q_OS_SYMBIAN
	if (_iBitmap != NULL) {
		_iBitmap->Reset();
		delete _iBitmap;
	}
#endif

	if (!_origImage.isNull())
		_origImage = QImage ();

	if (!_image.isNull())
		_image = QImage ();
}


int NSRTIFFDocument::getNumberOfPages() const
{
	if (_tiff == NULL)
		return 0;

	return _pageCount > 0 ? _pageCount : 1;
}

bool NSRTIFFDocument::isValid() const
{
	return (_tiff != NULL);
}

void NSRTIFFDocument::renderPage(int page)
{
	uint32	w, h;
	size_t	npixels;
	char	*imgBuf;
	QImage	*img;

	if (_tiff == NULL || page > getNumberOfPages() || page < 1)
		return;

	if (_pageCount > 0 && TIFFSetDirectory (_tiff, page - 1) == 0)
		return;

	TIFFGetField (_tiff, TIFFTAG_IMAGEWIDTH, &w);
	TIFFGetField (_tiff, TIFFTAG_IMAGELENGTH, &h);
	npixels = w * h;

	_readyForLoad = false;

	if (npixels * sizeof (uint32) > NSR_DOCUMENT_MAX_HEAP)
		return;

	if (!_image.isNull())
		_image = QImage ();

	if (isZoomToWidth()) {
		double wZoom = ((double) getScreenWidth() / (double) w * 100.0);
		setZoomSilent((int) wZoom);
	}

	if (getZoom() > getMaxZoom())
		setZoomSilent (getMaxZoom());
	else if (getZoom() < getMinZoom())
		setZoomSilent (getMinZoom());

	if (_cachedPage == page && !_origImage.isNull()) {
		QTransform trans;
		trans.scale(getZoom() / 100.0, getZoom() / 100.0);
		trans.rotate(-getRotation());
		_image = _origImage.transformed(trans);
		_readyForLoad = true;
		return;
	}

	if (!_origImage.isNull())
		_origImage = QImage ();

	imgBuf = new char[npixels * sizeof (uint32)];
	img = new QImage ((const uchar*) imgBuf, w, h, w * sizeof (uint32), QImage::Format_ARGB32);

	if (TIFFReadRGBAImage (_tiff, w, h, (uint32 *) img->bits(), 0) == 0) {
		delete img;
		delete imgBuf;
	} else {
		double scale = getZoom() / 100.0;
		QTransform trans;

		trans.scale(scale, scale);
		trans.rotate(-getRotation());

		if (_origImage.byteCount() > NSR_DOCUMENT_MAX_HEAP / (2 + scale * scale)) {
			_image = img->transformed(trans);
			_cachedPage = 0;

			delete img;
			delete imgBuf;
		} else {
			_origImage = img->copy();

			delete img;
			delete imgBuf;

			_image = _origImage.transformed(trans);
			_cachedPage = page;
		}

		_readyForLoad = true;
	}
}

int NSRTIFFDocument::getMaxZoom()
{
	if (_tiff == NULL)
		return 0;

	if (_cachedPageSize == QSize (0, 0))
		return 100;

	/* Each pixel needs 4 bytes (RGBA) of memory */
	double pageSize = _cachedPageSize.width() * _cachedPageSize.height() * 4;

	return (int) (sqrt (NSR_DOCUMENT_MAX_HEAP / pageSize - 1) * 100 + 0.5);
}

int NSRTIFFDocument::getMinZoom()
{
	if (_cachedPageSize == QSize (0, 0))
		return 25;

	/* Each pixel needs 4 bytes (RGBA) of memory */
	double pageSize = _cachedPageSize.width() * _cachedPageSize.height() * 4;

	if (pageSize > NSR_DOCUMENT_MAX_HEAP)
		return getMaxZoom();
	else
		return (getMaxZoom() / 10) > 25 ? 25 : getMaxZoom() / 10;
}

QPixmap NSRTIFFDocument::getCurrentPage()
{
	if (!_readyForLoad)
		return QPixmap();

	if (_image.isNull())
		return QPixmap ();

#ifdef Q_OS_SYMBIAN
	if (_iBitmap->SizeInPixels() != TSize (_image.width(), _image.height())) {
		_iBitmap->Reset();
		_iBitmap->Create (TSize (_image.width(), _image.height()), EColor16M);
	}

	_iBitmap->LockHeap();

	TInt rowSize = _iBitmap->DataStride();
	char *image = (char*) _iBitmap->DataAddress();

	for (int i = 0; i < _image.height(); ++i) {
		int k = 0;
		for (int j = 0; j < _image.width() * 4; ++j) {
			if (((j + 1) % 4) == 0)
				continue;

			if (isInvertedColors())
				*(image + rowSize * (_image.height() - i - 1) + k++) =
					255 - *(_image.bits() + i * _image.bytesPerLine() + j);
			else
				*(image + rowSize * (_image.height() - i - 1) + k++) =
					*(_image.bits() + i * _image.bytesPerLine() + j);
		}
	}

	_iBitmap->UnlockHeap();
	_image = QImage ();
	_readyForLoad = false;

	return QPixmap::fromSymbianCFbsBitmap(_iBitmap);
#else
	QPixmap ret = QPixmap::fromImage (_image);
	_image = QImage ();

	return ret;
#endif
}
