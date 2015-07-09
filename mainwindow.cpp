/*
 * Copyright (C) 2011-2015 Alexander Saprykin <xelfium@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA.
 */

#include "mainwindow.h"
#include "ui_nsrmainwindow.h"

/* Supported formats */
#include "nsrpopplerdocument.h"
#include "nsrdjvudocument.h"
#include "nsrtextdocument.h"
#include "nsrtiffdocument.h"

#ifdef Q_WS_X11
#  include "nsrinfomessagebox.h"
#  include "nsrinputdialog.h"
#  include "nsropenfiledialog.h"
#endif

#include <QCoreApplication>
#include <QtGui>
#include <QEasingCurve>
#ifdef Q_WS_X11
#include <QX11Info>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include "fixx11h.h"
#endif

#include <float.h>
#include <unistd.h>
#include "poppler/poppler/PDFDoc.h"

#ifdef Q_OS_SYMBIAN
#  include <aknquerydialog.h>
#  include "nativesymbiandlg.hrh"
#  include <dialogs_20049f7a.rsg>
#endif

#define NSR_TOOL_BUTTON_HEIGHT	48

QString fileToOpen;

nsrMainWindow::nsrMainWindow(QWidget *parent)
	: QMainWindow(parent),
	  ui(new Ui::NSRMainWindow),
	  _doc (NULL),
	  _aboutDlg (NULL),
	  _prefsDlg (NULL),
	  _newsDialog (NULL),
	  _lastPinchFactor (1.0),
	  _page (0),
	  _pageCount (0),
	  _lastZoom (100),
	  _timerId (-1),
	  _isFormatSpecificMode (false),
	  _switchModeAfterRender (false),
	  _ignorePressEvent (false),
	  _delayedLoading (false)
{
#ifdef Q_WS_MAEMO_5
	if (!isPortraitMode())
		setAttribute(Qt::WA_Maemo5LandscapeOrientation, true);
	else
		setAttribute(Qt::WA_Maemo5PortraitOrientation, true);
#else
	if (!isPortraitMode())
		setAttribute(Qt::WA_LockLandscapeOrientation, true);
	else
		setAttribute(Qt::WA_LockPortraitOrientation, true);
#endif

	ui->setupUi(this);

	_langData = NSRLanguage::instance()->getLanguage();

	_imgContainer = new NSRImageContainer (this);
	connect (_imgContainer, SIGNAL (pinchZoomRequested(double, QPoint)),
		 this, SLOT (onPinchZoomRequest(double, QPoint)));

	NSRSettings settings;

	_mode = NSRViewModeStart;
	_isInvertedColors = settings.isInvertedColors();

	NSRSession session = settings.getLastSession();
	bool haveSettings = QFile::exists(session.getFile()) && settings.isLoadLastDoc();

	_edit = new NSRTextEdit (this);
#ifdef Q_WS_X11
	_edit->setFrameStyle (QFrame::NoFrame);
#endif
	_edit->hide();
	connect (_edit, SIGNAL (zoomUpdated()), this, SLOT (updateZoomButtons()));

#ifdef Q_OS_SYMBIAN
	_startLabel = new NSRStartLabel (this);
#else
	_imgContainer->setStartMode (true);
#endif

	_frame = new NSRToolFrame (this);
	_frame->hide();

	connect (_frame, SIGNAL (gotoRequested()), this, SLOT (showPageSelectDialog()));
	connect (_frame, SIGNAL (aboutRequested()), this, SLOT (showAboutDialog()));
	connect (_frame, SIGNAL (zoomToRequested()), this, SLOT (showZoomToDialog()));
	connect (_frame, SIGNAL (preferencesRequested()), this, SLOT (showPreferencesDialog()));

	_renderThread = new NSRRenderThread (this);
	connect (_renderThread, SIGNAL (renderDone()), this, SLOT (updatePageAfterRender()));

	_docMenu = new NSRDocumentMenu (this);
	_docMenu->hide();
	_docMenu->move(QPoint (0, NSR_TOOL_BUTTON_HEIGHT - _docMenu->height()));

	connect (_docMenu, SIGNAL (fitToWidthRequested()), this, SLOT (pageToWidth()));
	connect (_docMenu, SIGNAL (rotateLeftRequested()), this, SLOT (rotatePageLeft()));
	connect (_docMenu, SIGNAL (rotateRightRequested()), this, SLOT (rotatePageRight()));
	connect (_docMenu, SIGNAL (zoomToRequested()), this, SLOT (showZoomToDialog()));

	_documentButton = new QPushButton(this);
	_documentButton->setEnabled(false);
	_documentButton->setMinimumSize(QSize(0, NSR_TOOL_BUTTON_HEIGHT));
	QIcon docIcon;
	docIcon.addFile(QString::fromUtf8(":/icons/icons/document.png"), QSize(), QIcon::Normal, QIcon::Off);
	docIcon.addFile(QString::fromUtf8(":/icons/icons/document-disabled.png"), QSize(), QIcon::Disabled, QIcon::Off);
	_documentButton->setIcon(docIcon);
	_documentButton->setIconSize(QSize(32, 32));
	_documentButton->setStyleSheet(QString::fromUtf8("border-bottom: 1px solid rgba(20, 20, 20, 255);"));

	_pagesLabel = new QLabel(this);
	_pagesLabel->setMinimumSize(QSize(0, NSR_TOOL_BUTTON_HEIGHT));
	_pagesLabel->setStyleSheet(QString::fromUtf8("border-bottom: 1px solid rgba(20, 20, 20, 255);"));

	_zoomLabel = new QLabel(this);
	_zoomLabel->setMinimumSize(QSize(0, NSR_TOOL_BUTTON_HEIGHT));
	_zoomLabel->setStyleSheet(QString::fromUtf8("border-bottom: 1px solid rgba(20, 20, 20, 255);"));

	_zoomOutButton = new QPushButton(this);
	_zoomOutButton->setEnabled(false);
	_zoomOutButton->setMinimumSize(QSize(0, NSR_TOOL_BUTTON_HEIGHT));
	_zoomOutButton->setStyleSheet(QString::fromUtf8("border-bottom: 1px solid rgba(20, 20, 20, 255);"));
	QIcon zoomOutIcon;
	zoomOutIcon.addFile(QString::fromUtf8(":/icons/icons/zoom-out.png"), QSize(), QIcon::Normal, QIcon::Off);
	zoomOutIcon.addFile(QString::fromUtf8(":/icons/icons/zoom-out-disabled.png"), QSize(), QIcon::Disabled, QIcon::Off);
	_zoomOutButton->setIcon(zoomOutIcon);
	_zoomOutButton->setIconSize(QSize(24, 24));

	_zoomInButton = new QPushButton(this);
	_zoomInButton->setEnabled(false);
	_zoomInButton->setMinimumSize(QSize(0, NSR_TOOL_BUTTON_HEIGHT));
	_zoomInButton->setStyleSheet(QString::fromUtf8("border-bottom: 1px solid rgba(20, 20, 20, 255);"));
	QIcon zoomInIcon;
	zoomInIcon.addFile(QString::fromUtf8(":/icons/icons/zoom-in.png"), QSize(), QIcon::Normal, QIcon::Off);
	zoomInIcon.addFile(QString::fromUtf8(":/icons/icons/zoom-in-disabled.png"), QSize(), QIcon::Disabled, QIcon::Off);
	_zoomInButton->setIcon(zoomInIcon);
	_zoomInButton->setIconSize(QSize(24, 24));

	_prevButton = new QPushButton(this);
	_prevButton->setEnabled(false);
	_prevButton->setMinimumSize(QSize(0, NSR_TOOL_BUTTON_HEIGHT));
	QIcon prevIcon;
	prevIcon.addFile(QString::fromUtf8(":/icons/icons/back.png"), QSize(), QIcon::Normal, QIcon::Off);
	prevIcon.addFile(QString::fromUtf8(":/icons/icons/back-disabled.png"), QSize(), QIcon::Disabled, QIcon::Off);
	_prevButton->setIcon(prevIcon);
	_prevButton->setIconSize(QSize(32, 32));

	_nextButton = new QPushButton(this);
	_nextButton->setEnabled(false);
	_nextButton->setMinimumSize(QSize(0, NSR_TOOL_BUTTON_HEIGHT));
	QIcon nextIcon;
	nextIcon.addFile(QString::fromUtf8(":/icons/icons/next.png"), QSize(), QIcon::Normal, QIcon::Off);
	nextIcon.addFile(QString::fromUtf8(":/icons/icons/next-disabled.png"), QSize(), QIcon::Disabled, QIcon::Off);
	_nextButton->setIcon(nextIcon);
	_nextButton->setIconSize(QSize(32, 32));

	_openButton = new QPushButton(this);
	_openButton->setMinimumSize(QSize(0, NSR_TOOL_BUTTON_HEIGHT));
	QIcon openIcon;
	openIcon.addFile(QString::fromUtf8(":/icons/icons/open.png"), QSize(), QIcon::Normal, QIcon::Off);
	_openButton->setIcon(openIcon);
	_openButton->setIconSize(QSize(32, 32));

	_advancedButton = new QPushButton(this);
	_advancedButton->setMinimumSize(QSize(0, NSR_TOOL_BUTTON_HEIGHT));
	QIcon advIcon;
	advIcon.addFile(QString::fromUtf8(":/icons/icons/nsrlogo.png"), QSize(), QIcon::Normal, QIcon::Off);
	_advancedButton->setIcon(advIcon);
	_advancedButton->setIconSize(QSize(NSR_TOOL_BUTTON_HEIGHT, NSR_TOOL_BUTTON_HEIGHT));
	_advancedButton->setCheckable(true);

	connect (_prevButton, SIGNAL (clicked()), this, SLOT (onPrevButtonClicked()));
	connect (_nextButton, SIGNAL (clicked()), this, SLOT (onNextButtonClicked()));
	connect (_openButton, SIGNAL (clicked()), this, SLOT (onOpenButtonClicked()));
	connect (_advancedButton, SIGNAL (clicked()), this, SLOT (onAdvancedButtonClicked()));

	connect (_zoomInButton, SIGNAL (clicked()), this, SLOT (onZoomInButtonClicked()));
	connect (_zoomOutButton, SIGNAL (clicked()), this, SLOT (onZoomOutButtonClicked()));
	connect (_documentButton, SIGNAL (clicked()), this, SLOT (onDocumentButtonClicked()));

	_fullscreenToggle = new QToolButton (this);
	_fullscreenToggle->resize(NSR_TOOL_BUTTON_HEIGHT, NSR_TOOL_BUTTON_HEIGHT);
	_fullscreenToggle->hide();
	_fullscreenToggle->setIcon(QIcon (":/icons/icons/uparrow.png"));
	_fullscreenToggle->setIconSize(QSize(NSR_TOOL_BUTTON_HEIGHT, NSR_TOOL_BUTTON_HEIGHT));
	_fullscreenToggle->setStyleSheet("border: none; background: none;");
	connect (_fullscreenToggle, SIGNAL (clicked()), this, SLOT (fullscreenToggleClicked()));

	_animationLabel = new NSRAnimationLabel (this);
	_animationLabel->hide();

	if (!fileToOpen.isEmpty()) {
		NSRSession tmpSession = settings.getSessionForFile(fileToOpen);

		loadDocument(fileToOpen, tmpSession, true);

		if (_doc != NULL && _doc->isValid() && !_delayedLoading) {
			resize(tmpSession.getSize());
			_page = 1;
			_doc->setInvertedColors(_isInvertedColors);
			updatePageWithoutThread();

			/* Update zoom labels for text mode */
			if (_mode == NSRViewModeText)
				updateZoomButtons();
		}
	} else if (haveSettings) {
		resize(session.getSize());
		loadDocument(session.getFile(), session, true);

		if (!_delayedLoading) {
			if (session.isFitToWidth())
				pageToWidthWithoutThread();
			else
				updatePageWithoutThread();
		}
	}

#ifdef Q_WS_X11
	if (_mode == NSRViewModeStart)
		resize (QApplication::desktop()->size ());
#endif

	_isFullscreenMode = settings.isFullscreenMode();
	setToolbarsVisible(!_isFullscreenMode);
	_fullscreenToggle->setVisible(_isFullscreenMode);

	/* We must prepare text mode for inverted colors anyway */
	switchInvertedMode(_isInvertedColors, false);

	connect (_imgContainer, SIGNAL (doubleClicked (QPoint)), this, SLOT (handleDoubleClick (QPoint)));
	connect (_imgContainer, SIGNAL (prevPageRequest()), this, SLOT (onPrevButtonClicked()));
	connect (_imgContainer, SIGNAL (nextPageRequest()), this, SLOT (onNextButtonClicked()));

	connect (_edit, SIGNAL (prevPageRequest()), this, SLOT (onPrevButtonClicked()));
	connect (_edit, SIGNAL (nextPageRequest()), this, SLOT (onNextButtonClicked()));
	connect (_edit, SIGNAL (doubleClicked(QPoint)), this, SLOT (handleDoubleClick(QPoint)));

	_imgContainer->installEventFilter(this);
	_edit->installEventFilter(this);
#ifdef Q_OS_SYMBIAN
	_startLabel->installEventFilter(this);
#endif

	QWidgetList widgets = QApplication::allWidgets();
	QWidget* w = 0;
	foreach (w, widgets) {
	    w->setContextMenuPolicy(Qt::NoContextMenu);
	}

#ifdef Q_OS_SYMBIAN
	/* Initialize MRemConCoreApiTargetObserver interface */
	_iInterfaceSelector = CRemConInterfaceSelector::NewL();
	_iCoreTarget = CRemConCoreApiTarget::NewL (*_iInterfaceSelector, *this);
	_iInterfaceSelector->OpenTargetL();

	_iTouchFeedback = MTouchFeedback::Instance();
	_iTouchFeedback->SetFeedbackEnabledForThisApp(ETrue);
#elif !defined (Q_WS_MAEMO_5)
	_resourceSet = new ResourcePolicy::ResourceSet ("player");
	_resourceSet->addResourceObject (new ResourcePolicy::ScaleButtonResource);
	_resourceSet->acquire ();
#endif

	qApp->installEventFilter(this);
	setOrientation(ScreenOrientationAuto);

	_frameAnimation = new QPropertyAnimation(this);
	_frameAnimation->setEasingCurve(QEasingCurve(QEasingCurve::OutCubic));
	_frameAnimation->setTargetObject(_frame);
	_frameAnimation->setPropertyName("pos");
#ifdef Q_OS_SYMBIAN
	_frameAnimation->setDuration(150);
#else
	_frameAnimation->setDuration(200);
#endif

	_docMenuAnimation = new QPropertyAnimation(this);
	_docMenuAnimation->setEasingCurve(QEasingCurve(QEasingCurve::OutCubic));
	_docMenuAnimation->setTargetObject(_docMenu);
	_docMenuAnimation->setPropertyName("pos");
	_docMenuAnimation->setDuration(150);

	QFont font = _edit->font();
	font.setFamily(settings.getFontFamily());
	_edit->setFont(font);

	QString addStyle = "QLabel { "
			   "background-color: "
				"qlineargradient(spread:pad, "
						"x1:0.5, y1:0,  x2:0.5, y2:1, "
						"stop:0 rgba(80, 80, 80), "
						"stop:0.25 rgba(65, 65, 65), "
						"stop:0.5 rgba(45, 45, 45), "
						"stop:0.75 rgba(20, 20, 20), "
						"stop:1 rgba(0, 0, 0, 255)); "
			   "color: rgb(255, 255, 255); "
#ifdef Q_OS_SYMBIAN
			   "font-size: 18px; "
#else
			   "font-size: 22px; "
#endif
			   "}";

	addStyle = styleSheet () + addStyle;
	setStyleSheet (addStyle);

	updateCentralWidget();

	if (!settings.isNewsShown()) {
		_newsDialog = new NSRNewsDialog (this);

		connect (_newsDialog, SIGNAL (closeRequested()), this, SLOT (closeNewsDialog()));

		_newsDialog->resize(QApplication::desktop()->screen()->size());
		_newsDialog->showFullScreen();
		settings.saveNewsShown();
	}

#ifdef Q_WS_MAEMO_5
	grabZoomKeys (true);
#endif
}

