#include <stdlib.h>

#include "EdgeSensing1Interpolator.h"

#include "InterpolationUtils.h"

EdgeSensing1Interpolator::EdgeSensing1Interpolator() {
}

uint16_t EdgeSensing1Interpolator::interpolateGreen(uint16_t x, uint16_t y,
		const CcdData& ccdData) {
	CcdColor ccdColor = ccdData.getPixelColor(x, y);
	switch (ccdColor) {
	case CCD_BLUE:
	case CCD_RED:
		return interpolateImmediateNeighbors(x, y, ccdData);
		break;
	case CCD_GREEN_BLUE:
	case CCD_GREEN_RED:
		return ccdData.getValue(x, y);
		break;
	}
}

void EdgeSensing1Interpolator::interpolate(uint16_t x, uint16_t y, const
		CcdData& ccdData, const Pixmap<uint16_t>& pixmap,
		RgbValue<uint16_t>& rgb) {
	// By the time this method is called, the green values are already
	// populated in the pixmap, so we only need to calculate red and
	// blue components. The remaining interpolation is the same as in
	// SmoothHueTransitionInterpolator (this should be refactored).
	CcdColor ccdColor = ccdData.getPixelColor(x, y);
	switch (ccdColor) {
	case CCD_BLUE:
		rgb.setRgb(interpolateDiagonalNeighbors(x, y, ccdData, pixmap),
				pixmap.getGreenValue(x, y),
				ccdData.getValue(x, y));
		break;
	case CCD_RED:
		rgb.setRgb(ccdData.getValue(x, y),
				pixmap.getGreenValue(x, y),
				interpolateDiagonalNeighbors(x, y, ccdData, pixmap));
		break;
	case CCD_GREEN_BLUE:
		rgb.setRgb(InterpolationUtils::
				smoothHueAverageVerticalNeighbors(x, y, ccdData,
				pixmap), pixmap.getGreenValue(x, y),
				InterpolationUtils::
				smoothHueAverageHorizontalNeighbors(x, y, ccdData,
				pixmap));
		break;
	case CCD_GREEN_RED:
		rgb.setRgb(InterpolationUtils::
				smoothHueAverageHorizontalNeighbors(x, y, ccdData,
				pixmap), pixmap.getGreenValue(x, y),
				InterpolationUtils::smoothHueAverageVerticalNeighbors(x,
				y, ccdData, pixmap));
		break;
	}
}

uint16_t EdgeSensing1Interpolator::interpolateDiagonalNeighbors(uint16_t x,
		uint16_t y, const CcdData& ccdData, const Pixmap<uint16_t>& pixmap)
		const {
	return MIN((uint16_t)((double)pixmap.getGreenValue(x, y) *
			(((double)ccdData.getValue(x - 1, y - 1) /
			(double)pixmap.getGreenValue(x - 1, y - 1)) +
			((double)ccdData.getValue(x + 1, y - 1) /
			(double)pixmap.getGreenValue(x + 1, y - 1)) +
			((double)ccdData.getValue(x - 1, y + 1) /
			(double)pixmap.getGreenValue(x - 1, y + 1)) +
			((double)ccdData.getValue(x + 1, y + 1) /
			(double)pixmap.getGreenValue(x + 1, y + 1))) /
			4.0), 4095);
}

uint16_t EdgeSensing1Interpolator::interpolateImmediateNeighbors(uint16_t x,
		uint16_t y, const CcdData& ccdData) const {
	uint32_t left = ccdData.getValue(x - 1, y);
	uint32_t right = ccdData.getValue(x + 1, y);
	uint32_t above = ccdData.getValue(x, y - 1);
	uint32_t below = ccdData.getValue(x, y + 1);
	uint16_t dH = abs((long)left - (long)right);
	uint16_t dV = abs((long)above - (long)below);
	// we choose threshold value T = (dH + dV) / 2, which simplifies
	// the algorithm as follows
	if (dH < dV) {
		return (uint16_t)((left + right) >> 1);
	} else if (dH > dV) {
		return (uint16_t)((above + below) >> 1);
	}
	return (uint16_t)((left + right + above + below) >> 2);
}

EdgeSensing1Interpolator::~EdgeSensing1Interpolator() {
}
