#ifndef NSRNEWSDIALOG_H
#define NSRNEWSDIALOG_H

#include <QWidget>

#include "nsrtouchdialog.h"

namespace Ui {
class NSRNewsDialog;
}

class NSRNewsDialog : public NSRTouchDialog
{
	Q_OBJECT
	
public:
	explicit NSRNewsDialog(QWidget *parent = 0);
	~NSRNewsDialog();
	
private:
	Ui::NSRNewsDialog *ui;
};

#endif // NSRNEWSDIALOG_H
