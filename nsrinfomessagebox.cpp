#include "nsrinfomessagebox.h"
#include "ui_nsrinfomessagebox.h"
#include "nsrlanguage.h"

#include <QPainter>

NSRInfoMessageBox::NSRInfoMessageBox (QWidget		*parent,
				      const QString&	title,
				      const QString&	text) :
	QWidget	(parent),
	ui (new Ui::NSRInfoMessageBox)
{
	const NSRLanguageData *lang = NSRLanguage::instance()->getLanguage ();

	ui->setupUi(this);
	ui->msgTitle->setText(title);
	ui->msgLabel->setText(text);
	ui->cancelButton->setVisible(false);
	ui->cancelButton->setText (lang->mainCancel);
}

NSRInfoMessageBox::~NSRInfoMessageBox()
{
	delete ui;
}

void NSRInfoMessageBox::showMessage()
{
	QEventLoop loop;

	show ();

	connect (ui->okButton, SIGNAL (clicked ()), &loop, SLOT (quit ()));
	connect (ui->cancelButton, SIGNAL (clicked ()), &loop, SLOT (quit ()));

	loop.exec ();
	close ();
}

void NSRInfoMessageBox::paintEvent(QPaintEvent *ev)
{
	Q_UNUSED (ev);

	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void NSRInfoMessageBox::on_okButton_clicked()
{
}

void NSRInfoMessageBox::on_cancelButton_clicked()
{
}
