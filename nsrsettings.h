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

#ifndef NSRSETTINGS_H
#define NSRSETTINGS_H

#include <QSettings>
#include <QtCore>

#include "nsrsession.h"

class NSRSettings : public QSettings
{
    Q_OBJECT

public:
	explicit NSRSettings (QObject *parent = 0);
	void saveSession (NSRSession *session);
	NSRSession getLastSession ();
	NSRSession getSessionForFile (const QString& file);

	void saveLoadLastDoc (bool load);
	bool isLoadLastDoc () const {return _isLoadLastDoc;}
	void saveFullscreenMode (bool fullscreen);
	bool isFullscreenMode () {return _isFullscreenMode;}
	void saveLastOpenDir (const QString& dir);
	QString getLastOpenDir () const {return _lastOpenDir;}
	void saveWordWrap (bool wrap);
	bool isWordWrap () const {return _isWordWrap;}
	void saveTextModeNoted ();
	bool isTextModeNoted () const {return _isTextModeNoted;}
	void saveInvertedColors (bool inverted);
	bool isInvertedColors () const {return _isInvertedColors;}
	bool isNewsShown () const {return _isNewsShown;}
	void saveNewsShown ();
	QString getFontFamily () const {return _fontFamily;}
	void saveFontFamily (const QString& ff);
	QString getTextEncoding () const {return _textEncoding;}
	void saveTextEncoding (const QString& textEnc);

	static QString getVersion ();
	static QStringList getSupportedEncodings ();

private:
	void	convertFromLegacy ();
	QString	formatFileName (const QString& name);
	void	readSession (const QString& name, NSRSession& session);
	void	cleanOldFiles ();

	bool	_isLoadLastDoc;
	bool	_isFullscreenMode;
	bool	_isWordWrap;
	bool	_isTextModeNoted;
	bool	_isInvertedColors;
	bool	_isNewsShown;
	QString	_lastOpenDir;
	QString	_fontFamily;
	QString	_textEncoding;
};

#endif // NSRSETTINGS_H
