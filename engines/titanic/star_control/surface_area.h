/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef TITANIC_SURFACE_OBJ_H
#define TITANIC_SURFACE_OBJ_H

#include "titanic/support/rect.h"
#include "titanic/support/video_surface.h"
#include "titanic/star_control/fpoint.h"
#include "titanic/star_control/frect.h"

namespace Titanic {

enum SurfaceAreaMode {
	SA_NONE = 0, SA_MODE1 = 1, SA_MODE2 = 2, SA_MODE3 = 3, SA_MODE4 = 4
};

class CSurfaceArea {
private:
	/**
	 * Initialize data for the class
	 */
	void initialize();

	/**
	 * Sets the drawing color and mask
	 */
	void setColor(uint rgb);

	void pixelToRGB(uint pixel, uint *rgb);
public:
	int _field0;
	int _width;
	int _height;
	int _pitch;
	int _bpp;
	uint16 *_pixelsPtr;
	FPoint _centroid;
	uint _pixel;
	byte _field24;
	byte _field25;
	byte _field26;
	byte _field27;
	uint _rgb;
	int _field2C;
	uint _colorMask;
	uint _color;
	SurfaceAreaMode _mode;
	Rect _bounds;
public:
	CSurfaceArea(CVideoSurface *surface);

	/**
	 * Sets the drawing mode, and returns the old mode
	 */
	SurfaceAreaMode setMode(SurfaceAreaMode mode);

	/**
	 * Sets the color from the current pixel
	 */
	void setColorFromPixel();

	double fn1(const FRect &rect);
};

} // End of namespace Titanic

#endif /* TITANIC_STAR_CONTROL_SUB16_H */
