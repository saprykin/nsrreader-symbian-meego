#ifndef NSRABOUTDIALOG_H
#define NSRABOUTDIALOG_H

#include <QtGui>

#include "nsrtouchdialog.h"

namespace Ui {
    class NSRAboutDialog;
}

class NSRAboutDialog : public NSRTouchDialog
{
	Q_OBJECT

public:
	explicit NSRAboutDialog(QWidget *parent = 0);
	~NSRAboutDialog();

private:
	Ui::NSRAboutDialog	*ui;
};

#endif // NSRABOUTDIALOG_H
