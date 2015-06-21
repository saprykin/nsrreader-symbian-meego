#ifndef NSRTOUCHDIALOG_H
#define NSRTOUCHDIALOG_H

#include <QtGui>

#ifdef Q_OS_SYMBIAN
#  include "touchfeedback.h"
#endif

namespace Ui {
    class NSRTouchDialog;
}

class NSRTouchDialog : public QWidget
{
	Q_OBJECT

public:
	explicit NSRTouchDialog(QWidget *parent = 0);
	~NSRTouchDialog();
	void setTitleIcon (const QPixmap& icon);
	void setTitle (const QString& title);
	void setContentsWidget (QWidget *widget);

protected:
	void resizeEvent(QResizeEvent *ev);

signals:
	void closeRequested ();

private slots:
	void onCloseRequest ();

private:
	Ui::NSRTouchDialog	*ui;
	QWidget			*_widget;
	QPushButton		*_closeBtn;
#ifdef Q_OS_SYMBIAN
	MTouchFeedback		*_iTouchFeedback;
#endif

	void updateButtonPosition();
};

#endif // NSRTOUCHDIALOG_H
