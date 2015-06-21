#ifndef ISCROLLABLE_H
#define ISCROLLABLE_H

#include <QPoint>
#include <QObject>

class IScrollable
{
public:
	virtual QPoint getCurrentPosition ()			= 0;
	virtual QPoint getMaximumPosition ()			= 0;
	virtual void setPosition (const QPoint& pos)		= 0;
	virtual void subscribeForEvents (QObject *obj)		= 0;
	virtual void unsubscribeOfEvents (QObject *obj)		= 0;
	virtual QPoint mapFromGlobalCursor (const QPoint& pos)	= 0;
};

#endif // ISCROLLABLE_H
