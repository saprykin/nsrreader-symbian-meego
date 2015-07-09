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

#include "nsrmaindocument.h"
#include "mainwindow.h"

#include <QDebug>

#include <APGWGNAM.H>
#include <APACMDLN.H>

extern QString fileToOpen;
extern nsrMainWindow *gMainWindow;

void NSRMainAppUi::OpenFileL (const TDesC &aFileName)
{
	fileToOpen = QString ((QChar *) aFileName.Ptr(), aFileName.Length());

	if (gMainWindow != NULL) {
		NSRSettings settings;
		gMainWindow->loadDocument (fileToOpen, settings.getSessionForFile(fileToOpen));
	}
}

void NSRMainDocument::OpenFileL(CFileStore *&aFileStore, RFile &aFile)
{
	QS60MainDocument::OpenFileL(aFileStore, aFile);

	TBuf<256> filename;
	aFile.FullName (filename);

	fileToOpen = QString ((QChar *) filename.Ptr(), filename.Length());
}

CFileStore *NSRMainDocument::OpenFileL (TBool aDoOpen, const TDesC &aFilename, RFs &aFs)
{
	fileToOpen = QString ((QChar *) aFilename.Ptr(), aFilename.Length());
	return QS60MainDocument::OpenFileL(aDoOpen, aFilename, aFs);
}

CEikAppUi *NSRMainDocument::CreateAppUiL()
{
	return (static_cast <CEikAppUi*> (new (ELeave) NSRMainAppUi ()));
}

CApaDocument * NSRMainApplication::CreateDocumentL()
{
	return new (ELeave) NSRMainDocument(*this);
}

void NSRMainApplication::PreDocConstructL()
{
	CEikonEnv* env = CEikonEnv::Static();

	// Check that this app is started as stand-alone
	if (!env->StartedAsServerApp() && !env->EikAppUi()) {
		RWsSession& ws = env->WsSession();
		const TInt myWgId = env->RootWin().Identifier();
		TInt wgId = 0;
		TUid uid (AppDllUid());

		// Look for another instance of this app
		while (wgId >= 0) {
			if (wgId && wgId != myWgId) { // Another instance found -> close it
				TApaTask other(ws);
				other.SetWgId(wgId);
				other.EndTask();          // Requires SwEvent capability?
			}

			CApaWindowGroupName::FindByAppUid (uid, ws, wgId);
		}
	}
}
