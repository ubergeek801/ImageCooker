#include <stdlib.h>

#include "EdgeSensing2Interpolator.h"

EdgeSensing2Interpolator::EdgeSensing2Interpolator() {
}

uint16_t EdgeSensing2Interpolator::interpolateGreen(uint16_t x, uint16_t y,
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

void EdgeSensing2Interpolator::interpolate(uint16_t x, uint16_t y, const
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
		rgb.setRgb(interpolateVerticalNeighbors(x, y, ccdData, pixmap),
				pixmap.getGreenValue(x, y),
				interpolateHorizontalNeighbors(x, y, ccdData, pixmap));
		break;
	case CCD_GREEN_RED:
		rgb.setRgb(interpolateHorizontalNeighbors(x, y, ccdData, pixmap),
				pixmap.getGreenValue(x, y),
				interpolateVerticalNeighbors(x, y, ccdData, pixmap));
		break;
	}
}

uint16_t EdgeSensing2Interpolator::interpolateDiagonalNeighbors(uint16_t x,
		uint16_t y, const CcdData& ccdData, const Pixmap<uint16_t>& pixmap)
		const {
	return MAX(MIN((long)pixmap.getGreenValue(x, y) + ((long)(ccdData.
			getValue(x - 1, y - 1) - (long)pixmap.getGreenValue(x - 1, y
			- 1) + (long)ccdData.getValue(x - 1, y + 1) - (long)pixmap.
			getGreenValue(x - 1, y + 1) + (long)ccdData.getValue(x + 1,
			y - 1) - (long)pixmap.getGreenValue(x + 1, y - 1) + (long)
			ccdData.getValue(x + 1, y + 1) - (long)pixmap.
			getGreenValue(x + 1, y + 1)) >> 2), 4095), 0);
}

uint16_t EdgeSensing2Interpolator::interpolateImmediateNeighbors(uint16_t x,
		uint16_t y, const CcdData& ccdData) const {
	uint32_t at = ccdData.getValue(x, y);
	uint32_t w1 = ccdData.getValue(x - 1, y);
	uint32_t e1 = ccdData.getValue(x + 1, y);
	uint32_t n1 = ccdData.getValue(x, y - 1);
	uint32_t s1 = ccdData.getValue(x, y + 1);
	uint32_t w2 = ccdData.getValue(x - 2, y);
	uint32_t e2 = ccdData.getValue(x + 2, y);
	uint32_t n2 = ccdData.getValue(x, y - 2);
	uint32_t s2 = ccdData.getValue(x, y + 2);
	uint16_t dH = abs((long)at - (long)((w2 + e2) >> 1));
	uint16_t dV = abs((long)at - (long)((n2 + s2) >> 1));
	// we choose threshold value T = (dH + dV) / 2, which simplifies
	// the algorithm as follows
	if (dH < dV) {
		return (uint16_t)((w1 + e1) >> 1);
	} else if (dH > dV) {
		return (uint16_t)((n1 + s1) >> 1);
	}
	return (uint16_t)((w1 + e1 + n1 + s1) >> 2);
}

uint16_t EdgeSensing2Interpolator::interpolateHorizontalNeighbors(uint16_t x,
		uint16_t y, const CcdData& ccdData, const Pixmap<uint16_t>& pixmap)
		const {
	return MAX(MIN((long)pixmap.getGreenValue(x, y) + ((long)(ccdData.
			getValue(x - 1, y) - (long)pixmap.getGreenValue(x - 1, y) +
			(long)ccdData.getValue(x + 1, y) - (long)pixmap.
			getGreenValue(x + 1, y)) >> 1), 4095), 0);
}

uint16_t EdgeSensing2Interpolator::interpolateVerticalNeighbors(uint16_t x,
		uint16_t y, const CcdData& ccdData, const Pixmap<uint16_t>& pixmap)
		const {
	return MAX(MIN((long)pixmap.getGreenValue(x, y) + ((long)(ccdData.
			getValue(x, y - 1) - (long)pixmap.getGreenValue(x, y - 1) +
			(long)ccdData.getValue(x, y + 1) - (long)pixmap.
			getGreenValue(x, y + 1)) >> 1), 4095), 0);
}

EdgeSensing2Interpolator::~EdgeSensing2Interpolator() {
}
