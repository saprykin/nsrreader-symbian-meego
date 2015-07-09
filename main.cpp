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
#include "nsrlanguage.h"

#include <QApplication>
#include <QObject>
#include <QDesktopWidget>
#include <QSplashScreen>

#ifdef Q_OS_SYMBIAN
#  include "nsrmaindocument.h"

#  include <coemain.h>
#  include <eikenv.h>
#  include <eikapp.h>
#  include <eikappui.h>
#else
#  include "nsrproxystyle.h"
#endif

nsrMainWindow *gMainWindow = NULL;
extern QString fileToOpen;

#ifdef Q_OS_SYMBIAN
CApaApplication *newNSRApplication ()
{
	return new NSRMainApplication ();
}

void loadSymbianResourceFile(const QString& resFileName)
{
    TPtrC resFileNameDescriptor (reinterpret_cast<const TText*>(resFileName.constData()), resFileName.length());
    _LIT(KResourcePath, "c:\\resource\\apps\\");
    TFileName appNamePath = CEikonEnv::Static()->EikAppUi()->Application()->AppFullName();
    TFileName resFile (KResourcePath);
    resFile[0] = appNamePath[0];
    resFile.Append (resFileNameDescriptor);

    QT_TRAP_THROWING (CCoeEnv::Static()->AddResourceFileL (resFile));
}
#endif

int main(int argc, char *argv[])
{
	QString pixmapName;

#ifdef Q_OS_SYMBIAN
	QApplication::setGraphicsSystem ("raster");
	QApplication app (newNSRApplication, argc, argv);
#else
	QApplication app (argc, argv);

	if (argc > 1)
		fileToOpen = QString (argv[1]);

	app.setStyle (new NSRProxyStyle ());
#endif

	const NSRLanguageData *lang = NSRLanguage::instance()->getLanguage();

#ifdef Q_OS_SYMBIAN
	loadSymbianResourceFile ("dialogs_20049f7a.rsc");
#endif

#ifdef Q_OS_SYMBIAN
	if (QApplication::desktop()->size().width() > QApplication::desktop()->size().height())
		pixmapName = ":/icons/icons/nsr-splash-wide.png";
	else
		pixmapName = ":/icons/icons/nsr-splash.png";
#else
	pixmapName = ":/icons/icons/nsr-splash-meego.png";
#endif

	QPixmap pixmap (pixmapName);
#ifdef Q_OS_SYMBIAN
	QSplashScreen splash (pixmap.scaled(QApplication::desktop()->size()), Qt::WindowStaysOnTopHint);
#else
	QSplashScreen splash (pixmap, Qt::WindowStaysOnTopHint);
	QFont fnt = splash.font ();
	fnt.setPixelSize (23);
	splash.setFont (fnt);
#endif

	splash.showMessage (lang->splashLoading, Qt::AlignHCenter | Qt::AlignVCenter, QColor (250, 185, 90));
	splash.showFullScreen ();
	splash.raise ();

	nsrMainWindow mainWindow;
	gMainWindow = &mainWindow;

#ifdef Q_OS_SYMBIAN
	mainWindow.setOrientation (nsrMainWindow::ScreenOrientationAuto);
#endif
	mainWindow.showFullScreen ();
	mainWindow.updateAfterConstructor ();
	splash.finish (&mainWindow);

	return app.exec ();
}
