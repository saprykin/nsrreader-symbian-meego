#ifndef NSRTIFFDOCUMENT_H
#define NSRTIFFDOCUMENT_H

#include "nsrabstractdocument.h"
#include "tiffio.h"

class NSRTIFFDocument : public NSRAbstractDocument
{
	Q_OBJECT
public:
	NSRTIFFDocument(const QString& file, QObject *parent = 0);
	~NSRTIFFDocument();
	int getNumberOfPages() const;
	void renderPage(int page);
	QPixmap getCurrentPage();
	bool isValid() const;
	int getMaxZoom();
	int getMinZoom();

private:
#ifdef Q_OS_SYMBIAN
	CFbsBitmap		*_iBitmap;
#endif
	TIFF			*_tiff;
	bool			_readyForLoad;
	QSize			_cachedPageSize;
	int			_pageCount;
	int			_cachedPage;
	QImage			_origImage;
	QImage			_image;
};

#endif // NSRTIFFDOCUMENT_H
