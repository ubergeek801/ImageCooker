#ifndef _RGBVALUE_H
#define _RGBVALUE_H

#include "types.h"

// Note that the implementation is provided in this class because g++ has
// serious problems compiling/linking template classes.
template<class pixmap_unit> class RgbValue {

private:
	pixmap_unit _red;
	pixmap_unit _green;
	pixmap_unit _blue;

public:
	RgbValue() : _red(0), _green(0), _blue(0) { }

	RgbValue(pixmap_unit red, pixmap_unit green, pixmap_unit blue) :
			_red(red), _green(green), _blue(blue) { }

	pixmap_unit getRed() const { return _red ; }

	void setRed(pixmap_unit red) { _red = red; }

	pixmap_unit getGreen() const { return _green ; }

	void setGreen(pixmap_unit green) { _green = green; }

	pixmap_unit getBlue() const { return _blue ; }

	void setBlue(pixmap_unit blue) { _blue = blue; }

	void setRgb(pixmap_unit red, pixmap_unit green, pixmap_unit blue) {
			_red = red; _green = green; _blue = blue; }

	virtual ~RgbValue() { }
};

#endif
