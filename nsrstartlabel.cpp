#include "nsrstartlabel.h"
#include "nsrlanguage.h"

#include <QtCore>
#include <QPainter>
#include <QPinchGesture>
#include <QDebug>

NSRStartLabel::NSRStartLabel(QWidget *parent) :
	QLabel (parent)
{
	const NSRLanguageData*	lang = NSRLanguage::instance()->getLanguage();
	QString			fontSize, align, text;

#ifdef Q_OS_SYMBIAN
	fontSize = QString::number (6);
	align = QString ("justify");
#else
	fontSize = QString::number (12);
	align = QString ("center");
#endif

	text = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">"
	       "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">"
	       "</style></head><body style=\" font-family:'Courier New,courier'; font-size:%1pt; font-weight:600; font-style:normal;\">"
	       "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:%2pt; font-weight:600; color:#ffffff;\">" + lang->startTitle + "</span></p>"
	       "<p align=\"%3\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:5px; margin-right:5px; -qt-block-indent:0; text-indent:0px; font-size:%1pt; font-weight:600; color:#ffffff;\"></p>"
	       "<p align=\"%3\" style=\" margin-top:0px; margin-bottom:0px; margin-left:5px; margin-right:5px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:%1pt; font-weight:600; color:#ffffff;\">" + lang->startTip1 + "</span></p>"
	       "<p align=\"%3\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:5px; margin-right:5px; -qt-block-indent:0; text-indent:0px; font-size:%1pt; font-weight:600; color:#ffffff;\"></p>"
	       "<p align=\"%3\" style=\" margin-top:0px; margin-bottom:0px; margin-left:5px; margin-right:5px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Courier New,courier'; font-size:%1pt; font-weight:600; color:#ffffff;\">" + lang->startTip2 + "</span></p>"
	       "<p align=\"%3\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:5px; margin-right:5px; -qt-block-indent:0; text-indent:0px; font-family:'Courier New,courier'; font-size:%1pt; font-weight:600; color:#ffffff;\"></p>"
	       "<p align=\"%3\" style=\" margin-top:0px; margin-bottom:0px; margin-left:5px; margin-right:5px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Courier New,courier'; font-size:%1pt; font-weight:600; color:#ffffff;\">" + lang->startTip3 + "</span></p>"
	       "<p align=\"%3\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:5px; margin-right:5px; -qt-block-indent:0; text-indent:0px; font-family:'Courier New,courier'; font-size:%1pt; font-weight:600; color:#ffffff;\"></p>"
	       "<p align=\"%3\" style=\" margin-top:0px; margin-bottom:0px; margin-left:5px; margin-right:5px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Courier New,courier'; font-size:%1pt; font-weight:600; color:#ffffff;\">" + lang->startTip4 + "</span></p>"
	       "<p align=\"%3\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:5px; margin-right:5px; -qt-block-indent:0; text-indent:0px; font-family:'Courier New,courier'; font-size:%1pt; font-weight:600; color:#008000;\"></p></body></html>";
	text = text.arg (fontSize).arg (QString::number (fontSize.toInt () + 2)).arg (align);

	/* Initial appearance */
	setStyleSheet ("background-color: rgb(0, 0, 0);");
	setAlignment (Qt::AlignHCenter | Qt::AlignVCenter);
	setWordWrap (true);
	setText (text);
}
