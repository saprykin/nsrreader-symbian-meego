#ifndef NSRTEXTEDIT_H
#define NSRTEXTEDIT_H

#include <QTextEdit>
#include <QScrollBar>
#include <QMouseEvent>
#include <QDateTime>
#include <QGesture>

#include "iscrollable.h"
#include "nsrkineticscroller.h"

class NSRTextEdit : public QTextEdit, IScrollable
{
	Q_OBJECT
public:
	explicit NSRTextEdit (QWidget *parent = 0);
	virtual ~NSRTextEdit ();
	void scrollTo (const QPoint& pos);
	void resetZoom ();
	int getZoom ();
	void setZoom (int zoom);
	void zoomOut (int range = 1);
	void zoomIn (int range = 1);
	int getMinZoom () {return 25;}
	int getMaxZoom () {return 400;}
	void stopKineticScroll() {_scroller->stopMoving();}

	/* IScrollable interface */
	QPoint getCurrentPosition ();
	QPoint getMaximumPosition ();
	void setPosition (const QPoint& pos);
	void subscribeForEvents (QObject *obj);
	void unsubscribeOfEvents (QObject *obj);
	QPoint mapFromGlobalCursor (const QPoint& pos);

protected:
	void resizeEvent(QResizeEvent *ev);
	bool event (QEvent *ev);
	bool gestureEvent (QGestureEvent *ev);
	void pinchTriggered (QPinchGesture *gesture);
	void swipeTriggered (QSwipeGesture *gesture);

signals:
	void nextPageRequest();
	void prevPageRequest();
	void doubleClicked(QPoint pos);
	void zoomUpdated();

private slots:
	void onTextChanged();
	void onScrollStarted();
	void onScrollStopped();
	void onScrollChanged();

private:
	void updateScrollBars();

	void pinchStarted ();
	void pinchFinished ();
	void pinchCanceled ();
	void pinchUpdated (double zoomFactor);

	QScrollBar		*_verticalBar;
	QScrollBar		*_horizontalBar;
	int			_defaultFontSize;
	int			_startPinchZoom;
	NSRKineticScroller	*_scroller;
};

#endif // NSRTEXTEDIT_H
