#include "nsraboutdialog.h"
#include "nsrsettings.h"
#include "nsrlanguage.h"
#include "ui_nsraboutdialog.h"

NSRAboutDialog::NSRAboutDialog (QWidget *parent) :
    NSRTouchDialog (parent),
    ui (new Ui::NSRAboutDialog)
{
	const NSRLanguageData *lang = NSRLanguage::instance()->getLanguage();

	QWidget *widget = new QWidget (this);
	ui->setupUi (widget);

	setTitleIcon (QPixmap(":/icons/icons/about.png"));
	setTitle (NSRLanguage::instance()->getLanguage()->frameAbout);
	setContentsWidget (widget);

	ui->developerLabel->setText (lang->aboutDeveloperName);
	ui->contactsLabel->setText (lang->aboutContacts + " <nsr.reader@gmail.com>");
	ui->iconsCreditLabel->setText (lang->aboutIconsNote + " Stuttgart pack, Axialis Team");
	ui->facebookLabel->setText (lang->aboutFacebookNote);
	ui->versionLabel->setText (QString("NSR Reader %1").arg(NSRSettings::getVersion()));

	QString style = "QLabel {"
			"background-color: white; "
			"border: 1px solid rgba(180, 180, 180, 255); "
			"border-right: none; "
			"border-left: none; "
			"border-top: none; "
			"color: rgb(0, 0, 0); "
#ifdef Q_OS_SYMBIAN
			"font-size: 16px; "
			"font-weight: bold; "
#else
			"font-size: 22px; "
#endif
			"}";

	ui->developerLabel->setStyleSheet (style);
	ui->contactsLabel->setStyleSheet (style);
	ui->iconsCreditLabel->setStyleSheet (style);
	ui->facebookLabel->setStyleSheet (style);
	ui->versionLabel->setStyleSheet (style);
}

NSRAboutDialog::~NSRAboutDialog ()
{
	delete ui;
}
