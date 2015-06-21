#ifndef NSRANIMATIONLABEL_H
#define NSRANIMATIONLABEL_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimerEvent>
#include <QPaintEvent>

class NSRAnimationLabel : public QLabel
{
	Q_OBJECT
public:
	NSRAnimationLabel (QWidget* parent);
	virtual ~NSRAnimationLabel();

public slots:
	void start( );
	void stop ();

protected:
	void timerEvent (QTimerEvent *ev);
	void paintEvent (QPaintEvent *ev);

private:
	void initData();

	QList<QPixmap *>	_pixmaps;
	int			_curIndex;
	bool			_isRunning;
	int			_timeId;
};

#endif // NSRANIMATIONLABEL_H
