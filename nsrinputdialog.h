#ifndef NSRINPUTDIALOG_H
#define NSRINPUTDIALOG_H

#include <QWidget>
#include <QEventLoop>

namespace Ui {
class NSRInputDialog;
}

class NSRInputDialog : public QWidget
{
	Q_OBJECT
	
public:
	enum InputType {
		INPUT_TYPE_ALPHABETIC	= 1,
		INPUT_TYPE_NUMERIC	= 2
	};

	enum ExitReason {
		EXIT_REASON_OK		= 1,
		EXIT_REASON_CANCEL	= 2
	};

	explicit NSRInputDialog (QWidget *parent = 0);
	~NSRInputDialog ();

	void paintEvent (QPaintEvent *ev);
	void execDialog ();

	void setPasswordMode (bool isPass);
	void setInputMode (InputType mode);
	void setInputText (const QString& text);
	void setPromptText (const QString& text);
	QString getInputString () const;
	ExitReason getExitReason () const;

protected:
	bool eventFilter (QObject *obj, QEvent *ev);
	void mousePressEvent (QMouseEvent *ev);
	
private slots:
	void on_okButton_clicked ();
	void on_cancelButton_clicked ();

private:
	Ui::NSRInputDialog	*ui;
	ExitReason		_exitReason;
	QEventLoop		_loop;
	bool			_focusSpy;
};

#endif // NSRINPUTDIALOG_H
