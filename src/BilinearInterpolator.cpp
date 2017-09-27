#include "BilinearInterpolator.h"

#include "InterpolationUtils.h"

BilinearInterpolator::BilinearInterpolator() {
}

void BilinearInterpolator::interpolate(uint16_t x, uint16_t y, const
		CcdData& ccdData, const Pixmap<uint16_t>& pixmap,
		RgbValue<uint16_t>& rgb) {
	// bilinear interpolation simply interpolates as defined in
	// InterpolationUtils, so the implementation is trivial
	InterpolationUtils::interpolateBilinear(x, y, ccdData, rgb);
}

BilinearInterpolator::~BilinearInterpolator() {
}
