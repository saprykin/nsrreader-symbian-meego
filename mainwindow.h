#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFrame>
#include <QScrollBar>
#include <QTextEdit>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QToolButton>

#include "nsrabstractdocument.h"
#include "nsrtoolframe.h"
#include "nsrsettings.h"
#include "nsrscrollarea.h"
#include "nsranimationlabel.h"
#include "nsrrenderthread.h"
#include "nsrtextedit.h"
#include "nsrimagecontainer.h"
#include "nsrstartlabel.h"
#include "nsrdocumentmenu.h"
#include "nsraboutdialog.h"
#include "nsrpreferences.h"
#include "nsrnewsdialog.h"
#include "nsrlanguage.h"

#ifdef Q_OS_SYMBIAN
#  include <remconcoreapitargetobserver.h>
#  include <remconcoreapitarget.h>
#  include <remconinterfaceselector.h>
#  include <touchfeedback.h>
#elif !defined (Q_WS_MAEMO_5)
#  include <resource-set.h>
#endif

namespace Ui {
    class NSRMainWindow;
}

#ifdef Q_OS_SYMBIAN
class nsrMainWindow : public QMainWindow, MRemConCoreApiTargetObserver
#else
class nsrMainWindow : public QMainWindow
#endif
{
	Q_OBJECT
public:
	enum ScreenOrientation {
		ScreenOrientationLockPortrait,
		ScreenOrientationLockLandscape,
		ScreenOrientationAuto
	};

	enum NSRViewMode {
		NSRViewModeGraphic	= 0,
		NSRViewModeText		= 1,
		NSRViewModeStart	= 2
	};

	explicit nsrMainWindow(QWidget *parent = 0);
	virtual ~nsrMainWindow();

	/* Note that this will only have an effect on Symbian and Fremantle. */
	void setOrientation(ScreenOrientation orientation);
	void showExpanded();
	void updateAfterConstructor();
	void loadDocument(const QString&	file,
			  const NSRSession	&session,
			  bool			fromConstructor = false);
#if defined (Q_WS_X11) && !defined (Q_WS_MAEMO_5)
	void disableSwipe ();
#endif

protected:
#ifdef Q_OS_SYMBIAN
	/* MRemConCoreApiTargetObserver interface */
	void MrccatoCommand(TRemConCoreApiOperationId aOperationId,
			    TRemConCoreApiButtonAction aButtonAct);
#endif

	void resizeEvent(QResizeEvent *ev);
	void closeEvent(QCloseEvent *ev);
	bool eventFilter(QObject *obj, QEvent *ev);
	void timerEvent(QTimerEvent *ev);

private:
	void goToPage(int page, const QPoint& pos = QPoint (0, 0));
	void updateNavigationButtons();
	void updateToolFrame();
	void updateFullscreenToggle();
	void updatePageWithoutThread();
	void updateStartLabel();
	void updateBottomToolbar();
	void updateTopToolbar();
	void updateCentralWidget();
	void pageToWidthWithoutThread();
	void setToolbarsVisible(bool visible);
        bool isPortraitMode();
	QPoint autodetectPagePosition();
	void switchViewMode(NSRViewMode mode);
	void switchInvertedMode(bool isInverted, bool updatePage = true);
	void stopKineticScroll();
	void saveSession();
	void checkViewModeOnLoad(const NSRSettings &settings);
#ifdef Q_WS_MAEMO_5
	void grabZoomKeys (bool grab);
#endif

#ifdef Q_OS_SYMBIAN
	/* For MRemConCoreApiTargetObserver interface */
	CRemConInterfaceSelector	*_iInterfaceSelector;
	CRemConCoreApiTarget		*_iCoreTarget;
	MTouchFeedback			*_iTouchFeedback;
#elif !defined (Q_WS_MAEMO_5)
	ResourcePolicy::ResourceSet	*_resourceSet;
#endif

	Ui::NSRMainWindow	*ui;
	QPushButton		*_prevButton;
	QPushButton		*_nextButton;
	QPushButton		*_openButton;
	QPushButton		*_advancedButton;
	QPushButton		*_documentButton;
	QPushButton		*_zoomInButton;
	QPushButton		*_zoomOutButton;
	QLabel			*_pagesLabel;
	QLabel			*_zoomLabel;
	NSRViewMode		_mode;
	NSRViewMode		_lateMode;
	NSRToolFrame		*_frame;
	NSRAbstractDocument	*_doc;
	NSRRenderThread		*_renderThread;
	NSRAnimationLabel	*_animationLabel;
	NSRImageContainer	*_imgContainer;
	NSRStartLabel		*_startLabel;
	NSRTextEdit		*_edit;
	NSRDocumentMenu		*_docMenu;
	NSRAboutDialog		*_aboutDlg;
	NSRPreferences		*_prefsDlg;
	NSRNewsDialog		*_newsDialog;
	const NSRLanguageData	*_langData;
        QPoint                  _lastPos;
	QToolButton		*_fullscreenToggle;
	QPixmap			_cachedPixmap;
	double			_lastPinchFactor;
	int			_page;
	int			_pageCount;
        int                     _lastZoom;
	int			_timerId;
	bool			_isFullscreenMode;
	bool			_isInvertedColors;
	bool			_isFormatSpecificMode;
	bool			_switchModeAfterRender;
	QPropertyAnimation	*_frameAnimation;
	QPropertyAnimation	*_docMenuAnimation;
	bool			_ignorePressEvent;
	bool			_delayedLoading;

private slots:
	void onOpenButtonClicked ();
	void onAdvancedButtonClicked ();
	void onZoomOutButtonClicked ();
	void onZoomInButtonClicked ();
	void showPageSelectDialog ();
	void showAboutDialog ();
	void showZoomToDialog ();
	void toggleFullscreenMode ();
	void onPinchZoomRequest (double zoomFactor, QPoint scrollPoint);
	void onDocumentButtonClicked ();
	void fullscreenToggleClicked ();
	void updateZoomButtons ();
	void closeAboutDialog ();
	void showPreferencesDialog ();
	void closeNewsDialog ();

public slots:
	void pageToWidth();
	void updatePage();
	void updatePageAfterRender();
	void onNextButtonClicked();
	void onPrevButtonClicked();
	void handleDoubleClick (const QPoint& pos);
	void rotatePageLeft();
	void rotatePageRight();
	void updatePreferences();
};

#endif // MAINWINDOW_H