nsrMainWindow::~nsrMainWindow()
{
#ifdef Q_OS_SYMBIAN
	delete _iInterfaceSelector;
#endif
	delete ui;
}

void nsrMainWindow::setOrientation(ScreenOrientation orientation)
{
	Qt::WidgetAttribute attribute;

#ifdef Q_WS_MAEMO_5
	switch (orientation) {
	case ScreenOrientationLockPortrait:
		attribute = Qt::WA_Maemo5PortraitOrientation;
		break;
	case ScreenOrientationLockLandscape:
		attribute = Qt::WA_Maemo5LandscapeOrientation;
		break;
	case ScreenOrientationAuto:
	default:
		attribute = Qt::WA_Maemo5AutoOrientation;
		break;
	}
#else

	// If the version of Qt on the device is < 4.7.2, that attribute won't work
	if (orientation != ScreenOrientationAuto) {
		const QStringList v = QString::fromAscii(qVersion()).split(QLatin1Char('.'));
		if (v.count() == 3 && (v.at(0).toInt() << 16 | v.at(1).toInt() << 8 | v.at(2).toInt()) < 0x040702) {
			qWarning("Screen orientation locking only supported with Qt 4.7.2 and above");
			return;
		}
	}

	switch (orientation) {
#  if QT_VERSION < 0x040702
	// Qt < 4.7.2 does not yet have the Qt::WA_*Orientation attributes
	case ScreenOrientationLockPortrait:
		attribute = static_cast<Qt::WidgetAttribute>(128);
		break;
	case ScreenOrientationLockLandscape:
		attribute = static_cast<Qt::WidgetAttribute>(129);
		break;
	default:
	case ScreenOrientationAuto:
		attribute = static_cast<Qt::WidgetAttribute>(130);
		break;
#  else // QT_VERSION < 0x040702
	case ScreenOrientationLockPortrait:
		attribute = Qt::WA_LockPortraitOrientation;
		break;
	case ScreenOrientationLockLandscape:
		attribute = Qt::WA_LockLandscapeOrientation;
		break;
	default:
	case ScreenOrientationAuto:
		attribute = Qt::WA_AutoOrientation;
		break;
#  endif // QT_VERSION < 0x040702
	};
#endif
	setAttribute(attribute, true);
}

