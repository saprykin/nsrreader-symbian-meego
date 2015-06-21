#ifndef NSRTOOLFRAME_H
#define NSRTOOLFRAME_H

#include <QFrame>

#ifdef Q_OS_SYMBIAN
#  include "touchfeedback.h"
#endif

class NSRToolFrame : public QFrame
{
    Q_OBJECT
public:
	explicit NSRToolFrame(QWidget *parent = 0);
	~NSRToolFrame();

signals:
	void gotoRequested();
	void aboutRequested();
	void preferencesRequested();
	void zoomToRequested();

public slots:
	void onExitClicked();
	void onPrefsClicked();
	void onGotoClicked();
	void onAboutClicked();

#ifdef Q_OS_SYMBIAN
private:
	MTouchFeedback	*_iTouchFeedback;
#endif
};

#endif // NSRTOOLFRAME_H
