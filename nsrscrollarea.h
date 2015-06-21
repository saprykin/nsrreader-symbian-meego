#ifndef NSRSCROLLAREA_H
#define NSRSCROLLAREA_H

#include <QScrollArea>
#include <QScrollBar>
#include <QMouseEvent>

class NSRScrollArea : public QScrollArea
{
	Q_OBJECT

public:
	NSRScrollArea (QWidget *parent = 0);
	virtual ~NSRScrollArea();

protected:
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void resizeEvent(QResizeEvent *ev);

private:
	void updateScrollBars();
	void onScrollStarted();
	void onScrollStopped();
	void onScrollChanged();

	QScrollBar	*_verticalBar;
	QScrollBar	*_horizontalBar;
	QPoint		_mousePos;
	bool		_xScroll;
	bool		_yScroll;
};

#endif // NSRSCROLLAREA_H