void nsrMainWindow::showExpanded()
{
	showFullScreen();
}

void nsrMainWindow::loadDocument(const QString		&file,
				 const NSRSession	&session,
				 bool			fromConstructor /* false */)
{
	NSRSettings settings;
	const NSRLanguageData *lang = NSRLanguage::instance()->getLanguage ();

	saveSession();

	if (_doc != NULL)
		delete _doc;

	/* If first document loading */
	if (_mode == NSRViewModeStart) {
		_mode = settings.isWordWrap() ? NSRViewModeText : NSRViewModeGraphic;
		switchViewMode(_mode);
	}

	QString fileExt = QFileInfo (file).suffix();

	if (fileExt.compare (QString ("pdf"), Qt::CaseInsensitive) == 0) {
		_doc = new NSRPopplerDocument (file, this);		
		checkViewModeOnLoad(settings);
	} else if (fileExt.compare (QString ("djvu"), Qt::CaseInsensitive) == 0) {
		_doc = new NSRDjVuDocument (file, this);
		checkViewModeOnLoad(settings);
	} else if (fileExt.compare (QString ("txt"), Qt::CaseInsensitive) == 0) {
		_doc = new NSRTextDocument (file, this);
		_isFormatSpecificMode = true;

		if (_mode != NSRViewModeText)
			switchViewMode(NSRViewModeText);
#if defined (NSR_SYMBIAN_S3) || defined (Q_WS_X11)
	} else if (fileExt.compare (QString ("tif"), Qt::CaseInsensitive) == 0 ||
		   fileExt.compare (QString ("tiff"), Qt::CaseInsensitive) == 0) {
		_doc = new NSRTIFFDocument (file, this);
		checkViewModeOnLoad(settings);
#endif
	} else {
		if (_mode != NSRViewModeStart)
			switchViewMode(NSRViewModeStart);

		return;
	}

	_edit->resetZoom();
	_edit->clear();

	_doc->setTextOnly(_mode == NSRViewModeText);
	_doc->setEncoding(settings.getTextEncoding());
	_doc->setInvertedColors(_isInvertedColors);

	if (!_doc->isValid()) {
		if (_doc->getLastError() == NSRAbstractDocument::NSR_DOCUMENT_ERROR_PASSWD) {
#ifdef Q_OS_SYMBIAN
			TBuf<KMaxPasswordLength> password;
			HBufC* tmpPassword = HBufC::NewLC (KMaxPasswordLength);
			TPtr tmpPtr = tmpPassword->Des();
			TPtrC16 textPtr (reinterpret_cast<const TUint16*>(lang->mainEnterPassword.utf16()));

			CAknTextQueryDialog* dlg =  new CAknTextQueryDialog (password, CAknQueryDialog::ENoTone);
			dlg->SetPromptL (textPtr);

			if (dlg->ExecuteLD (R_DIALOG_PASSWORD_QUERY)) {
			    tmpPtr.Copy(password);
			    _doc->setPassword (QString ((QChar *)tmpPtr.Ptr(), tmpPassword->Length()));
			}

			CleanupStack::PopAndDestroy();
#else
			if (fromConstructor) {
				_delayedLoading = true;
				_timerId = startTimer (100);
				return;
			}
			NSRInputDialog	*dlg = new NSRInputDialog (this);
			dlg->setWindowTitle ("InputDialog");
			dlg->setInputMode (NSRInputDialog::INPUT_TYPE_ALPHABETIC);
			dlg->setPasswordMode (true);
			dlg->setInputText ("");
			dlg->setPromptText (lang->mainEnterPassword);
			dlg->resize (size ());
			dlg->execDialog ();

			QString passwd = dlg->getInputString ();
			delete dlg;

			_doc->setPassword (passwd);
#endif
		}
	}

	if (!_doc->isValid()) {
#ifdef Q_OS_SYMBIAN
		if (_doc->getLastError() == NSRAbstractDocument::NSR_DOCUMENT_ERROR_PASSWD)
			QMessageBox::information (NULL,
						  _langData->mainMessageInfo,
						  _langData->mainNotOpenPassword);
		else
			QMessageBox::information (NULL, _langData->mainMessageInfo, _langData->mainNotOpen);
#else
		NSRInfoMessageBox *msgBox;

		if (_doc->getLastError() == NSRAbstractDocument::NSR_DOCUMENT_ERROR_PASSWD)
			msgBox = new NSRInfoMessageBox (this,
							_langData->mainMessageInfo,
							_langData->mainNotOpenPassword);
		else
			msgBox = new NSRInfoMessageBox (this,
							_langData->mainMessageInfo,
							_langData->mainNotOpen);

		msgBox->resize (size ());
		msgBox->showMessage ();
		delete msgBox;
#endif
		delete _doc;
		_doc = NULL;

		if (_mode != NSRViewModeStart)
			switchViewMode(NSRViewModeStart);

		updateNavigationButtons();
		updateZoomButtons();

		return;
	}

	_pageCount = _doc->getNumberOfPages();

	if (session.getFile() == file) {
		_page = session.getPage();
		_doc->setRotation(session.getRotation());

		if (session.isFitToWidth())
			_doc->zoomToWidth(width());
		else
			_doc->setZoom(session.getZoomGraphic());

		/* We need to keep in sync zoom for text */
		int textZoom = session.getZoomText();

		if (textZoom > 100) {
			if ((textZoom % _doc->getZoomStep()) != 0)
				textZoom = (textZoom / _doc->getZoomStep() + 1) * _doc->getZoomStep();

			_edit->zoomIn((textZoom - 100) / _doc->getZoomStep());
		} else if (textZoom < 100) {
			if ((textZoom % _doc->getZoomStep()) != 0)
				textZoom = (textZoom / _doc->getZoomStep()) * _doc->getZoomStep();

			_edit->zoomOut((100 - textZoom) / _doc->getZoomStep());
		}

		/* Update zoom labels for text mode */
		if (_mode == NSRViewModeText)
			updateZoomButtons();

		_lastPos = session.getPosition();
	} else
		_page = 1;
}


