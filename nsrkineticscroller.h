#ifndef NSRKINETICSCROLLER_H
#define NSRKINETICSCROLLER_H

#include <QObject>
#include <QPoint>
#include <QTime>
#include <QList>

#include "iscrollable.h"

struct TrackingPoint {
	TrackingPoint (const QTime& t, const QPoint& p) :
		time (t),
		point (p)
	{}

	QTime	time;
	QPoint	point;
};

class NSRKineticScroller : public QObject
{
	Q_OBJECT

public:
	explicit NSRKineticScroller (QObject *parent = 0);
	virtual ~NSRKineticScroller ();
	void setScrollerObject (IScrollable *obj);
	void setScrollEnabled (bool enabled);
	void stopMoving();
	void stopMovingSilent();

protected:
	bool eventFilter (QObject *obj, QEvent *ev);
	void timerEvent(QTimerEvent *ev);

signals:
	void scrollStarted();
	void scrollChanged();
	void scrollStopped();
	void doubleClicked(const QPoint& pos);
	void prevPageRequest();
	void nextPageRequest();
	void firstClick (const QPoint& pos);

private:
	void startMoving();

	IScrollable		*_scrollObject;
	bool			_isPressed;
	bool			_isTracking;
	int			_actionsToIgnore;
	QPoint			_velocity;
	QPoint			_lastPosition;
	QPoint			_lastVelocity;
	int			_timerId;
	bool			_xScroll;
	bool			_yScroll;
	bool			_canGoPrevPage;
	bool			_canGoNextPage;
	bool			_isScrollEnabled;
	QList<TrackingPoint>	_trackingPoints;
	int			_trackingIndex;
	QTime			_lastClickTime;
	QPoint			_lastClickPos;
};

#endif // NSRKINETICSCROLLER_H
