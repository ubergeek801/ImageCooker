#ifndef _PIXMAP_H
#define _PIXMAP_H

#include <fstream>
#include <stdint.h>
#include <string>

#include "RgbValue.h"
#include "types.h"

using namespace std;

// Note that the implementation is provided in this class because g++ has
// serious problems compiling/linking template classes.
template<class pixmap_unit> class Pixmap {
private:
	uint16_t _width;
	uint16_t _height;
	pixmap_unit* _pixmapRedData;
	pixmap_unit* _pixmapGreenData;
	pixmap_unit* _pixmapBlueData;

public:
	Pixmap(uint16_t width, uint16_t height) : _width(width), _height(height)
			{
		_pixmapRedData = new pixmap_unit[width * height];
		_pixmapGreenData = new pixmap_unit[width * height];
		_pixmapBlueData = new pixmap_unit[width * height];
	}

	uint16_t getWidth() const { return _width; }

	uint16_t getHeight() const { return _height; }

	pixmap_unit getRedValue(uint16_t x, uint16_t y) const {
		return _pixmapRedData[x + y * _width];
	}

	void setRedValue(uint16_t x, uint16_t y, pixmap_unit redValue) {
		_pixmapRedData[x + y * _width] = redValue;
	}

	pixmap_unit getGreenValue(uint16_t x, uint16_t y) const {
		return _pixmapGreenData[x + y * _width];
	};

	void setGreenValue(uint16_t x, uint16_t y, pixmap_unit greenValue) {
		_pixmapGreenData[x + y * _width] = greenValue;
	}

	pixmap_unit getBlueValue(uint16_t x, uint16_t y) const {
		return _pixmapBlueData[x + y * _width];
	}

	void setBlueValue(uint16_t x, uint16_t y, pixmap_unit blueValue) {
		_pixmapBlueData[x + y * _width] = blueValue;
	}

	void getRgbValue(uint16_t x, uint16_t y, RgbValue<pixmap_unit>&
			rgbValue) const {
		rgbValue.setRed(_pixmapRedData[x + y * _width]);
		rgbValue.setGreen(_pixmapGreenData[x + y * _width]);
		rgbValue.setBlue(_pixmapBlueData[x + y * _width]);
	}

	void setRgbValue(uint16_t x, uint16_t y, const RgbValue<pixmap_unit>&
			rgbValue) {
		_pixmapRedData[x + y * _width] = rgbValue.getRed();
		_pixmapGreenData[x + y * _width] = rgbValue.getGreen();
		_pixmapBlueData[x + y * _width] = rgbValue.getBlue();
	}

	void writePpm(string filename) const {
		RgbValue<pixmap_unit> rgb;
		ofstream ppmFile(filename.c_str(), ofstream::out | ofstream::
				trunc | ofstream::binary);
		ppmFile << "P6 " << _width - 8 << " " << _height - 8 << " 255 ";
		for (int y = 4; y < _height - 4; y++) {
			for (int x = 4; x < _width - 4; x++) {
				// we want to produce a GIMP-compatible PPM, so truncate
				// the 12-bit samples to 8 bits
				getRgbValue(x, y, rgb);
				ppmFile << (char)((uint16_t)rgb.getRed() >> 4) << (char)
						((uint16_t)rgb.getGreen() >> 4) << (char)((uint16_t)
						rgb.getBlue() >> 4);
			}
		}
		ppmFile.close();
	}

	virtual ~Pixmap() {
		delete _pixmapRedData;
		delete _pixmapGreenData;
		delete _pixmapBlueData;
	}

private:
	Pixmap() { }
};

#endif
