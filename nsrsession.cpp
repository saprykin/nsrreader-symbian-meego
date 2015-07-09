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
