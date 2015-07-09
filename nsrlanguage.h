#ifndef NSRLANGUAGE_H
#define NSRLANGUAGE_H

#include <QString>

#ifdef Q_OS_SYMBIAN
#  include <coemain.h>
#endif

struct NSRLanguageData {
	/* Main window */
	QString mainMessageInfo;
	QString mainNotOpenPassword;
	QString mainNotOpen;
	QString	mainOpenFileTitle;
	QString mainOpenAllFormats;
	QString mainOpenPDFFormat;
	QString mainOpenDjVuFormat;
	QString mainOpenTxtFormat;
	QString mainOpenTIFFFormat;
	QString mainFirstTextMode;
	QString mainZoomTo;
	QString mainPageNumber;
	QString mainAlreadyOpened;
	QString	mainNoTextData;
	QString mainEnterPassword;
#ifdef Q_WS_X11
	QString mainCancel;
#endif
	/* Popup tool frame */
	QString framePreferences;
	QString frameGoToPage;
	QString frameAbout;
	QString frameExit;
	/* Top left context menu */
	QString contextFitToWidth;
	QString contextZoomTo;
	QString contextRotateLeft;
	QString contextRotateRight;
	/* About window */
	QString aboutDeveloperName;
	QString aboutContacts;
	QString aboutIconsNote;
	QString aboutFacebookNote;
	/* Splash screen */
	QString splashLoading;
	/* News window */
	QString newsTitle;
	QString newsWelcome;
	QString newsTitleLabel;
	QString newsFeature1;
	/* Preferences window */
	QString prefsSaveLastDoc;
	QString prefsFullscreen;
	QString prefsTextMode;
	QString prefsInvertedMode;
	QString prefsFont;
	QString prefsEncoding;
	QString prefsFontDialog;
	QString	prefsEncodingDialog;
	/* Touch dialog */
	QString touchClose;
	/* Start label */
	QString startTitle;
	QString startTip1;
	QString startTip2;
	QString startTip3;
	QString startTip4;
};

#ifdef Q_OS_SYMBIAN
class NSRLanguage : public CCoeStatic
#else
class NSRLanguage
#endif
{
public:
	static NSRLanguage * instance ();
	const NSRLanguageData * getLanguage ();

private:
	NSRLanguage ();

	int _langIndex;
};

#endif // NSRLANGUAGE_H
