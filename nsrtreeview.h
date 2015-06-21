#ifndef NSRTREEVIEW_H
#define NSRTREEVIEW_H

#include "iscrollable.h"
#include "nsrkineticscroller.h"

#include <QTreeView>

class NSRTreeView : public QTreeView, public IScrollable
{
	Q_OBJECT
public:
	explicit NSRTreeView (QWidget *parent = 0);

	/* IScrollable iface */
	QPoint getCurrentPosition ();
	QPoint getMaximumPosition ();
	void setPosition (const QPoint& pos);
	void subscribeForEvents (QObject *obj);
	void unsubscribeOfEvents (QObject *obj);
	QPoint mapFromGlobalCursor (const QPoint& pos);

private slots:
	void onMouseClick (const QPoint& pos);

private:
	NSRKineticScroller	*_scroller;
};

#endif // NSRTREEVIEW_H