void nsrMainWindow::onOpenButtonClicked()
{
#ifdef Q_OS_SYMBIAN
	_iTouchFeedback->InstantFeedback(ETouchFeedbackBasic);
#endif

	stopKineticScroll();

	NSRSettings	settings;
	QString		file;

#ifdef Q_OS_SYMBIAN
	file = QFileDialog::getOpenFileName(NULL,
					    _langData->mainOpenFileTitle,
					    settings.getLastOpenDir(),
#  ifdef NSR_SYMBIAN_S3
					    _langData->mainOpenAllFormats + " (*.pdf *.djvu *.txt *.tif *.tiff);;" +
#  else
					    _langData->mainOpenAllFormats + " (*.pdf *.djvu *.txt);;" +
#  endif
					    _langData->mainOpenPDFFormat + " (*.pdf);;" +
					    _langData->mainOpenDjVuFormat + " (*.djvu);;" +
					    _langData->mainOpenTxtFormat + " (*.txt)"
#  ifdef NSR_SYMBIAN_S3
					    + ";;" + _langData->mainOpenTIFFFormat + " (*.tif *.tiff)");
#  else
					    );
#  endif
#else
	NSROpenFileDialog *openDial = new NSROpenFileDialog (this);
	openDial->setWindowTitle (_langData->mainOpenFileTitle);
	openDial->setRootPath (settings.getLastOpenDir());
	openDial->setNameFilters (QStringList ("*.pdf") << "*.djvu"
							<< "*.tif"
							<< "*.tiff"
							<< "*.txt");
	openDial->resize (size());
	openDial->execDialog ();

	file = openDial->getSelectedFile ();

	delete openDial;
#endif

	if (file.isNull())
		return;

	if (_doc != NULL && _doc->getDocumentPath() == file) {
#ifdef Q_OS_SYMBIAN
		QMessageBox::information (NULL,
					  _langData->mainMessageInfo,
					  _langData->mainAlreadyOpened);
#else
		NSRInfoMessageBox *msgBox = new NSRInfoMessageBox (this,
								   _langData->mainMessageInfo,
								   _langData->mainAlreadyOpened);
		msgBox->resize (size ());
		msgBox->showMessage ();

		delete msgBox;
#endif
		return;
	}

	settings.saveLastOpenDir(QFileInfo(file).absolutePath());

	loadDocument(file, settings.getSessionForFile (file));
	goToPage(_page, _lastPos);
}

void nsrMainWindow::onNextButtonClicked()
{
#ifdef Q_OS_SYMBIAN
	_iTouchFeedback->InstantFeedback(ETouchFeedbackBasic);
#endif

	stopKineticScroll();

	if (_renderThread->isRunning())
		return;

	if ((_page + 1) > _pageCount)
		return;

	goToPage(_page + 1, autodetectPagePosition());
}

void nsrMainWindow::onPrevButtonClicked()
{
#ifdef Q_OS_SYMBIAN
	_iTouchFeedback->InstantFeedback(ETouchFeedbackBasic);
#endif

	stopKineticScroll();

	if (_renderThread->isRunning())
		return;

	if ((_page - 1) == 0)
		return;

	goToPage(_page - 1, autodetectPagePosition());
}

void nsrMainWindow::onAdvancedButtonClicked()
{
#ifdef Q_OS_SYMBIAN
	_iTouchFeedback->InstantFeedback(ETouchFeedbackBasic);
#endif

	stopKineticScroll();

	if (_frameAnimation->state() == QPropertyAnimation::Running)
		_frameAnimation->stop();

	if (!_frame->isVisible()) {
		disconnect (_frameAnimation, SIGNAL (finished()), _frame, SLOT (hide()));
		disconnect (_frameAnimation, SIGNAL (finished()), this, SLOT (toggleFullscreenMode()));

		_frameAnimation->setStartValue(_frame->pos());
		_frameAnimation->setEndValue(QPoint (0, height() - _prevButton->height() - _frame->height()));
		_frame->show();
		_frameAnimation->start();

	} else {
		_frameAnimation->setStartValue(_frame->pos());
		_frameAnimation->setEndValue(QPoint (0, _prevButton->pos().y()));

		connect (_frameAnimation, SIGNAL (finished()), _frame, SLOT (hide()));

		if (_isFullscreenMode && !_fullscreenToggle->isVisible())
			connect (_frameAnimation, SIGNAL (finished()), this, SLOT (toggleFullscreenMode()));

		_frameAnimation->start();
	}
}

void nsrMainWindow::onZoomOutButtonClicked()
{
#ifdef Q_OS_SYMBIAN
	_iTouchFeedback->InstantFeedback(ETouchFeedbackBasic);
#endif

	stopKineticScroll();

	if (_doc == NULL)
		return;

	if (_mode == NSRViewModeGraphic) {
		_doc->ZoomOut();
		goToPage(_page, _imgContainer->getContentsPosition());
	} else if (_mode == NSRViewModeText) {
		_edit->zoomOut();
		updateZoomButtons();
	}
}

void nsrMainWindow::onZoomInButtonClicked()
{
#ifdef Q_OS_SYMBIAN
	_iTouchFeedback->InstantFeedback(ETouchFeedbackBasic);
#endif

	stopKineticScroll();

	if (_doc == NULL)
                return;

	if (_mode == NSRViewModeGraphic) {
		_doc->ZoomIn();
		goToPage(_page, _imgContainer->getContentsPosition());
	} else if (_mode == NSRViewModeText) {
		_edit->zoomIn();
		updateZoomButtons();
	}
}

