#ifndef NSRRENDERTHREAD_H
#define NSRRENDERTHREAD_H

#include <QThread>

#include "nsrabstractdocument.h"

class NSRRenderThread : public QThread
{
	Q_OBJECT
public:
	explicit NSRRenderThread(QObject *parent = 0);
	void setRenderContext(NSRAbstractDocument *doc, int page) {
		_doc = doc;
		_page = page;
	}
	void run();

signals:
	void renderDone ();

public slots:

private:
	NSRAbstractDocument	*_doc;
	int			_page;
};

#endif // NSRRENDERTHREAD_H
