#ifndef NSRSESSION_H
#define NSRSESSION_H

#include <QtCore>

class NSRSession
{
public:
	NSRSession ();
	NSRSession (const QString& file,
		    int page,
		    int zoomText,
		    int zoomGraphic,
		    bool isFitToWidth,
		    const QPoint& pos,
		    const QSize& size,
		    double angle);

	QString getFile () const {return _file;}
	int getPage () const {return _page;}
	int getZoomText () const {return _zoomText;}
	int getZoomGraphic () const {return _zoomGraphic;}
	bool isFitToWidth () const {return _isFitToWidth;}
	QPoint getPosition () const {return _pos;}
	QSize getSize () const {return _size;}
	double getRotation () const {return _angle;}

	void setFile (const QString& file) {_file = file;}
	void setPage (int page) {_page = page;}
	void setZoomText (int zoom) {_zoomText = zoom;}
	void setZoomGraphic (int zoom) {_zoomGraphic = zoom;}
	void setFitToWidth (bool fit) {_isFitToWidth = fit;}
	void setPosition (const QPoint& pos) {_pos = pos;}
	void setSize (const QSize& size) {_size = size;}
	void setRotation (double angle) {_angle = angle;}

private:
	QString _file;
	int	_page;
	int	_zoomText;
	int	_zoomGraphic;
	bool	_isFitToWidth;
	QPoint	_pos;
	QSize	_size;
	double	_angle;
};

#endif // NSRSESSION_H
