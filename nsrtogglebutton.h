#ifndef NSRTOGGLEBUTTON_H
#define NSRTOGGLEBUTTON_H

#include <QPushButton>

#ifdef Q_OS_SYMBIAN
#  include "touchfeedback.h"
#endif

class NSRToggleButton : public QPushButton
{
	Q_OBJECT
public:
	explicit NSRToggleButton(QWidget *parent = 0);
	void setTogglePixmaps (const QPixmap& on_pxm, const QPixmap& off_pxm);
	void setChecked(bool checked);

private slots:
	void onClicked(bool checked);
	void checkStateChanged(bool checked);

private:
	QIcon		_icon_on;
	QIcon		_icon_off;
#ifdef Q_OS_SYMBIAN
	MTouchFeedback	*_iTouchFeedback;
#endif

};

#endif // NSRTOGGLEBUTTON_H
