#include "nsrtextedit.h"

#include <QTextBrowser>

NSRTextEdit::NSRTextEdit (QWidget *parent) :
	QTextEdit (parent),
#ifdef Q_OS_SYMBIAN
	_defaultFontSize (font().pointSize()),
#else
	_defaultFontSize (16),
#endif
	_startPinchZoom (100)
{
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setReadOnly(true);
	setFocusPolicy(Qt::NoFocus);
	setTextInteractionFlags(Qt::NoTextInteraction);


	_horizontalBar = new QScrollBar (this);
	_horizontalBar->setOrientation(Qt::Horizontal);
	_horizontalBar->hide();
	_horizontalBar->setStyleSheet("QScrollBar:horizontal {border: none; background: transparent; height: 10px;}"
				     "QScrollBar::handle:horizontal {background: rgba(40, 40, 40, 220); min-width: 50px;"
					  "border-width: 1px; border-color: rgba(235, 235, 235, 240);"
					  "border-style: solid; border-radius: 5px;}"
				     "QScrollBar::add-line:horizontal {background: transparent; width: 0px; border: none;}"
				     "QScrollBar::sub-line:horizontal {background: transparent; width: 0px; border: none;}"
				     "QScrollBar:up-arrow:horizontal, QScrollBar::down-arrow:horizontal {"
					  "width: 0px; height: 0px; background: none;}"
				     "QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {background: transparent;}");

	_verticalBar = new QScrollBar (this);
	_verticalBar->setOrientation(Qt::Vertical);
	_verticalBar->hide();
	_verticalBar->setStyleSheet("QScrollBar:vertical {border: none; background: transparent; width: 10px;}"
				     "QScrollBar::handle:vertical {background: rgba(40, 40, 40, 220); min-height: 50px;"
					  "border-width: 1px; border-color: rgba(235, 235, 235, 240);"
					  "border-style: solid; border-radius: 5px;}"
				     "QScrollBar::add-line:vertical {background: transparent; height: 0px; border: none;}"
				     "QScrollBar::sub-line:vertical {background: transparent; height: 0px; border: none;}"
				     "QScrollBar:up-arrow:vertical, QScrollBar::down-arrow:vertical {"
					  "width: 0px; height: 0px; background: none;}"
				     "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background: transparent;}");

	connect (this, SIGNAL (textChanged()), this, SLOT (onTextChanged()));

	QFont fnt = font ();
	fnt.setPointSize (_defaultFontSize);
	setFont (fnt);

	grabGesture(Qt::PinchGesture);

	_scroller = new NSRKineticScroller (this);
	_scroller->setScrollerObject(this);

	connect (_scroller, SIGNAL (scrollStarted()), this, SLOT (onScrollStarted()));
	connect (_scroller, SIGNAL (scrollChanged()), this, SLOT (onScrollChanged()));
	connect (_scroller, SIGNAL (scrollStopped()), this, SLOT (onScrollStopped()));
	connect (_scroller, SIGNAL (prevPageRequest()), this, SIGNAL (prevPageRequest()));
	connect (_scroller, SIGNAL (nextPageRequest()), this, SIGNAL (nextPageRequest()));
	connect (_scroller, SIGNAL (doubleClicked(QPoint)), this, SIGNAL (doubleClicked(QPoint)));
}

NSRTextEdit::~NSRTextEdit()
{
}

void NSRTextEdit::updateScrollBars()
{
	if (document()->size().width() <= width())
		_horizontalBar->hide();

	if (document()->size().height() <= height())
		_verticalBar->hide();

	_horizontalBar->setRange(horizontalScrollBar()->minimum(),
				 horizontalScrollBar()->maximum());
	_horizontalBar->setPageStep(width());
	_verticalBar->setRange(verticalScrollBar()->minimum(),
			       verticalScrollBar()->maximum());
	_verticalBar->setPageStep(height());

	_horizontalBar->resize(width() - _verticalBar->width() - 5, 10);
	_horizontalBar->move(QPoint (5, height() - _horizontalBar->height()));

	_verticalBar->resize(10, height() - _horizontalBar->height() - 5);
	_verticalBar->move(QPoint (width() - _verticalBar->width(), 5));
}

void NSRTextEdit::onScrollStarted()
{
	updateScrollBars();

	if (document()->size().width() > width())
		_horizontalBar->show();

	if (document()->size().height() > height())
		_verticalBar->show();
}

void NSRTextEdit::onScrollStopped()
{
	_horizontalBar->hide();
	_verticalBar->hide();
}

void NSRTextEdit::onScrollChanged()
{
	_horizontalBar->setValue(horizontalScrollBar()->value());
	_verticalBar->setValue(verticalScrollBar()->value());
}

