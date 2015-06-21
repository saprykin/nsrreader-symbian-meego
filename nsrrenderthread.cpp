#include "nsrrenderthread.h"

NSRRenderThread::NSRRenderThread(QObject *parent) :
	QThread(parent), _doc (NULL), _page (0)
{
}

void NSRRenderThread::run()
{
	if (_doc == NULL || _page < 1) {
		emit renderDone();
		return;
	}

	_doc->renderPage(_page);
	emit renderDone();
}
