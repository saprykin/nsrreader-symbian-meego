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
