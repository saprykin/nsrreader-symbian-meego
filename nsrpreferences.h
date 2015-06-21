#ifndef NSRPREFERENCES_H
#define NSRPREFERENCES_H

#include <QWidget>

#include "nsrtouchdialog.h"
#include "nsrsettings.h"

namespace Ui {
    class NSRPreferences;
}

class NSRPreferences : public NSRTouchDialog
{
    Q_OBJECT

public:
    explicit NSRPreferences(QWidget *parent = 0);
    ~NSRPreferences();

private slots:
	void closeButtonClicked();
	void onTextFontButtonClicked();
	void onTextEncodingButtonClicked();

signals:
	void preferencesUpdated();

private:
	Ui::NSRPreferences	*ui;
	NSRSettings		*_settings;
};

#endif // NSRPREFERENCES_H
