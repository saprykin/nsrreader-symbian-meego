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
