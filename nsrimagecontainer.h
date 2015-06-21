#ifndef NSRIMAGECONTAINER_H
#define NSRIMAGECONTAINER_H

#include <QWidget>
#include <QPixmap>
#include <QMouseEvent>
#include <QScrollBar>
#include <QGesture>
#include <QPinchGesture>

#include "iscrollable.h"
#include "nsrkineticscroller.h"

class NSRImageContainer : public QWidget, IScrollable
{
	Q_OBJECT
public:
	explicit NSRImageContainer(QWidget *parent = 0);

	void setPixmap (const QPixmap& pixmap);
	void setBetaPixmap (const QPixmap& pixmap);
	void scrollTo (const QPoint& pos);
	void setBetaPixmapEnabled (bool enabled);
	QPoint getContentsPosition () const;
	QPoint getMaximumContentsPosition () const;
	void clearBetaPixmap ();
	void clearPixmap ();
	bool isBetaPixmap () const {return _useBetaPixmap;}
	void setBackgroundColor (const QColor& color);
	void stopKineticScroll() {_scroller->stopMoving();}
	void setMaxZoomFactor (double factor) {_maxZoomFactor = factor;}
	void setStartMode (bool enable);
	bool isStartMode () const {return _isStartMode;}

	/* IScrollable iface */
	QPoint getCurrentPosition ();
	QPoint getMaximumPosition ();
	void setPosition (const QPoint& pos);
	void subscribeForEvents (QObject *obj);
	void unsubscribeOfEvents (QObject *obj);
	QPoint mapFromGlobalCursor (const QPoint& pos);

protected:
	void paintEvent (QPaintEvent *ev);
	void resizeEvent (QResizeEvent *ev);
	bool event (QEvent *ev);
	bool gestureEvent (QGestureEvent *ev);
	void pinchTriggered (QPinchGesture *gesture);

private slots:
	void onScrollStarted ();
	void onScrollChanged ();
	void onScrollStopped ();

signals:
	void pinchZoomRequested (double zoomFactor, QPoint scrollPoint);
	void doubleClicked (const QPoint& pos);
	void prevPageRequest ();
	void nextPageRequest ();

private:
	void updateScrollBars ();
	void pinchStarted ();
	void pinchFinished ();
	void pinchCanceled ();
	void pinchUpdated (double zoomFactor);

	QPixmap			_pxm;
	QPixmap			_betaPxm;
	QPixmap			_cachedPixmap;
	QPixmap			_startPixmap;
	QPoint			_currentPos;
	QScrollBar		*_horizontalBar;
	QScrollBar		*_verticalBar;
	double			_lastPinchFactor;
	QPoint			_lastPinchPoint;
	bool			_useBetaPixmap;
	QColor			_backgroundColor;
	NSRKineticScroller	*_scroller;
	double			_maxZoomFactor;
	bool			_isStartMode;
};

#endif // NSRIMAGECONTAINER_H
