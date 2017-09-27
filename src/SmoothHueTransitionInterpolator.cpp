#include "SmoothHueTransitionInterpolator.h"

#include "InterpolationUtils.h"

SmoothHueTransitionInterpolator::SmoothHueTransitionInterpolator() {
}

uint16_t SmoothHueTransitionInterpolator::interpolateGreen(uint16_t x,
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

void SmoothHueTransitionInterpolator::interpolate(uint16_t x, uint16_t y,
		const CcdData& ccdData, const Pixmap<uint16_t>& pixmap,
		RgbValue<uint16_t>& rgb) {
	// By the time this method is called, the green values are already
	// populated in the pixmap, so we only need to calculate red and
	// blue components.
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

uint16_t SmoothHueTransitionInterpolator::
		interpolateDiagonalNeighbors(uint16_t x, uint16_t y, const CcdData&
		ccdData, const Pixmap<uint16_t>& pixmap) const {
	// if any of the green values is zero, use 1 instead (which is close
	// to zero but won't result in a divide by zero error)
	uint16_t nwGreen = MAX(pixmap.getGreenValue(x - 1, y - 1), 1);
	uint16_t neGreen = MAX(pixmap.getGreenValue(x + 1, y - 1), 1);
	uint16_t swGreen = MAX(pixmap.getGreenValue(x - 1, y + 1), 1);
	uint16_t seGreen = MAX(pixmap.getGreenValue(x + 1, y + 1), 1);
	return MIN((uint16_t)((double)pixmap.getGreenValue(x, y) *
			(((double)ccdData.getValue(x - 1, y - 1) / (double)nwGreen)
			+ ((double)ccdData.getValue(x + 1, y - 1) / (double)neGreen)
			+ ((double)ccdData.getValue(x - 1, y + 1) / (double)swGreen)
			+ ((double)ccdData.getValue(x + 1, y + 1) / (double)
			seGreen)) / 4.0), 4095);
}

SmoothHueTransitionInterpolator::~SmoothHueTransitionInterpolator() {
}
