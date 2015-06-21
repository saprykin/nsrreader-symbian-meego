#ifndef NSRABSTRACTDOCUMENT_H
#define NSRABSTRACTDOCUMENT_H

#include <QObject>
#include <QPixmap>

#include "nsrlanguage.h"

#ifdef Q_OS_SYMBIAN
#  ifdef NSR_SYMBIAN_S3
#  define NSR_DOCUMENT_MAX_HEAP	(0x5000000 * 0.35)
#  else
#  define NSR_DOCUMENT_MAX_HEAP	(0x3000000 * 0.25)
#  endif
#else
#  define NSR_DOCUMENT_MAX_HEAP	(0xA000000 * 0.50)
#endif

class NSRAbstractDocument : public QObject
{
	Q_OBJECT
public:
	enum DocumentError {
		NSR_DOCUMENT_ERROR_NO		= 0,
		NSR_DOCUMENT_ERROR_PASSWD	= 1,
		NSR_DOCUMENT_ERROR_TOO_LARGE	= 2,
		NSR_DOCUMENT_ERROR_UNKNOWN	= 3
	};

	explicit NSRAbstractDocument(const QString& file, QObject *parent = 0);
	virtual ~NSRAbstractDocument ();
	inline QString getDocumentPath () const {return _docPath;}
	virtual int getNumberOfPages () const		= 0;
	virtual void renderPage (int page)		= 0;
	virtual QPixmap getCurrentPage ()		= 0;
	virtual bool isValid ()	const			= 0;
	virtual int ZoomIn ();
	virtual int ZoomOut ();
	int getZoom () const {return _zoom;}
	void setZoom (int zoom);
        void setZoomSilent (int zoom) {_zoom = zoom;}
	virtual int getMaxZoom ()			= 0;
	virtual int getMinZoom ()			= 0;
	virtual int getZoomStep () const;
	virtual void zoomToWidth (int screenWidth);
	bool isZoomToWidth () const {return _zoomToWidth;}
	int getScreenWidth () const {return _screenWidth;}
	virtual void rotateLeft ();
	virtual void rotateRight ();
	virtual void setRotation (int angle);
	virtual int getRotation () const {return _rotation;}
	virtual QString getText () {return NSRLanguage::instance()->getLanguage()->mainNoTextData;}
	virtual void setTextOnly (bool textOnly) {_textOnly = textOnly;}
	virtual bool isTextOnly () const {return _textOnly;}
	void setInvertedColors (bool isInverted) {_invertedColors = isInverted;}
	bool isInvertedColors () const {return _invertedColors;}
	DocumentError getLastError () const {return _lastError;}
	virtual void setPassword (const QString& passwd) {Q_UNUSED (passwd);}
	virtual void setEncoding (const QString& enc);
	virtual QString getEncoding () const {return _encoding;}

protected:
	void setZoomToWidth (bool toWidth) {_zoomToWidth = toWidth;}
	void setLastError (DocumentError err) {_lastError = err;}
	QString processText(const QString& text);

private:
	QString		_docPath;
	int		_zoom;
	int		_screenWidth;
	bool		_zoomToWidth;
	bool		_textOnly;
	bool		_invertedColors;
	DocumentError	_lastError;
	QString		_encoding;
	int		_rotation;
};

#endif // NSRABSTRACTDOCUMENT_H
