#include "NoInterpolator.h"

/**
 * NoInterpolator doesn't estimate missing RGB values; rather it just
 * fills them in with zeroes. This isn't useful as an interpolation
 * strategy, but it does produce some "interesting" images in which each
 * pixel has only a red, green or blue value.
 */
NoInterpolator::NoInterpolator() {
}

void NoInterpolator::interpolate(uint16_t x, uint16_t y, const CcdData&
		ccdData, const Pixmap<uint16_t>& pixmap, RgbValue<uint16_t>& rgb) {
	CcdColor ccdColor = ccdData.getPixelColor(x, y);
	switch (ccdColor) {
	case CCD_BLUE:
		rgb.setRgb(0, 0, ccdData.getValue(x, y));
		break;
	case CCD_RED:
		rgb.setRgb(ccdData.getValue(x, y), 0, 0);
		break;
	case CCD_GREEN_BLUE:
	case CCD_GREEN_RED:
		rgb.setRgb(0, ccdData.getValue(x, y), 0);
		break;
	}
}

NoInterpolator::~NoInterpolator() {
}
