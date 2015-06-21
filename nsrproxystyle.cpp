#include "nsrproxystyle.h"

NSRProxyStyle::NSRProxyStyle () :
	QProxyStyle ()
{
#ifdef Q_WS_MAEMO_5
	setBaseStyle (new QCommonStyle ());
#endif
}

void NSRProxyStyle::drawPrimitive (QStyle::PrimitiveElement	element,
				   const QStyleOption		*option,
				   QPainter			*painter,
				   const QWidget		*widget) const
{
	if (element == QStyle::PE_FrameFocusRect)
		return;

	QProxyStyle::drawPrimitive (element, option, painter, widget);
}
