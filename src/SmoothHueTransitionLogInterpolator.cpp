#include "math.h"

#include "SmoothHueTransitionLogInterpolator.h"

#include "InterpolationUtils.h"

static double logTable[4096];

SmoothHueTransitionLogInterpolator::
		SmoothHueTransitionLogInterpolator() : _logPixmap(NULL) {
	// Initialize the log table. log(0) isn't valid, so we substitute a
	// value similar to negative infinity
	logTable[0] = -8191.0;
	for (int i = 1; i < 4096; i++) {
		logTable[i] = log((double)i);
	}
}

uint16_t SmoothHueTransitionLogInterpolator::interpolateGreen(uint16_t x,
		uint16_t y, const CcdData& ccdData) {
	CcdColor ccdColor = ccdData.getPixelColor(x, y);
	switch (ccdColor) {
	case CCD_BLUE:
	case CCD_RED:
		return InterpolationUtils::averageImmediateNeighbors(x, y,
				ccdData);
		break;
	case CCD_GREEN_BLUE:
	case CCD_GREEN_RED:
		return ccdData.getValue(x, y);
		break;
	}
}

void SmoothHueTransitionLogInterpolator::postInterpolateGreen(const
		CcdData& ccdData, const Pixmap<uint16_t>& pixmap) {
	// First we need to convert all data to logarithmic values, using
	// a second Pixmap instead of the CcdData. We already have the
	// green values in our existing Pixmap, so we'll copy those over,
	// then we'll copy the red/blue data from the CcdData. Note that
	// this value may represent either the red or blue value, but we
	// "abuse" the Pixmap a little bit by always putting this value
	// into the red component (so it's easier to find during
	// interpolation).
	if (_logPixmap != NULL) {
		delete _logPixmap;
	}
	_logPixmap = new Pixmap<double>(ccdData.getWidth(), ccdData.
			getHeight());
	RgbValue<double> rgb;
	for (int y = 1; y < ccdData.getHeight() - 1; y++) {
		for (int x = 1; x < ccdData.getWidth() - 1; x++) {
			rgb.setRgb(logTable[ccdData.getValue(x, y)],
					logTable[pixmap.getGreenValue(x, y)], 0);
			_logPixmap->setRgbValue(x, y, rgb);
		}
	}
}

void SmoothHueTransitionLogInterpolator::interpolate(uint16_t x, uint16_t y,
		const CcdData& ccdData, const Pixmap<uint16_t>& pixmap,
		RgbValue<uint16_t>& rgb) {
	// The behavior here is different from many other interpolation
	// algorithms because we're performing interpolation on our private
	// Pixmap, where we've stored color values in logarithmic space).
	// Therefore we pretty much ignore (for interpolation purposes) the
	// CcdData and the Pixmap passed to us.
	CcdColor ccdColor = ccdData.getPixelColor(x, y);
	double logRed, logGreen, logBlue;
	switch (ccdColor) {
	case CCD_BLUE:
		logRed = interpolateDiagonalNeighbors(x, y);
		logGreen = _logPixmap->getGreenValue(x, y),
		rgb.setRgb(MIN((uint16_t)exp(logRed), 4095), MIN((uint16_t)
				exp(logGreen), 4095), ccdData.getValue(x, y));
		break;
	case CCD_RED:
		logGreen = _logPixmap->getGreenValue(x, y);
		logBlue = interpolateDiagonalNeighbors(x, y);
		rgb.setRgb(ccdData.getValue(x, y), MIN((uint16_t)exp(logGreen),
				4095), MIN((uint16_t)exp(logBlue), 4095));
		break;
	case CCD_GREEN_BLUE:
		logRed = interpolateVerticalNeighbors(x, y);
		logGreen = _logPixmap->getGreenValue(x, y);
		logBlue = interpolateHorizontalNeighbors(x, y);
		rgb.setRgb(MIN((uint16_t)exp(logRed), 4095), MIN((uint16_t)
				exp(logGreen), 4095), MIN((uint16_t)exp(logBlue), 4095));
		break;
	case CCD_GREEN_RED:
		logRed = interpolateHorizontalNeighbors(x, y);
		logGreen = _logPixmap->getGreenValue(x, y);
		logBlue = interpolateVerticalNeighbors(x, y);
		rgb.setRgb(MIN((uint16_t)exp(logRed), 4095), MIN((uint16_t)
				exp(logGreen), 4095), MIN((uint16_t)exp(logBlue), 4095));
		break;
	}
}

// Note that for the interpolation methods, we "abuse" the logarithmic
// Pixmap a little bit by storing the red/blue value from the CCD in the
// the red component.

double SmoothHueTransitionLogInterpolator::
		interpolateDiagonalNeighbors(uint16_t x, uint16_t y) {
	return _logPixmap->getGreenValue(x, y) + ((_logPixmap->getRedValue(x
			- 1, y - 1) - _logPixmap->getGreenValue(x - 1, y - 1) +
			_logPixmap->getRedValue(x + 1, y - 1) - _logPixmap->
			getGreenValue(x + 1, y - 1) + _logPixmap->getRedValue(x - 1,
			y + 1) - _logPixmap->getGreenValue(x - 1, y + 1) +
			_logPixmap->getRedValue(x + 1, y + 1) - _logPixmap->
			getGreenValue(x + 1, y + 1)) / 4.0);
}

double SmoothHueTransitionLogInterpolator::
		interpolateHorizontalNeighbors(uint16_t x, uint16_t y) {
	return _logPixmap->getGreenValue(x, y) + ((_logPixmap->getRedValue(x
			- 1, y) - _logPixmap->getGreenValue(x - 1, y) + _logPixmap->
			getRedValue(x + 1, y) - _logPixmap->getGreenValue(x + 1, y))
			/ 2.0);
}

double SmoothHueTransitionLogInterpolator::
		interpolateVerticalNeighbors(uint16_t x, uint16_t y) {
	return _logPixmap->getGreenValue(x, y) + ((_logPixmap->getRedValue(x,
			y - 1) - _logPixmap->getGreenValue(x, y - 1) + _logPixmap->
			getRedValue(x, y + 1) - _logPixmap->getGreenValue(x, y + 1))
			/ 2.0);
}

SmoothHueTransitionLogInterpolator::~SmoothHueTransitionLogInterpolator() {
	if (_logPixmap != NULL) {
		delete _logPixmap;
	}
}
