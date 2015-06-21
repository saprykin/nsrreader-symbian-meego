#include "nsrtogglebutton.h"

NSRToggleButton::NSRToggleButton(QWidget *parent) :
	QPushButton(parent)
{
	setCheckable(true);
	setIconSize(QSize (76, 32));

	setTogglePixmaps(QPixmap(":/icons/icons/toggle-on.png"),
			 QPixmap(":/icons/icons/toggle-off.png"));

	connect (this, SIGNAL (clicked(bool)), this, SLOT (onClicked(bool)));

#ifdef Q_OS_SYMBIAN
	_iTouchFeedback = MTouchFeedback::Instance();
#endif
}

void NSRToggleButton::setTogglePixmaps(const QPixmap &on_pxm, const QPixmap &off_pxm)
{
	_icon_on = QIcon (on_pxm);
	_icon_off = QIcon (off_pxm);

	if (isChecked())
		setIcon(_icon_on);
	else
		setIcon(_icon_off);
}

void NSRToggleButton::checkStateChanged(bool checked)
{
	if (checked)
		setIcon (_icon_on);
	else
		setIcon (_icon_off);
}

void NSRToggleButton::onClicked(bool checked)
{
#ifdef Q_OS_SYMBIAN
	_iTouchFeedback->InstantFeedback(ETouchFeedbackBasic);
#endif

	checkStateChanged(checked);
}

void NSRToggleButton::setChecked(bool checked)
{
	QPushButton::setChecked(checked);
	checkStateChanged(checked);
}
