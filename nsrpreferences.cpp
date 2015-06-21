#include "nsrpreferences.h"
#include "nsrlanguage.h"
#include "ui_nsrpreferences.h"

#include <QtGui>
#include <QTextCodec>
#include <QFontDatabase>

#ifdef Q_OS_SYMBIAN
#  ifdef NSR_SYMBIAN_S3
#    include <mw/aknlistquerydialog.h>
#    include <mw/aknPopupHeadingPane.h>
#  else
#    include <aknlistquerydialog.h>
#    include <aknPopupHeadingPane.h>
#  endif

#  include <eiklbm.h>
#  include <badesca.h>

#  ifdef NSR_LITE_VERSION
#    include <dialogs_2005ffa7.rsg>
#  else
#    include <dialogs_20049f7a.rsg>
#  endif
#else
#  include "nsrlistselector.h"
#endif

NSRPreferences::NSRPreferences(QWidget *parent) :
	NSRTouchDialog(parent),
	ui(new Ui::NSRPreferences)
{
	const NSRLanguageData *lang = NSRLanguage::instance()->getLanguage();
	_settings = new NSRSettings (this);

	QWidget *widget = new QWidget (this);
	ui->setupUi (widget);

	setTitleIcon (QPixmap(":/icons/icons/gear.png"));
	setTitle (lang->framePreferences);
	ui->saveLastButton->setChecked (_settings->isLoadLastDoc());
	ui->fullscreenButton->setChecked (_settings->isFullscreenMode());
	ui->textModeButton->setChecked (_settings->isWordWrap());
	ui->invertedColorsButton->setChecked (_settings->isInvertedColors());
	ui->textFont->setText (_settings->getFontFamily());
	ui->textEncoding->setText (_settings->getTextEncoding());

	ui->saveLastLabel->setText (lang->prefsSaveLastDoc);
	ui->fullscreenLabel->setText (lang->prefsFullscreen);
	ui->textModeLabel->setText (lang->prefsTextMode);
	ui->invertedColorsLabel->setText (lang->prefsInvertedMode);
	ui->textFontLabel->setText (lang->prefsFont);
	ui->textEncodingLabel->setText (lang->prefsEncoding);

	QString style = "QLabel { "
			"background-color: white; "
			"border: 1px solid rgba(180, 180, 180, 255); "
			"border-right: none; "
			"border-left: none; "
			"border-top: none; "
#ifdef Q_OS_SYMBIAN
			"font-size: 16px; "
			"font-weight: bold; "
#else
			"font-size: 21px; "
			"padding-left: 10px; "
#endif
			"color: rgb(0, 0, 0); "
			"}";

	QString style2 = "QLabel { "
			 "background-color: white; "
			 "border: none; "
#ifdef Q_OS_SYMBIAN
			 "font-size: 16px; "
			 "font-weight: bold; "
			 "margin-left: 1px; "
#else
			 "font-size: 21px; "
			 "padding-left: 10px; "
#endif
			 "color: rgb(0, 0, 0); "
			 "}";

	QString style3 = "QLabel { "
			 "background-color: white; "
			 "border: 1px solid rgba(180, 180, 180, 255); "
			 "border-right:none; "
			 "border-left:none; "
			 "border-top:none; "
#ifdef Q_OS_SYMBIAN
			 "font-size: 16px; "
			 "font-weight: bold; "
			 "margin-left: 3px; "
#else
			 "font-size: 21px; "
			 "padding-left: 10px; "
#endif
			 "color: rgb(0, 0, 0); "			 
			 "}";

	ui->saveLastLabel->setStyleSheet (style);
	ui->fullscreenLabel->setStyleSheet (style);
	ui->textModeLabel->setStyleSheet (style);
	ui->invertedColorsLabel->setStyleSheet (style);
	ui->textFontLabel->setStyleSheet (style2);
	ui->textEncodingLabel->setStyleSheet (style2);
	ui->textFont->setStyleSheet (style3);
	ui->textEncoding->setStyleSheet (style3);

#ifdef Q_WS_X11
	ui->textFontButton->setIcon (QIcon (":/icons/icons/dropdown-wide.png"));
	ui->textEncodingButton->setIcon (QIcon (":/icons/icons/dropdown-wide.png"));
	ui->textFontButton->setIconSize (QSize (62, 32));
	ui->textEncodingButton->setIconSize (QSize (62, 32));

	ui->saveLastLabel->setMinimumHeight (52);
	ui->fullscreenLabel->setMinimumHeight (52);
	ui->textModeLabel->setMinimumHeight (52);
	ui->invertedColorsLabel->setMinimumHeight (52);
	ui->textFontLabel->setMinimumHeight (52);
	ui->textEncodingLabel->setMinimumHeight (52);
	ui->textFont->setMinimumHeight (52);
	ui->textEncoding->setMinimumHeight (52);

	ui->saveLastButton->setMinimumHeight (52);
	ui->fullscreenButton->setMinimumHeight (52);
	ui->textModeButton->setMinimumHeight (52);
	ui->invertedColorsButton->setMinimumHeight (52);
#endif

	setContentsWidget (widget);

	connect (this, SIGNAL (closeRequested()), this, SLOT (closeButtonClicked()));
	connect (ui->textFontButton, SIGNAL (clicked()), this, SLOT (onTextFontButtonClicked()));
	connect (ui->textEncodingButton, SIGNAL (clicked()), this, SLOT (onTextEncodingButtonClicked()));
}

