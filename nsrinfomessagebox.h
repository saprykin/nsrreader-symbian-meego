#ifndef NSRINFOMESSAGEBOX_H
#define NSRINFOMESSAGEBOX_H

#include <QWidget>

namespace Ui {
class NSRInfoMessageBox;
}

class NSRInfoMessageBox : public QWidget
{
	Q_OBJECT
	
public:
	explicit NSRInfoMessageBox (QWidget		*parent,
				    const QString&	title,
				    const QString&	text);
	~NSRInfoMessageBox ();
	void showMessage ();

protected:
	void paintEvent (QPaintEvent *ev);
	
private slots:
	void on_okButton_clicked();
	void on_cancelButton_clicked();

private:
	Ui::NSRInfoMessageBox	*ui;
};

#endif // NSRINFOMESSAGEBOX_H
