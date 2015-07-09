#include "nsrnewsdialog.h"
#include "nsrsettings.h"
#include "nsrlanguage.h"
#include "ui_nsrnewsdialog.h"

NSRNewsDialog::NSRNewsDialog(QWidget *parent) :
	NSRTouchDialog(parent),
        ui(new Ui::NSRNewsDialog)
{
	const NSRLanguageData *lang = NSRLanguage::instance()->getLanguage();

	QWidget *widget = new QWidget (this);
	ui->setupUi (widget);

	setTitleIcon (QPixmap(":/icons/icons/nsrlogo-title.png"));
	setTitle (lang->newsTitle);
	setContentsWidget (widget);

	QString style = "QLabel { "
			"background-color: white; "
			"border: 1px solid rgbs(180, 180, 180, 255); "
			"border-right:none; "
			"border-left:none; "
			"border-top:none; "
			"color: rgb(255, 0, 0); "
#ifdef Q_OS_SYMBIAN
			"font: bold 16px; "
#else
			"font-size: 21px; "
#endif
			"}";

	QString style2 = "QLabel { "
			 "background-color: white; "
			 "border: 1px solid rgbs(180, 180, 180, 255); "
			 "border-right:none; "
			 "border-left:none; "
			 "border-top:none; "
			 "color: rgb(0, 0, 0); "
#ifdef Q_OS_SYMBIAN
			 "font: bold 16px; "
#else
			 "font-size: 21px; "
#endif
			 "}";

	ui->welcomeLabel->setStyleSheet (style);
	ui->newsTitleLabel->setStyleSheet (style2);
	ui->newsLabel1->setStyleSheet (style2);

	ui->welcomeLabel->setText ((lang->newsWelcome + " NSR Reader %1!").arg (NSRSettings::getVersion()));
	ui->newsTitleLabel->setText (lang->newsTitleLabel);
	ui->newsLabel1->setText (lang->newsFeature1);
}

NSRNewsDialog::~NSRNewsDialog()
{
	delete ui;
}
