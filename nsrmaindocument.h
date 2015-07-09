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
