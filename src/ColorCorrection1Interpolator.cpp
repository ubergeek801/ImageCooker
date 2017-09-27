#include <stdlib.h>

#include "ColorCorrection1Interpolator.h"

ColorCorrection1Interpolator::ColorCorrection1Interpolator() {
}

uint16_t ColorCorrection1Interpolator::interpolateGreen(uint16_t x, uint16_t
		y, const CcdData& ccdData) {
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

void ColorCorrection1Interpolator::interpolate(uint16_t x, uint16_t y, const
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
				pixmap.getGreenValue(x, y), ccdData.getValue(x, y));
		break;
	case CCD_RED:
		rgb.setRgb(ccdData.getValue(x, y), pixmap.getGreenValue(x, y),
				interpolateDiagonalNeighbors(x, y, ccdData, pixmap));
		break;
	case CCD_GREEN_BLUE:
		rgb.setRgb(interpolateVerticalNeighbors(x, y, ccdData, pixmap),
				pixmap.getGreenValue(x, y),
				interpolateHorizontalNeighbors(x, y, ccdData, pixmap));
		break;
	case CCD_GREEN_RED:
		rgb.setRgb(interpolateHorizontalNeighbors(x, y, ccdData,
				pixmap), pixmap.getGreenValue(x, y),
				interpolateVerticalNeighbors(x, y, ccdData, pixmap));
		break;
	}
}

uint16_t ColorCorrection1Interpolator::interpolateDiagonalNeighbors(uint16_t
		x, uint16_t y, const CcdData& ccdData, const Pixmap<uint16_t>&
		pixmap) const {
	long gAt = (long)pixmap.getGreenValue(x, y);
	long gNW = (long)pixmap.getGreenValue(x - 1, y - 1);
	long gNE = (long)pixmap.getGreenValue(x + 1, y - 1);
	long gSW = (long)pixmap.getGreenValue(x - 1, y + 1);
	long gSE = (long)pixmap.getGreenValue(x + 1, y + 1);
	long cNW = (long)ccdData.getValue(x - 1, y - 1);
	long cNE = (long)ccdData.getValue(x + 1, y - 1);
	long cSW = (long)ccdData.getValue(x - 1, y + 1);
	long cSE = (long)ccdData.getValue(x + 1, y + 1);
	long dN = abs(cNW - cSE) + abs(gAt - gNW + gAt - gSE);
	long dP = abs(cNE - cSW) + abs(gAt - gNE + gAt - gSW);
	// Some of the following calculations differ from those described
	// in Chen's study because they appear to be wrong, dividing by 2
	// when they should divide by 4, and dividing by 4 when they should
	// divide by 8. It has been determined experimentally that the
	// modified calculations below are correct (and anyway, they're
	// consistent with the calculations performed in
	// interpolateImmediateNeighbors()).
	if (dN < dP) {
		return (uint16_t)MAX(MIN((((cNW + cSE) >> 1) + ((gAt - gNW + gAt -
				gSE) >> 2)), 4095), 0);
	} else if (dN > dP) {
		return (uint16_t)MAX(MIN((((cNE + cSW) >> 1) + ((gAt - gNE + gAt -
				gSW) >> 2)), 4095), 0);
	}
	return (uint16_t)MAX(MIN((((cNW + cNE + cSW + cSE) >> 2) + ((gAt - gNW
			+ gAt - gNE + gAt - gSW + gAt - gSE) >> 3)), 4095), 0);
}

uint16_t ColorCorrection1Interpolator::
		interpolateImmediateNeighbors(uint16_t x, uint16_t y, const CcdData&
		ccdData) const {
	long at = ccdData.getValue(x, y);
	long w1 = ccdData.getValue(x - 1, y);
	long e1 = ccdData.getValue(x + 1, y);
	long n1 = ccdData.getValue(x, y - 1);
	long s1 = ccdData.getValue(x, y + 1);
	long w2 = ccdData.getValue(x - 2, y);
	long e2 = ccdData.getValue(x + 2, y);
	long n2 = ccdData.getValue(x, y - 2);
	long s2 = ccdData.getValue(x, y + 2);
	long dH = abs(w1 - e1) + abs(at - w2 + at - e2);
	long dV = abs(n1 - s1) + abs(at - n2 + at - s2);
	if (dH < dV) {
		return (uint16_t)MAX(MIN((((w1 + e1) >> 1) + ((at - w2 + at - e2)
				>> 2)), 4095), 0);
	} else if (dH > dV) {
		return (uint16_t)MAX(MIN((((n1 + s1) >> 1) + ((at - n2 + at - s2)
				>> 2)), 4095), 0);
	}
	return (uint16_t)MAX(MIN((((n1 + e1 + s1 + w1) >> 2) + ((at - n2 + at
			- e2 + at - s2 + at - w2) >> 3)), 4095), 0);
}

uint16_t ColorCorrection1Interpolator::
		interpolateHorizontalNeighbors(uint16_t x, uint16_t y, const
		CcdData& ccdData, const Pixmap<uint16_t>& pixmap) const {
	long greenAt = pixmap.getGreenValue(x, y);
	return (uint16_t)MAX(MIN((((long)ccdData.getValue(x - 1, y) + (long)
			ccdData.getValue(x + 1, y)) >> 1) + ((greenAt - (long)pixmap.
			getGreenValue(x - 1, y) + greenAt - (long)pixmap.
			getGreenValue(x + 1, y)) >> 2), 4095), 0);
}

uint16_t ColorCorrection1Interpolator::interpolateVerticalNeighbors(uint16_t
		x, uint16_t y, const CcdData& ccdData, const Pixmap<uint16_t>&
		pixmap) const {
	long greenAt = pixmap.getGreenValue(x, y);
	return (uint16_t)MAX(MIN((((long)ccdData.getValue(x, y - 1) + (long)
			ccdData.getValue(x, y + 1)) >> 1) + ((greenAt - (long)pixmap.
			getGreenValue(x, y - 1) + greenAt - (long)pixmap.
			getGreenValue(x, y + 1)) >> 2), 4095), 0);
}

ColorCorrection1Interpolator::~ColorCorrection1Interpolator() {
}
