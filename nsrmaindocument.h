#ifndef NSRMAINDOCUMENT_H
#define NSRMAINDOCUMENT_H

#include <QS60MainDocument>
#include <QS60MainApplication>
#include <QS60MainAppUi>

class NSRMainAppUi : public QS60MainAppUi
{
public:
	NSRMainAppUi () :
		QS60MainAppUi ()
	{}

	void OpenFileL (const TDesC &aFileName);
};

class NSRMainDocument : public QS60MainDocument
{
public:
	NSRMainDocument (CEikApplication& mainApplication) :
		QS60MainDocument (mainApplication)
	{}

	void OpenFileL (CFileStore *&aFileStore, RFile &aFile);
	CFileStore * OpenFileL (TBool aDoOpen, const TDesC &aFilename, RFs &aFs);
	virtual CEikAppUi *CreateAppUiL();
};

class NSRMainApplication : public QS60MainApplication
{
public:
	NSRMainApplication () :
		QS60MainApplication ()
	{}

	CApaDocument * CreateDocumentL ();
	void PreDocConstructL();
};

#endif // NSRMAINDOCUMENT_H