NSRPreferences::~NSRPreferences()
{
	delete ui;
}

void NSRPreferences::closeButtonClicked()
{
	_settings->saveLoadLastDoc (ui->saveLastButton->isChecked());
	_settings->saveFullscreenMode (ui->fullscreenButton->isChecked());
	_settings->saveWordWrap (ui->textModeButton->isChecked());
	_settings->saveInvertedColors (ui->invertedColorsButton->isChecked());
	_settings->saveFontFamily (ui->textFont->text());
	_settings->saveTextEncoding (ui->textEncoding->text());
	emit preferencesUpdated ();
}

void NSRPreferences::onTextFontButtonClicked()
{
	QStringList		families = QFontDatabase().families();
	const NSRLanguageData	*lang = NSRLanguage::instance()->getLanguage();

	if (families.isEmpty ())
		return;

#ifdef Q_OS_SYMBIAN
	TInt			selectedOption (-1), count (0);
	CDesCArrayFlat		*array = NULL;

	array = new (ELeave) CDesCArrayFlat(families.count());

	CleanupStack::PushL(array);

	count = families.count();
	for (TInt i = 0; i < count; ++i)
	   array->AppendL(TPtrC16 (reinterpret_cast<const TUint16*>(families.at(i).utf16())));

	CAknListQueryDialog* dlg = new(ELeave) CAknListQueryDialog(&selectedOption);
	dlg->PrepareLC(R_DIALOG_LIST_QUERY);
	dlg->QueryHeading()->SetTextL(TPtrC16 (reinterpret_cast<const TUint16*>(lang->prefsFontDialog.utf16())));
	dlg->SetItemTextArray(array);
	dlg->SetOwnershipType(ELbmDoesNotOwnItemArray);
	dlg->RunLD();

	CleanupStack::PopAndDestroy();
#else
	NSRListSelector	*dlg = new NSRListSelector (parentWidget ());
	int		selectedOption = 0;

	dlg->setWindowTitle ("InputDialog");
	dlg->setItems (families);
	dlg->resize (size ());
	dlg->setPromptText (lang->prefsFontDialog);

	hide ();
	dlg->execDialog ();
	show ();

	selectedOption = dlg->getSelectedIndex ();

	delete dlg;
#endif

	if (selectedOption == -1)
		return;

	ui->textFont->setText (families.at (selectedOption));
}

void NSRPreferences::onTextEncodingButtonClicked()
{
	const NSRLanguageData	*lang = NSRLanguage::instance()->getLanguage();
	QStringList		codecs;

	codecs = NSRSettings::getSupportedEncodings();

#ifdef Q_OS_SYMBIAN
	TInt			selectedOption (-1), count;
	CDesCArrayFlat		*array = NULL;

	count = codecs.count();
	array = new (ELeave) CDesCArrayFlat(count);

	CleanupStack::PushL(array);

	for (TInt i = 0; i < count; ++i)
		array->AppendL(TPtrC16 (reinterpret_cast<const TUint16*>(codecs.at(i).utf16())));

	CAknListQueryDialog* dlg = new(ELeave) CAknListQueryDialog(&selectedOption);
	dlg->PrepareLC(R_DIALOG_LIST_QUERY);
	dlg->QueryHeading()->SetTextL(TPtrC16 (reinterpret_cast<const TUint16*>(lang->prefsEncodingDialog.utf16())));
	dlg->SetItemTextArray(array);
	dlg->SetOwnershipType(ELbmDoesNotOwnItemArray);
	dlg->RunLD();

	CleanupStack::PopAndDestroy();
#else
	NSRListSelector	*dlg = new NSRListSelector (parentWidget ());
	int		selectedOption = 0;

	dlg->setWindowTitle ("InputDialog");
	dlg->setItems (codecs);
	dlg->resize (size ());
	dlg->setPromptText (lang->prefsEncodingDialog);

	hide ();
	dlg->execDialog ();
	show ();

	selectedOption = dlg->getSelectedIndex ();

	delete dlg;
#endif

	if (selectedOption == -1)
		return;

	ui->textEncoding->setText (codecs.at (selectedOption));
}