void NSRTextEdit::resizeEvent(QResizeEvent *ev)
{
	double contentPos;

	contentPos = (double) _verticalBar->value() / (double) _verticalBar->maximum();

	QTextEdit::resizeEvent(ev);
	updateScrollBars();

	verticalScrollBar()->setValue((int) (verticalScrollBar()->maximum() * contentPos));
	_verticalBar->setValue(verticalScrollBar()->value());
}

void NSRTextEdit::onTextChanged()
{
	/* Reset scrollbars after changing page data */
	_horizontalBar->setValue(0);
	_verticalBar->setValue(0);
}

void NSRTextEdit::scrollTo(const QPoint &pos)
{
	verticalScrollBar()->setValue(pos.y());
	horizontalScrollBar()->setValue(pos.x());

	onScrollChanged();
}

void NSRTextEdit::resetZoom()
{
	QFont f = font();
	f.setPointSize(_defaultFontSize);
	setFont(f);
}

int NSRTextEdit::getZoom()
{
	int curFontSize = font().pointSize();

	if (_defaultFontSize == curFontSize)
		return 100;
	else if (_defaultFontSize < curFontSize)
		return 100 + (curFontSize - _defaultFontSize) * 25;
	else
		return 100 - (_defaultFontSize - curFontSize) * 25;
}

void NSRTextEdit::zoomOut(int range)
{
	range = qAbs(range);

	if ((getZoom() - range * 25) < 25)
		range = (getZoom() - 25) / 25;

	QTextEdit::zoomOut(range);
}

void NSRTextEdit::zoomIn(int range)
{
	range = qAbs(range);

	if ((getZoom() + range * 25) > 400)
		range = (400 - getZoom()) / 25;

	QTextEdit::zoomIn(range);
}

void NSRTextEdit::setZoom(int zoom)
{
	if (zoom <= 0 || zoom == getZoom())
		return;

	if (qAbs ((zoom - getZoom()) / 25) < 1)
		return;

	if (getZoom() < zoom)
		zoomIn((zoom - getZoom()) / 25);
	else
		zoomOut((getZoom() - zoom) / 25);

}

bool NSRTextEdit::event(QEvent *ev)
{
	if (ev->type() == QEvent::Gesture)
		return gestureEvent(static_cast<QGestureEvent*>(ev));

	return QTextEdit::event(ev);
}

bool NSRTextEdit::gestureEvent(QGestureEvent *ev)
{
	if (QGesture *pinch = ev->gesture(Qt::PinchGesture))
		pinchTriggered(static_cast<QPinchGesture *>(pinch));

	return true;
}

void NSRTextEdit::pinchTriggered(QPinchGesture *gesture)
{
	if (gesture->state() == Qt::GestureStarted) {
		pinchStarted();
	} else if (gesture->state() == Qt::GestureFinished) {
		pinchFinished();
	} else if (gesture->state() == Qt::GestureCanceled) {
		pinchCanceled();
	} else if (gesture->state() == Qt::GestureUpdated) {
		QPinchGesture::ChangeFlags changeTotalFlags = gesture->totalChangeFlags();

		if (changeTotalFlags & QPinchGesture::ScaleFactorChanged)
		    pinchUpdated (gesture->property("totalScaleFactor").toDouble());
	}
}

void NSRTextEdit::pinchStarted()
{
	_scroller->stopMoving();

	_startPinchZoom = getZoom();
	_scroller->setScrollEnabled(false);
}

void NSRTextEdit::pinchFinished()
{
	_scroller->setScrollEnabled(true);
	emit zoomUpdated();
}

void NSRTextEdit::pinchCanceled()
{
	setZoom (_startPinchZoom);
	_scroller->setScrollEnabled(true);
}

void NSRTextEdit::pinchUpdated (double zoomFactor)
{
	setZoom((int)(_startPinchZoom * zoomFactor));
}

/* IScrollable interface */
QPoint NSRTextEdit::getCurrentPosition()
{
	return QPoint (horizontalScrollBar()->value(),
		       verticalScrollBar()->value());
}

QPoint NSRTextEdit::getMaximumPosition()
{
	return QPoint (horizontalScrollBar()->maximum(),
		       verticalScrollBar()->maximum());
}

void NSRTextEdit::setPosition(const QPoint &pos)
{
	scrollTo(pos);
}

void NSRTextEdit::subscribeForEvents(QObject *obj)
{
	installEventFilter(obj);
	viewport()->installEventFilter(obj);
}

void NSRTextEdit::unsubscribeOfEvents(QObject *obj)
{
	viewport()->removeEventFilter(obj);
	removeEventFilter(obj);
}

QPoint NSRTextEdit::mapFromGlobalCursor(const QPoint &pos)
{
	return mapFromGlobal(pos);
}
