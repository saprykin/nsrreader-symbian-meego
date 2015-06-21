#ifndef NSRSTARTPLABEL_H
#define NSRSTARTLABEL_H

#include <QtCore>
#include <QLabel>
#include <QMouseEvent>
#include <QGesture>

class NSRStartLabel : public QLabel
{
	Q_OBJECT

public:
	NSRStartLabel(QWidget *parent = 0);
};

#endif // NSRSTARTLABEL_H