void nsrMainWindow::updatePage()
{
	if (_page < 1 || _doc == NULL)
		return;

	_animationLabel->setFixedSize(size());
	_animationLabel->show();
	_animationLabel->start();

	/* We do not need beta pixmap if start label is visible */
#ifdef Q_OS_SYMBIAN
	if (!_imgContainer->isBetaPixmap() && !_startLabel->isVisible()) {
#else
	if (!_imgContainer->isBetaPixmap() && !_imgContainer->isStartMode()) {
#endif
		QPixmap pxm (_imgContainer->size());
		QPainter painter (&pxm);
		_imgContainer->render(&painter);
		_imgContainer->setBetaPixmap(pxm);
		_imgContainer->setBetaPixmapEnabled(true);
	}

	_imgContainer->clearPixmap();

	/* Lock screen orientation because we are using fake view */
#ifdef Q_WS_MAEMO_5
	if (!isPortraitMode())
		setAttribute(Qt::WA_Maemo5LandscapeOrientation, true);
	else
		setAttribute(Qt::WA_Maemo5PortraitOrientation, true);
#else
	if (!isPortraitMode())
		setAttribute(Qt::WA_LockLandscapeOrientation, true);
	else
		setAttribute(Qt::WA_LockPortraitOrientation, true);
#endif

	_renderThread->setRenderContext(_doc, _page);
	_renderThread->start();
}

void nsrMainWindow::updatePageAfterRender()
{
	bool realGraphicMode = (_mode == NSRViewModeGraphic &&
				(!_switchModeAfterRender || _lateMode == NSRViewModeGraphic));

	if (realGraphicMode)  {
		QPixmap	img = _doc->getCurrentPage();
		_imgContainer->setPixmap(img);
		_imgContainer->scrollTo(_lastPos);
		_imgContainer->setBetaPixmapEnabled(false);
		_imgContainer->clearBetaPixmap();
	} else
		_edit->setText(_doc->getText());

	_animationLabel->stop();
	_animationLabel->hide();

	/* Hide start label if we've launched from the start mode */
#ifdef Q_OS_SYMBIAN
	_startLabel->hide();
#else
	_imgContainer->setStartMode (false);
#endif
	updateCentralWidget();

	_pagesLabel->setText(QString ("%1/%2").arg(QString::number(_page), QString::number(_pageCount)));
	_documentButton->setEnabled(realGraphicMode);

	/* Unlock screen orientation */
#ifdef Q_WS_MAEMO_5
	setAttribute(Qt::WA_Maemo5LandscapeOrientation, false);
	setAttribute(Qt::WA_Maemo5PortraitOrientation, false);
	setAttribute(Qt::WA_Maemo5AutoOrientation, true);
#else
	setAttribute(Qt::WA_LockLandscapeOrientation, false);
	setAttribute(Qt::WA_LockPortraitOrientation, false);
	setAttribute(Qt::WA_AutoOrientation, true);
#endif

	if (_switchModeAfterRender) {
		switchViewMode(_lateMode);
		_switchModeAfterRender = false;
	}

	updateZoomButtons();
	updateNavigationButtons();

	_imgContainer->setMaxZoomFactor ((double) _doc->getMaxZoom() / _doc->getZoom());
}

void nsrMainWindow::updateZoomButtons()
{
	int curZoom, maxZoom, minZoom;

	if (_doc == NULL) {
		_zoomOutButton->setEnabled(false);
		_zoomInButton->setEnabled(false);
		return;
	}

	if (_mode == NSRViewModeGraphic) {
		curZoom = _doc->getZoom();
		minZoom = _doc->getMinZoom();
		maxZoom = _doc->getMaxZoom();
	} else if (_mode == NSRViewModeText) {
		curZoom = _edit->getZoom();
		minZoom = _edit->getMinZoom();
		maxZoom = _edit->getMaxZoom();
	} else {
		curZoom = 100;
		minZoom = 100;
		maxZoom = 100;
	}

	if (curZoom == minZoom)
		_zoomOutButton->setEnabled(false);
	else
		_zoomOutButton->setEnabled(true);

	if (curZoom == maxZoom)
		_zoomInButton->setEnabled(false);
	else
		_zoomInButton->setEnabled(true);

	_zoomLabel->setText(QString::number(curZoom) + QString ("%"));
}

void nsrMainWindow::pageToWidth()
{
	if (_doc == NULL)
		return;

	_doc->zoomToWidth(width());
	updatePage();
}

void nsrMainWindow::resizeEvent(QResizeEvent *ev)
{
	if (_aboutDlg != NULL)
		_aboutDlg->resize(size());

	if (_prefsDlg != NULL)
		_prefsDlg->resize(size());

	if (_newsDialog != NULL)
		_newsDialog->resize(size());

	updateCentralWidget();
	updateTopToolbar();
	updateBottomToolbar();
	updateStartLabel();
	updateToolFrame();
	updateFullscreenToggle();

	if (_docMenu->isVisible())
		_docMenu->move(QPoint (0, _documentButton->height()));
}

void nsrMainWindow::updateToolFrame()
{
	_frame->setFixedWidth(width());
	_frame->setMaximumHeight(height() - _zoomLabel->height() - _prevButton->height());

	_frame->adjustSize();

	if (_frame->isVisible())
		_frame->move(QPoint (0, mapToGlobal(_prevButton->pos()).y() - _frame->height()));
	else
		_frame->move(QPoint (0, height() - _prevButton->height()));
}

void nsrMainWindow::goToPage(int page, const QPoint& pos)
{
	if (_doc == NULL || page < 1 || page > _pageCount)
		return;

	_page = page;
	_lastPos = pos;
	updatePage();
}

void nsrMainWindow::closeEvent(QCloseEvent *ev)
{
	saveSession();

	ev->accept();
}

void nsrMainWindow::updateNavigationButtons()
{
	if (_doc == NULL) {
		_prevButton->setEnabled(false);
		_nextButton->setEnabled(false);
		return;
	}

	if (_page == _pageCount)
		_nextButton->setEnabled(false);
	else
		_nextButton->setEnabled(true);

	if (_page > 1)
		_prevButton->setEnabled(true);
	else if (_page == 1)
		_prevButton->setEnabled(false);
}

void nsrMainWindow::updatePageWithoutThread()
{
	if (_page < 1 || _doc == NULL)
		return;

	_doc->renderPage(_page);
	updatePageAfterRender();
}

void nsrMainWindow::pageToWidthWithoutThread()
{
	if (_doc == NULL)
		return;

	_doc->zoomToWidth(width());
	updatePageWithoutThread();
}

void nsrMainWindow::setToolbarsVisible(bool visible)
{
	_pagesLabel->setVisible(visible);
	_zoomLabel->setVisible(visible);
	_zoomInButton->setVisible(visible);
	_zoomOutButton->setVisible(visible);
	_documentButton->setVisible(visible);

	_prevButton->setVisible(visible);
	_nextButton->setVisible(visible);
	_openButton->setVisible(visible);
	_advancedButton->setVisible(visible);
}

void nsrMainWindow::handleDoubleClick(const QPoint &pos)
{
	if (_doc == NULL)
		return;

	if (pos.x() > width() * 2 / 3)
		onNextButtonClicked();
	else if (pos.x() < width() / 3)
		onPrevButtonClicked();
	else if (_mode == NSRViewModeGraphic) {
		if (_doc->isZoomToWidth()) {
			_doc->setZoom(_lastZoom);
			goToPage(_page, _imgContainer->getContentsPosition());
		} else {
			_lastZoom = _doc->getZoom();
			pageToWidth();
		}
	}
}

void nsrMainWindow::showPageSelectDialog()
{
	if (_doc == NULL)
		return;

	QString prompt = _langData->mainPageNumber.arg ("1-" + QString::number (_doc->getNumberOfPages ()));

#ifdef Q_OS_SYMBIAN
	TInt number = _page > 0 ? _page : 1;
	TPtrC16 textPtr (reinterpret_cast<const TUint16*>(prompt.utf16()));

	CAknNumberQueryDialog* dlg =  new CAknNumberQueryDialog (number, CAknQueryDialog::ENoTone);
	dlg->PrepareLC (R_DIALOG_NUMBER_QUERY);
	dlg->SetPromptL (textPtr);

	if (dlg->RunLD ())
		goToPage (qBound (1, number, _pageCount), autodetectPagePosition ());
#else
	int		number;
	NSRInputDialog	*dlg = new NSRInputDialog (this);
	dlg->setWindowTitle ("InputDialog");
	dlg->setInputMode (NSRInputDialog::INPUT_TYPE_NUMERIC);
	dlg->setPasswordMode (false);
	dlg->setInputText (QString::number (_page > 0 ? _page : 1));
	dlg->setPromptText (prompt);
	dlg->resize (size ());
	dlg->execDialog ();

	QString nm = dlg->getInputString ();
	bool	ok;

	if (dlg->getExitReason () == NSRInputDialog::EXIT_REASON_OK && !nm.isEmpty ())
		number = dlg->getInputString().toInt (&ok);
	else
		number = -1;

	delete dlg;

	if (number > 0 && ok)
		goToPage (qBound (1, number, _pageCount), autodetectPagePosition ());
#endif	
}

void nsrMainWindow::showAboutDialog()
{
	if (_aboutDlg != NULL)
		return;

#ifdef NSR_SYMBIAN_S3
	_aboutDlg = new NSRAboutDialog (this);
	_aboutDlg->move(0, height());
	_aboutDlg->resize(size());

	connect (_aboutDlg, SIGNAL (closeRequested()), this, SLOT(closeAboutDialog()));

	QPropertyAnimation *dialogAnimation = new QPropertyAnimation(this);
	dialogAnimation->setEasingCurve(QEasingCurve(QEasingCurve::OutCubic));
	dialogAnimation->setDuration(300);
	dialogAnimation->setTargetObject(_aboutDlg);
	dialogAnimation->setPropertyName("pos");
	dialogAnimation->setStartValue(_aboutDlg->pos());
	dialogAnimation->setEndValue(QPoint (0, 0));

	_aboutDlg->show();
	dialogAnimation->start(QAbstractAnimation::DeleteWhenStopped);
#else
	_aboutDlg = new NSRAboutDialog (this);
	_aboutDlg->resize(size());

	connect (_aboutDlg, SIGNAL (closeRequested()), this, SLOT(closeAboutDialog()));

	_aboutDlg->show();
#endif
}

#ifdef Q_OS_SYMBIAN
void nsrMainWindow::MrccatoCommand(TRemConCoreApiOperationId aOperationId,
				   TRemConCoreApiButtonAction aButtonAct)
{
	TRequestStatus status;

	switch (aOperationId) {
	case ERemConCoreApiVolumeUp:
		if (aButtonAct == ERemConCoreApiButtonClick)
			onPrevButtonClicked();

		_iCoreTarget->VolumeUpResponse(status, KErrNone);
		User::WaitForRequest(status);
		break;

	case ERemConCoreApiVolumeDown:
		if (aButtonAct == ERemConCoreApiButtonClick)
			onNextButtonClicked();

		_iCoreTarget->VolumeDownResponse(status, KErrNone);
		User::WaitForRequest(status);
		break;

	default:
		break;
	}
}
#endif

void nsrMainWindow::rotatePageLeft()
{
	if (_doc == NULL)
		return;

	_doc->rotateLeft();
	updatePage();
}

void nsrMainWindow::rotatePageRight()
{
	if (_doc == NULL)
		return;

	_doc->rotateRight();
	updatePage();
}

void nsrMainWindow::updateFullscreenToggle()
{
	_fullscreenToggle->move(width() - _fullscreenToggle->width(), height() - _fullscreenToggle->height());
}

void nsrMainWindow::toggleFullscreenMode()
{
	if (!_isFullscreenMode)
		return;

	_fullscreenToggle->setVisible(!_fullscreenToggle->isVisible());
	setToolbarsVisible(!_fullscreenToggle->isVisible());
	updateStartLabel();
}

bool nsrMainWindow::eventFilter(QObject *obj, QEvent *ev)
{
#if defined (Q_WS_X11) && !defined (Q_WS_MAEMO_5)
	if (ev->type() == QEvent::ApplicationActivate)
		_resourceSet->acquire ();
	else if (ev->type() == QEvent::ApplicationDeactivate)
		_resourceSet->release ();
#endif

	if (ev->type() == QEvent::MouseButtonPress) {
		if (_ignorePressEvent) {
			_ignorePressEvent = false;
			return false;
		}

		if (_docMenu->isVisible()) {
			QPoint pos = ((QMouseEvent *)ev)->pos();
			pos = ((QWidget *) obj)->mapToGlobal(pos);

			QRect rect = _docMenu->rect();
			rect.moveTo(QPoint (0, _documentButton->height()));

			if (!rect.contains(pos) && !_documentButton->rect().contains(pos))
				_docMenu->hide();
		}

#ifdef Q_OS_SYMBIAN
		if (obj == _imgContainer || obj == _edit->viewport() || obj == _startLabel) {
#else
		if (obj == _imgContainer || obj == _edit->viewport()) {
#endif
			if (_frame->isVisible()) {
				onAdvancedButtonClicked();
				_ignorePressEvent = true;
			} else if (_isFullscreenMode && !_fullscreenToggle->isVisible()) {
				toggleFullscreenMode();
				_ignorePressEvent = true;
			}

			return false;
		}
	}

	if (ev->type() == QEvent::KeyPress && _mode != NSRViewModeStart) {
		QKeyEvent	*keyEv = (QKeyEvent *) ev;
		QPoint		pos;

		if (_mode == NSRViewModeGraphic)
			pos = _imgContainer->getContentsPosition();
		else
			pos = QPoint (_edit->horizontalScrollBar()->value(),
				      _edit->verticalScrollBar()->value());

#ifdef Q_WS_X11
# ifdef Q_WS_MAEMO_5
		if (keyEv->key () == Qt::Key_F7) {
			     keyEv->accept ();
			     onPrevButtonClicked ();
		} else if (keyEv->key () == Qt::Key_F8) {
			     keyEv->accept ();
			     onNextButtonClicked ();
		}
# else
		if (keyEv->key() == Qt::Key_VolumeUp)
			onPrevButtonClicked ();
		else if (keyEv->key() == Qt::Key_VolumeDown)
			onNextButtonClicked ();
# endif
#endif

		if (keyEv->key() == Qt::Key_Up) {
			pos.setY (pos.y() - 10);

			if (_mode == NSRViewModeGraphic)
				_imgContainer->scrollTo (pos);
			else
				_edit->scrollTo (pos);
		} else if (keyEv->key() == Qt::Key_Down) {
			pos.setY (pos.y() + 10);

			if (_mode == NSRViewModeGraphic)
				_imgContainer->scrollTo (pos);
			else
				_edit->scrollTo (pos);
		} else if (keyEv->key() == Qt::Key_Left && _mode == NSRViewModeGraphic) {
			pos.setX (pos.x() - 10);
			_imgContainer->scrollTo (pos);
		} else if (keyEv->key() == Qt::Key_Right && _mode == NSRViewModeGraphic) {
			pos.setX (pos.x() + 10);
			_imgContainer->scrollTo (pos);
		} else
			return QMainWindow::eventFilter(obj, ev);

		return false;
	}

	return QMainWindow::eventFilter(obj, ev);
}

void nsrMainWindow::timerEvent (QTimerEvent *ev)
{
	if (_delayedLoading && _timerId == ev->timerId ()) {
		NSRSettings	settings;
		NSRSession	session = settings.getLastSession ();
		QColor		invColor = _isInvertedColors ?
						   QColor (30, 30, 30) :
						   QColor (121, 121, 121);

		_imgContainer->setBackgroundColor (invColor);

		_delayedLoading = false;
		loadDocument (session.getFile (), session);

		killTimer (_timerId);
		_timerId = -1;

		goToPage (_page, _lastPos);
	}
}

void nsrMainWindow::updatePreferences()
{
	NSRSettings	settings;
	NSRViewMode	newMode;
	QFont		font;
	bool		needPageReload;

	if (_isFullscreenMode != settings.isFullscreenMode()) {
		_isFullscreenMode = settings.isFullscreenMode();
		toggleFullscreenMode();
		updateCentralWidget();
	}

	if (_isInvertedColors != settings.isInvertedColors())
		switchInvertedMode(settings.isInvertedColors());

	needPageReload = false;

	if (_doc != NULL && (_doc->getEncoding() != settings.getTextEncoding())) {
		_doc->setEncoding(settings.getTextEncoding());
		needPageReload = (_mode == NSRViewModeText);
	}

	if (!_isFormatSpecificMode) {
		newMode = (settings.isWordWrap() ? NSRViewModeText : NSRViewModeGraphic);

		if (newMode == NSRViewModeText && !settings.isTextModeNoted()) {
			settings.saveTextModeNoted();
#ifdef Q_OS_SYMBIAN
			QMessageBox::information(NULL,
						 _langData->mainMessageInfo,
						 _langData->mainFirstTextMode);
#else
			NSRInfoMessageBox *msgBox = new NSRInfoMessageBox (this,
									   _langData->mainMessageInfo,
									   _langData->mainFirstTextMode);
			msgBox->resize (size ());
			msgBox->showMessage ();

			delete msgBox;
#endif
		}

		if (_mode != newMode && _doc != NULL) {
			_doc->setTextOnly(newMode == NSRViewModeText);

			if (newMode == NSRViewModeText) {
				_switchModeAfterRender = true;
				_lateMode = newMode;
			} else
				switchViewMode(newMode);

			needPageReload = true;
		}
	}

	if (_edit->fontFamily() != settings.getFontFamily()) {
		font = _edit->font();
		font.setFamily(settings.getFontFamily());
		_edit->setFont(font);
	}

	if (needPageReload)
		goToPage(_page);
}

QPoint nsrMainWindow::autodetectPagePosition()
{
	/* We do not need to autodetect in text mode */
	if (_mode == NSRViewModeText)
		return QPoint (0, 0);

	QPoint maxPos = _imgContainer->getMaximumContentsPosition();
	QPoint curPos = _imgContainer->getContentsPosition();

	if (curPos.x() != maxPos.x() && curPos.x() != 0)
		return QPoint (curPos.x(), 0);
	else
	    return QPoint (0, 0);
}

bool nsrMainWindow::isPortraitMode()
{
	return (QApplication::desktop()->width() < QApplication::desktop()->height());
}

void nsrMainWindow::switchViewMode(nsrMainWindow::NSRViewMode mode)
{
	if (mode == NSRViewModeGraphic) {
		if (_mode == NSRViewModeText) {
			QPixmap pixmap (_edit->size());
			QPainter fakeView (&pixmap);

			_edit->render(&fakeView);
			_imgContainer->setBetaPixmap (pixmap);
			_imgContainer->setBetaPixmapEnabled (true);
		}

		_edit->hide();
#ifdef Q_WS_X11
		_imgContainer->setStartMode (false);
#endif
		_imgContainer->show();
	} else if (mode == NSRViewModeText) {
		_imgContainer->hide();

		/* Always clean up beta pixmap */
		_imgContainer->setBetaPixmapEnabled(false);
		_imgContainer->clearBetaPixmap();

		_edit->show();
	} else {
		_documentButton->setEnabled(false);
#ifdef Q_OS_SYMBIAN
		_startLabel->show();
#else
		_imgContainer->setStartMode (true);
		_imgContainer->show ();
		_edit->hide ();
#endif
		_pagesLabel->setText("");
		_zoomLabel->setText("");
		/* This blocks pinch gesture if password failed on startup */
		/* _edit->hide();
		_imgContainer->hide(); */
	}

	_mode = mode;
	updateStartLabel();
	updateCentralWidget();
#ifdef Q_WS_X11
	qApp->processEvents ();
#endif
}

void nsrMainWindow::updateAfterConstructor()
{
	updateStartLabel();

	if (_doc == NULL || !fileToOpen.isEmpty())
		return;

	NSRSettings settings;
	NSRSession session;

	session = settings.getLastSession();

	/* Also scroll to last pos */
	if (_doc->isValid() && settings.isLoadLastDoc()) {
		if ( _mode == NSRViewModeGraphic)
			_imgContainer->scrollTo(session.getPosition());
		else if (_mode == NSRViewModeText)
			_edit->scrollTo(session.getPosition());
	}
}

void nsrMainWindow::switchInvertedMode(bool isInverted,  bool updatePage)
{
	QColor invColor = isInverted ? QColor (30, 30, 30) : QColor (121, 121, 121);

	_isInvertedColors = isInverted;

	if (isInverted)
		_edit->setStyleSheet("QTextEdit {background-color: \"black\"; color: \"white\"}");
	else
		_edit->setStyleSheet("QTextEdit {background-color: \"white\"; color: \"black\"}");

	if (_mode != NSRViewModeGraphic) {
		/* Set image's background color anyway */
		if (_doc != NULL)
			_doc->setInvertedColors(isInverted);

		_imgContainer->setBackgroundColor (invColor);
		return;
	}

	if (_doc == NULL || !_doc->isValid())
		return;

	_doc->setInvertedColors(isInverted);

	/* We must re-render page only in graphic mode*/
	if (_mode == NSRViewModeGraphic && updatePage) {
		QPoint lastPos = _imgContainer->getContentsPosition();
		goToPage(_page, lastPos);
	}

	/* We must set background after page re-render */
	_imgContainer->setBackgroundColor (invColor);
}

void nsrMainWindow::showZoomToDialog()
{
	int minZoom, maxZoom, zoom;

	if (_doc == NULL)
		return;

	minZoom = _doc->getMinZoom();
	maxZoom = _doc->getMaxZoom();
	zoom = _doc->getZoom();

#ifdef Q_OS_SYMBIAN
	TPtrC16 textPtr (reinterpret_cast<const TUint16*>(_langData->mainZoomTo.utf16()));

	CAknNumberQueryDialog* dlg =  new CAknNumberQueryDialog (zoom, CAknQueryDialog::ENoTone);
	dlg->PrepareLC (R_DIALOG_NUMBER_QUERY);
	dlg->SetPromptL (textPtr);

	if (dlg->RunLD ())
#else
	NSRInputDialog	*dlg = new NSRInputDialog (this);
	dlg->setWindowTitle ("InputDialog");
	dlg->setInputMode (NSRInputDialog::INPUT_TYPE_NUMERIC);
	dlg->setPasswordMode (false);
	dlg->setInputText (QString::number (zoom));
	dlg->setPromptText (_langData->mainZoomTo);
	dlg->resize (size ());
	dlg->execDialog ();

	QString nm = dlg->getInputString ();
	bool	ok;

	if (dlg->getExitReason () == NSRInputDialog::EXIT_REASON_OK && !nm.isEmpty ())
		zoom = dlg->getInputString().toInt (&ok);
	else
		zoom = -1;

	delete dlg;

	if (zoom > 0 && ok)
#endif
	{
		_doc->setZoom (qBound (minZoom, zoom, maxZoom));
		goToPage (_page, _imgContainer->getContentsPosition());
	}
}

void nsrMainWindow::onPinchZoomRequest(double zoomFactor, QPoint scrollPoint)
{
	if ((_doc->getZoom() == _doc->getMaxZoom() && zoomFactor >= 1.0) ||
	     qAbs (zoomFactor - 1.0) <= DBL_EPSILON) {
		_imgContainer->setBetaPixmapEnabled(false);
		_imgContainer->clearBetaPixmap();
		return;
	}

	_lastPos = scrollPoint;
	_doc->setZoom (_doc->getZoom() * zoomFactor);
	updatePage();
}

void nsrMainWindow::onDocumentButtonClicked()
{
#ifdef Q_OS_SYMBIAN
	_iTouchFeedback->InstantFeedback(ETouchFeedbackBasic);
#endif

	stopKineticScroll();

	if (_docMenuAnimation->state() == QPropertyAnimation::Running)
		_docMenuAnimation->stop();

	if (!_docMenu->isVisible()) {
		disconnect (_docMenuAnimation, SIGNAL (finished()), _docMenu, SLOT (hide()));
		_docMenuAnimation->setStartValue(_docMenu->pos());
		_docMenuAnimation->setEndValue(QPoint (0, _documentButton->height()));
		_docMenu->show();
		_docMenuAnimation->start();

	} else {
		_docMenuAnimation->setStartValue(_docMenu->pos());
		_docMenuAnimation->setEndValue(QPoint (0, _documentButton->height() - _docMenu->height()));
		connect (_docMenuAnimation, SIGNAL (finished()), _docMenu, SLOT (hide()));
		_docMenuAnimation->start();
	}
}

void nsrMainWindow::fullscreenToggleClicked()
{
#ifdef Q_OS_SYMBIAN
	_iTouchFeedback->InstantFeedback(ETouchFeedbackBasic);
#endif
	toggleFullscreenMode();
}

void nsrMainWindow::updateStartLabel()
{
#ifdef Q_OS_SYMBIAN
	if (_mode == NSRViewModeStart) {
		_startLabel->move(0, _fullscreenToggle->isVisible() ? 0 : _documentButton->height());
		_startLabel->resize(width(),
				    _fullscreenToggle->isVisible() ?
					    height() :
					    _prevButton->pos().y() - _documentButton->height());
	}
#endif
}

void nsrMainWindow::updateBottomToolbar()
{
	int widthPortion = width() / 4;
	int yPos = height() - NSR_TOOL_BUTTON_HEIGHT;

	_prevButton->resize(widthPortion, NSR_TOOL_BUTTON_HEIGHT);
	_nextButton->resize(widthPortion, NSR_TOOL_BUTTON_HEIGHT);
	_openButton->resize(widthPortion, NSR_TOOL_BUTTON_HEIGHT);
	_advancedButton->resize(width() - widthPortion * 3, NSR_TOOL_BUTTON_HEIGHT);

	_prevButton->move(0, yPos);
	_nextButton->move(_prevButton->width(), yPos);
	_openButton->move(_nextButton->pos().x() + _nextButton->width(), yPos);
	_advancedButton->move(_openButton->pos().x() + _openButton->width(), yPos);
}

void nsrMainWindow::closeAboutDialog()
{
#ifdef NSR_SYMBIAN_S3
	QPropertyAnimation *dialogAnimation = new QPropertyAnimation(this);
	dialogAnimation->setEasingCurve(QEasingCurve(QEasingCurve::OutCubic));
	dialogAnimation->setDuration(300);
	dialogAnimation->setTargetObject(sender());
	dialogAnimation->setPropertyName("pos");
	dialogAnimation->setStartValue(((NSRTouchDialog *)sender())->pos());
	dialogAnimation->setEndValue(QPoint (0, height()));

	if (sender() == _aboutDlg)
		_aboutDlg = NULL;
	else if (sender() == _prefsDlg)
		_prefsDlg = NULL;

	connect (dialogAnimation, SIGNAL (finished()), sender(), SLOT (deleteLater()));

	dialogAnimation->start(QAbstractAnimation::DeleteWhenStopped);
#else
	if (sender() == _aboutDlg) {
		_aboutDlg->close();
		delete _aboutDlg;
		_aboutDlg = NULL;
	} else if (sender() == _prefsDlg) {
		_prefsDlg->close();
		delete _prefsDlg;
		_prefsDlg = NULL;
	}
#endif
}

void nsrMainWindow::showPreferencesDialog()
{
	if (_prefsDlg != NULL)
		return;
#ifdef NSR_SYMBIAN_S3
	_prefsDlg = new NSRPreferences (this);
	_prefsDlg->move(0, height());
	_prefsDlg->resize(size());

	QPropertyAnimation *dialogAnimation = new QPropertyAnimation(this);
	dialogAnimation->setEasingCurve(QEasingCurve(QEasingCurve::OutCubic));
	dialogAnimation->setDuration(300);
	dialogAnimation->setTargetObject(_prefsDlg);
	dialogAnimation->setPropertyName("pos");
	dialogAnimation->setStartValue(_prefsDlg->pos());
	dialogAnimation->setEndValue(QPoint (0, 0));

	connect (_prefsDlg, SIGNAL (preferencesUpdated()), this, SLOT (updatePreferences()));
	connect (_prefsDlg, SIGNAL (closeRequested()), this, SLOT(closeAboutDialog()));

	_prefsDlg->show();
	dialogAnimation->start(QAbstractAnimation::DeleteWhenStopped);
#else
	_prefsDlg = new NSRPreferences (this);
	_prefsDlg->resize(size());

	connect (_prefsDlg, SIGNAL (preferencesUpdated()), this, SLOT (updatePreferences()));
	connect (_prefsDlg, SIGNAL (closeRequested()), this, SLOT(closeAboutDialog()));

	_prefsDlg->show();
#endif
}

void nsrMainWindow::closeNewsDialog()
{
	_newsDialog->close();
	delete _newsDialog;
	_newsDialog = NULL;
}

void nsrMainWindow::updateTopToolbar()
{
	int widthPortion = width() / 7;

	_documentButton->resize(widthPortion, NSR_TOOL_BUTTON_HEIGHT);
	_zoomOutButton->resize(widthPortion, NSR_TOOL_BUTTON_HEIGHT);
	_zoomInButton->resize(widthPortion, NSR_TOOL_BUTTON_HEIGHT);
	_pagesLabel->resize(widthPortion * 2, NSR_TOOL_BUTTON_HEIGHT);
	_zoomLabel->resize(width() - widthPortion * 5, NSR_TOOL_BUTTON_HEIGHT);

	_documentButton->move(0, 0);
	_pagesLabel->move(_documentButton->width(), 0);
	_zoomLabel->move(_pagesLabel->pos().x() + _pagesLabel->width(), 0);
	_zoomOutButton->move(_zoomLabel->pos().x() + _zoomLabel->width(), 0);
	_zoomInButton->move(_zoomOutButton->pos().x() + _zoomOutButton->width(), 0);
}

void nsrMainWindow::updateCentralWidget()
{
	QWidget *centralWidget;
	int	fixedHeight = _isFullscreenMode ?
				height() : height() - NSR_TOOL_BUTTON_HEIGHT * 2;

	int	yPos = _isFullscreenMode ? 0 : NSR_TOOL_BUTTON_HEIGHT;

#ifdef Q_OS_SYMBIAN
	if (_mode == NSRViewModeGraphic)
#else
	if (_mode == NSRViewModeGraphic || _mode == NSRViewModeStart)
#endif
		centralWidget = _imgContainer;
	else if (_mode == NSRViewModeText)
		centralWidget = _edit;
#ifdef Q_OS_SYMBIAN
	else
		centralWidget = _startLabel;
#endif

	centralWidget->setFixedSize(width(), fixedHeight);
	centralWidget->move(0, yPos);

}

void nsrMainWindow::stopKineticScroll()
{
	if (_mode == NSRViewModeGraphic)
		_imgContainer->stopKineticScroll();
	else if (_mode == NSRViewModeText)
		_edit->stopKineticScroll();
}

void nsrMainWindow::saveSession()
{
	NSRSettings	settings;
	QPoint		pos;

	if (settings.isLoadLastDoc() && _doc != NULL) {
		if (_mode == NSRViewModeGraphic)
			pos = _imgContainer->getContentsPosition();
		else if (_mode == NSRViewModeText)
			pos = QPoint (_edit->horizontalScrollBar()->value(),
				      _edit->verticalScrollBar()->value());
		else
			pos = QPoint (0, 0);

		NSRSession session (_doc->getDocumentPath(),
				    _page,
				    _edit->getZoom(),
				    _doc->getZoom(),
				    _doc->isZoomToWidth(),
				    pos,
				    size(),
				    _doc->getRotation());
		settings.saveSession(&session);
	}
}

void nsrMainWindow::checkViewModeOnLoad(const NSRSettings& settings)
{
	if (!_isFormatSpecificMode)
		return;

	NSRViewMode mode = settings.isWordWrap() ? NSRViewModeText : NSRViewModeGraphic;

	if (_mode != mode)
		switchViewMode(mode);

	_isFormatSpecificMode = false;
}

#ifdef Q_WS_MAEMO_5
void nsrMainWindow::grabZoomKeys (bool grab)
{
	if (!winId ())
		return;

	unsigned long val = (grab) ? 1 : 0;
	Atom atom = XInternAtom (QX11Info::display(), "_HILDON_ZOOM_KEY_ATOM", False);
	if (!atom)
		return;

	XChangeProperty (QX11Info::display(),
			 winId(),
			 atom,
			 XA_INTEGER,
			 32,
			 PropModeReplace,
			 reinterpret_cast<unsigned char *>(&val),
			 1);
}
#endif

#if defined (Q_WS_X11) && !defined (Q_WS_MAEMO_5)
void nsrMainWindow::disableSwipe ()
{
	Display *dpy = QX11Info::display();

	unsigned long val = 1;
	Atom atom = XInternAtom(dpy, "_MEEGOTOUCH_CANNOT_MINIMIZE", false);
	XChangeProperty (dpy,
			this->effectiveWinId(),
			atom,
			XA_CARDINAL,
			32,
			PropModeReplace,
			reinterpret_cast<unsigned char *>(&val),
			1);
}
#endif
