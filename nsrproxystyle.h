#ifndef NSRPROXYSTYLE_H
#define NSRPROXYSTYLE_H

#include <QProxyStyle>

class NSRProxyStyle : public QProxyStyle
{
	Q_OBJECT
public:
	explicit NSRProxyStyle ();

	void drawPrimitive (PrimitiveElement	element,
			    const QStyleOption	*option,
			    QPainter		*painter,
			    const QWidget	*widget) const;
};

#endif // NSRPROXYSTYLE_H
