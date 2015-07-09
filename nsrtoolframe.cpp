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

#include <QtGui/QtGui>

#include "nsrtoolframe.h"
#include "nsrscrollarea.h"
#include "nsrlanguage.h"

NSRToolFrame::NSRToolFrame(QWidget *parent) :
    QFrame(parent)
{
	const NSRLanguageData *lang = NSRLanguage::instance()->getLanguage();
	setFrameStyle(QFrame::Plain | QFrame::Box);

	setStyleSheet(QString ("QFrame { "
			      "background-color: qlineargradient(spread:pad, "
								"x1:0.5, y1:0, x2:0.5, y2:1, "
								"stop:0 rgba(0, 0, 0, 255), "
								"stop:1 rgba(80, 80, 80, 255)); "
			      "border-top: 2px solid rgba(130, 130, 130, 255); "
			      "border-bottom: 1px solid rgba(30, 30, 30, 255); "
			      "};"));

	QVBoxLayout *layout = new QVBoxLayout (this);
	layout->setContentsMargins(0, 0, 0, 0);

	NSRScrollArea *scrollArea = new NSRScrollArea (this);
	scrollArea->setStyleSheet(QString("QScrollArea {background-color: transparent; border: none;}"));

	QWidget *contentsWidget = new QWidget (this);
	contentsWidget->setObjectName("contentsWidget");
	contentsWidget->setStyleSheet(QString("#contentsWidget {background-color: transparent; border: none;}"));
	QGridLayout *scrollLayout = new QGridLayout (contentsWidget);
	scrollLayout->setContentsMargins(5, 5, 5, 5);

	scrollArea->setWidget(contentsWidget);

	QToolButton *prefBtn = new QToolButton (this);
#ifdef Q_WS_X11
	prefBtn->setStyleSheet ("QToolButton {font-size: 20px;}");
#endif
	prefBtn->setIcon(QIcon (":/icons/icons/gear.png"));
	prefBtn->setIconSize(QSize (32, 32));
	prefBtn->setText(lang->framePreferences);
	prefBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	prefBtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	prefBtn->setMinimumHeight(64);
	scrollLayout->addWidget(prefBtn, 0, 0);

	QToolButton *gotoBtn = new QToolButton (this);
#ifdef Q_WS_X11
	gotoBtn->setStyleSheet ("QToolButton {font-size: 20px;}");
#endif
	gotoBtn->setIcon(QIcon (":/icons/icons/goto.png"));
	gotoBtn->setIconSize(QSize (32, 32));
	gotoBtn->setText(lang->frameGoToPage);
	gotoBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	gotoBtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	gotoBtn->setMinimumHeight(64);
	scrollLayout->addWidget(gotoBtn, 0, 1);

	QToolButton *aboutBtn = new QToolButton (this);
#ifdef Q_WS_X11
	aboutBtn->setStyleSheet ("QToolButton {font-size: 20px;}");
#endif
	aboutBtn->setIcon(QIcon (":/icons/icons/about.png"));
	aboutBtn->setIconSize(QSize (32, 32));
	aboutBtn->setText(lang->frameAbout);
	aboutBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	aboutBtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	aboutBtn->setMinimumHeight(64);
	scrollLayout->addWidget(aboutBtn, 0, 2);

	QToolButton *exitBtn = new QToolButton (this);
#ifdef Q_WS_X11
	exitBtn->setStyleSheet ("QToolButton {font-size: 20px;}");
#endif
	exitBtn->setIcon(QIcon (":/icons/icons/door.png"));
	exitBtn->setIconSize(QSize (32, 32));
	exitBtn->setText(lang->frameExit);
	exitBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	exitBtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	exitBtn->setMinimumHeight(64);
	scrollLayout->addWidget(exitBtn, 1, 0);

	layout->addWidget(scrollArea);

	connect (exitBtn, SIGNAL (clicked()), this, SLOT (onExitClicked()));
	connect (prefBtn, SIGNAL (clicked()), this, SLOT (onPrefsClicked()));
	connect (gotoBtn, SIGNAL (clicked()), this, SLOT (onGotoClicked()));
	connect (aboutBtn, SIGNAL (clicked()), this, SLOT (onAboutClicked()));

#ifdef Q_OS_SYMBIAN
	_iTouchFeedback = MTouchFeedback::Instance();
#endif
}

NSRToolFrame::~NSRToolFrame()
{
}

void NSRToolFrame::onExitClicked()
{
#ifdef Q_OS_SYMBIAN
	_iTouchFeedback->InstantFeedback(ETouchFeedbackBasic);
#endif
	parentWidget()->close();
}

void NSRToolFrame::onPrefsClicked()
{
#ifdef Q_OS_SYMBIAN
	_iTouchFeedback->InstantFeedback(ETouchFeedbackBasic);
#endif
	hide ();
	emit preferencesRequested();
}

void NSRToolFrame::onGotoClicked()
{
#ifdef Q_OS_SYMBIAN
	_iTouchFeedback->InstantFeedback(ETouchFeedbackBasic);
#endif
	hide ();
	emit gotoRequested();
}

void NSRToolFrame::onAboutClicked()
{
#ifdef Q_OS_SYMBIAN
	_iTouchFeedback->InstantFeedback(ETouchFeedbackBasic);
#endif
	hide ();
	emit aboutRequested();
}
