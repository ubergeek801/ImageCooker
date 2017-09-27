#include "NearestNeighborInterpolator.h"

NearestNeighborInterpolator::NearestNeighborInterpolator() {
}

void NearestNeighborInterpolator::interpolate(uint16_t x, uint16_t y, const
		CcdData& ccdData, const Pixmap<uint16_t>& pixmap,
		RgbValue<uint16_t>& rgb) {
	CcdColor ccdColor = ccdData.getPixelColor(x, y);
	switch (ccdColor) {
	case CCD_BLUE:
		rgb.setRgb(ccdData.getValue(x - 1, y - 1),
				ccdData.getValue(x - 1, y),
				ccdData.getValue(x, y));
		break;
	case CCD_RED:
		rgb.setRgb(ccdData.getValue(x, y),
				ccdData.getValue(x - 1, y),
				ccdData.getValue(x - 1, y - 1));
		break;
	case CCD_GREEN_BLUE:
		rgb.setRgb(ccdData.getValue(x, y - 1),
				ccdData.getValue(x, y),
				ccdData.getValue(x - 1, y));
		break;
	case CCD_GREEN_RED:
		rgb.setRgb(ccdData.getValue(x - 1, y),
				ccdData.getValue(x, y),
				ccdData.getValue(x, y - 1));
		break;
	}
}

NearestNeighborInterpolator::~NearestNeighborInterpolator() {
}
