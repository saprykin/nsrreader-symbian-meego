#include "nsrsession.h"

NSRSession::NSRSession () :
	_file (QString()),
	_page (0),
	_zoomText (100),
	_zoomGraphic (100),
	_isFitToWidth (false),
	_pos (QPoint (0, 0)),
	_size (360, 480),
	_angle (0)
{

}

NSRSession::NSRSession (const QString& file,
			int page,
			int zoomText,
			int zoomGraphic,
			bool isFitToWidth,
			const QPoint& pos,
			const QSize& size,
			double angle) :
	_file (file),
	_page (page),
	_zoomText (zoomText),
	_zoomGraphic (zoomGraphic),
	_isFitToWidth (isFitToWidth),
	_pos (pos),
	_size (size),
	_angle (angle)
{
}
