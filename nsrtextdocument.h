#ifndef NSRTEXTDOCUMENT_H
#define NSRTEXTDOCUMENT_H

#include "nsrabstractdocument.h"

#include <QMap>

class NSRTextDocument : public NSRAbstractDocument
{
public:
	NSRTextDocument(const QString& file, QObject *parent = 0);
	~NSRTextDocument();

	int getNumberOfPages () const;
	void renderPage (int page);
	QPixmap getCurrentPage ();
	bool isValid ()	const;
	int getMaxZoom ();
	int getMinZoom ();
	void setTextOnly (bool textOnly);
	QString getText ();

private:
	QString			_text;
	int			_pagesCount;
};

#endif // NSRTEXTDOCUMENT_H
