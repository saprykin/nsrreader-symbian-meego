#ifndef NSRDOCUMENTMENU_H
#define NSRDOCUMENTMENU_H

#include <QWidget>

#ifdef Q_OS_SYMBIAN
#  include "touchfeedback.h"
#endif

namespace Ui {
class NSRDocumentMenu;
}

class NSRDocumentMenu : public QWidget
{
	Q_OBJECT

public:
	explicit NSRDocumentMenu(QWidget *parent = 0);
	~NSRDocumentMenu();

signals:
	void fitToWidthRequested();
	void rotateLeftRequested();
	void rotateRightRequested();
	void zoomToRequested();

private slots:
	void on_fitToWidthButton_clicked();
	void on_rotateLeftButton_clicked();
	void on_rotateRightButton_clicked();
	void on_zoomToButton_clicked();

private:
	Ui::NSRDocumentMenu	*ui;
#ifdef Q_OS_SYMBIAN
	MTouchFeedback		*_iTouchFeedback;
#endif
};

#endif // NSRDOCUMENTMENU_H
