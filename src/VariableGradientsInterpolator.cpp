#include <stdlib.h>

#include "VariableGradientsInterpolator.h"

#include "InterpolationUtils.h"

class Region {
public:
	Region(signed char xOffset, signed char yOffset) : x(xOffset),
			y(yOffset) { }

	signed char x, y;

	~Region() { }
};

class Gradients {
public:
	Gradients() : n(0), ne(0), e(0), se(0), s(0), sw(0), w(0), nw(0),
   			_min(0xffff), _max(0xffff) { }
	uint16_t n;
	uint16_t ne;
	uint16_t e;
	uint16_t se;
	uint16_t s;
	uint16_t sw;
	uint16_t w;
	uint16_t nw;

	uint16_t getMin() {
		if (_min == 0xffff) {
			_min = MIN(n, MIN(ne, MIN(e, MIN(se, MIN(s, MIN(sw, MIN(w,
					nw)))))));
		}
		return _min;
	}

	uint16_t getMax() {
		if (_max == 0xffff) {
			_max = MAX(n, MAX(ne, MAX(e, MAX(se, MAX(s, MAX(sw, MAX(w,
					nw)))))));
		}
		return _max;
	}

	uint16_t getThreshold(double k1, double k2) {
		double min = (double)getMin();
		double max = (double)getMax();
		return (uint16_t)(k1 * min + k2 * (max - min));
	}

	int getRegions(double k1, double k2, Region** regions) {
		uint16_t threshold = getThreshold(k1, k2);
		int i = 0;
		if (n < threshold) {
			regions[i++] = new Region(0, -1);
		}
		if (ne < threshold) {
			regions[i++] = new Region(1, -1);
		}
		if (e < threshold) {
			regions[i++] = new Region(1, 0);
		}
		if (se < threshold) {
			regions[i++] = new Region(1, 1);
		}
		if (s < threshold) {
			regions[i++] = new Region(0, 1);
		}
		if (sw < threshold) {
			regions[i++] = new Region(-1, 1);
		}
		if (w < threshold) {
			regions[i++] = new Region(-1, 0);
		}
		if (nw < threshold) {
			regions[i++] = new Region(-1, -1);
		}
		return i;
	}

	~Gradients() { }

protected:
	uint16_t _min;
	uint16_t _max;
};

VariableGradientsInterpolator::VariableGradientsInterpolator() :
		k1(1.5), k2(0.5) {
}

void VariableGradientsInterpolator::interpolate(uint16_t x, uint16_t y, const
		CcdData& ccdData, const Pixmap<uint16_t>& pixmap,
		RgbValue<uint16_t>& rgb) {
	CcdColor ccdColor = ccdData.getPixelColor(x, y);
	switch (ccdColor) {
	case CCD_BLUE:
		rgb.setBlue(ccdData.getValue(x, y));
		interpolateRedAndGreenAtBlue(x, y, rgb, ccdData);
		break;
	case CCD_RED:
		rgb.setRed(ccdData.getValue(x, y));
		interpolateGreenAndBlueAtRed(x, y, rgb, ccdData);
		break;
	case CCD_GREEN_BLUE:
	case CCD_GREEN_RED:
		rgb.setGreen(ccdData.getValue(x, y));
		interpolateRedAndBlueAtGreen(x, y, rgb, ccdData);
		break;
	}
}

/**
 * Processes the gradients, determining which gradients (if any) are
 * within the threshold and adding those gradients to the sums. If
 * useGradientsForGreen is true, the gradients used to interpolate at a
 * green pixel are used; otherwise the gradients used to interpolate at
 * a non-green pixel are used. If any regions are within the threshold,
 * returns true and populates rSum, gSum, and bSum. If no regions match,
 * performs bilinear interpolation on the current pixel and returns
 * false.
 */
bool VariableGradientsInterpolator::processGradients(uint16_t x, uint16_t y,
		bool useGradientsForGreen, RgbValue<uint16_t>& rgb, const CcdData&
		ccdData, long& rSum, long& gSum, long& bSum, int& numRegions)
		const {
	Gradients gradients;
	if (useGradientsForGreen) {
		getGradientsForGreen(x, y, gradients, ccdData);
	} else {
		getGradientsForNonGreen(x, y, gradients, ccdData);
	}
	Region* regions[8];
	numRegions = gradients.getRegions(k1, k2, regions);
	RgbValue<uint16_t> regionRgb;
	for (int i = 0; i < numRegions; i++) {
		InterpolationUtils::interpolateBilinear(x + regions[i]->x, y +
				regions[i]->y, ccdData, regionRgb);
		rSum += regionRgb.getRed();
		gSum += regionRgb.getGreen();
		bSum += regionRgb.getBlue();
	}
	// we don't need the regions anymore
	for (int i = 0; i < numRegions; i++) {
		delete regions[i];
	}
	if (numRegions == 0) {
		// There aren't any regions within the threshold (this can
		// happen in a 3x3 or greater area of solid color, such as a
		// blown highlight). In this case, bilinear interpolation can't
		// be all that bad, so just do that.
		InterpolationUtils::interpolateBilinear(x, y, ccdData, rgb);
		return false;
	}
	return true;
}

void VariableGradientsInterpolator::interpolateRedAndBlueAtGreen(uint16_t
		x, uint16_t y, RgbValue<uint16_t>& rgb, const CcdData& ccdData)
		const {
	long rSum = 0;
	long gSum = 0;
	long bSum = 0;
	int numRegions = 0;
	if (processGradients(x, y, true, rgb, ccdData, rSum, gSum, bSum,
			numRegions)) {
		rgb.setRed((uint16_t)MIN(MAX((long)rgb.getGreen() + (long)
				((double)(rSum - gSum) / (double)numRegions), 0),
				4095));
		rgb.setBlue((uint16_t)MIN(MAX((long)rgb.getGreen() + (long)
				((double)(bSum - gSum) / (double)numRegions), 0),
				4095));
	}
}

void VariableGradientsInterpolator::interpolateRedAndGreenAtBlue(uint16_t
		x, uint16_t y, RgbValue<uint16_t>& rgb, const CcdData& ccdData)
		const {
	long rSum = 0;
	long gSum = 0;
	long bSum = 0;
	int numRegions = 0;
	if (processGradients(x, y, false, rgb, ccdData, rSum, gSum, bSum,
			numRegions)) {
		rgb.setRed((uint16_t)MIN(MAX((long)rgb.getBlue() + (long)((double)
				(rSum - bSum) / (double)numRegions), 0), 4095));
		rgb.setGreen((uint16_t)MIN(MAX((long)rgb.getBlue() + (long)
				((double)(gSum - bSum) / (double)numRegions), 0),
				4095));
	}
}

void VariableGradientsInterpolator::interpolateGreenAndBlueAtRed(uint16_t
		x, uint16_t y, RgbValue<uint16_t>& rgb, const CcdData& ccdData)
		const {
	long rSum = 0;
	long gSum = 0;
	long bSum = 0;
	int numRegions = 0;
	if (processGradients(x, y, false, rgb, ccdData, rSum, gSum, bSum,
			numRegions)) {
		rgb.setGreen((uint16_t)MIN(MAX((long)rgb.getRed() + (long)
				((double)(gSum - rSum) / (double)numRegions), 0),
				4095));
		rgb.setBlue((uint16_t)MIN(MAX((long)rgb.getRed() + (long)((double)
				(bSum - rSum) / (double)numRegions), 0), 4095));
	}
}

// this macro is defined as a convenience to avoid duplicating the same
// code in the following two methods
#define SETUP_5X5_PIXELS \
	long pix1 = (long)ccdData.getValue(x - 2, y - 2); \
	long pix2 = (long)ccdData.getValue(x - 1, y - 2); \
	long pix3 = (long)ccdData.getValue(x, y - 2); \
	long pix4 = (long)ccdData.getValue(x + 1, y - 2); \
	long pix5 = (long)ccdData.getValue(x + 2, y - 2); \
	long pix6 = (long)ccdData.getValue(x - 2, y - 1); \
	long pix7 = (long)ccdData.getValue(x - 1, y - 1); \
	long pix8 = (long)ccdData.getValue(x, y - 1); \
	long pix9 = (long)ccdData.getValue(x + 1, y - 1); \
	long pix10 = (long)ccdData.getValue(x + 2, y - 1); \
	long pix11 = (long)ccdData.getValue(x - 2, y); \
	long pix12 = (long)ccdData.getValue(x - 1, y); \
	long pix13 = (long)ccdData.getValue(x, y); \
	long pix14 = (long)ccdData.getValue(x + 1, y); \
	long pix15 = (long)ccdData.getValue(x + 2, y); \
	long pix16 = (long)ccdData.getValue(x - 2, y + 1); \
	long pix17 = (long)ccdData.getValue(x - 1, y + 1); \
	long pix18 = (long)ccdData.getValue(x, y + 1); \
	long pix19 = (long)ccdData.getValue(x + 1, y + 1); \
	long pix20 = (long)ccdData.getValue(x + 2, y + 1); \
	long pix21 = (long)ccdData.getValue(x - 2, y + 2); \
	long pix22 = (long)ccdData.getValue(x - 1, y + 2); \
	long pix23 = (long)ccdData.getValue(x, y + 2); \
	long pix24 = (long)ccdData.getValue(x + 1, y + 2); \
	long pix25 = (long)ccdData.getValue(x + 2, y + 2);

void VariableGradientsInterpolator::getGradientsForNonGreen(uint16_t x,
		uint16_t y, Gradients& gradients, const CcdData& ccdData) const {
	SETUP_5X5_PIXELS

	gradients.n = (uint16_t)(abs(pix8 - pix18) + abs(pix3 - pix13) +
			(abs(pix7 - pix17) >> 1) + (abs(pix9 - pix19) >> 1) +
			(abs(pix2 - pix12) >> 1) + (abs(pix4 - pix14) >> 1));
	gradients.ne = (uint16_t)(abs(pix9 - pix17) + abs(pix5 - pix13) +
			(abs(pix8 - pix12) >> 1) + (abs(pix14 - pix18) >> 1) +
			(abs(pix4 - pix8) >> 1) + (abs(pix10 - pix14) >> 1));
	gradients.e = (uint16_t)(abs(pix14 - pix12) + abs(pix15 - pix13) +
			(abs(pix9 - pix7) >> 1) + (abs(pix19 - pix17) >> 1) +
			(abs(pix10 - pix8) >> 1) + (abs(pix20 - pix18) >> 1));
	gradients.se = (uint16_t)(abs(pix19 - pix7) + abs(pix25 - pix13) +
			(abs(pix14 - pix8) >> 1) + (abs(pix18 - pix12) >> 1) +
			(abs(pix20 - pix14) >> 1) + (abs(pix24 - pix18) >> 1));
	gradients.s = (uint16_t)(abs(pix18 - pix8) + abs(pix23 - pix13) +
			(abs(pix19 - pix9) >> 1) + (abs(pix17 - pix7) >> 1) +
			(abs(pix24 - pix14) >> 1) + (abs(pix22 - pix12) >> 1));
	gradients.sw = (uint16_t)(abs(pix17 - pix9) + abs(pix21 - pix13) +
			(abs(pix18 - pix14) >> 1) + (abs(pix12 - pix8) >> 1) +
			(abs(pix22 - pix18) >> 1) + (abs(pix16 - pix12) >> 1));
	gradients.w = (uint16_t)(abs(pix12 - pix14) + abs(pix11 - pix13) +
			(abs(pix17 - pix19) >> 1) + (abs(pix7 - pix9) >> 1) +
			(abs(pix16 - pix18) >> 1) + (abs(pix6 - pix8) >> 1));
	gradients.nw = (uint16_t)(abs(pix7 - pix19) + abs(pix1 - pix13) +
			(abs(pix12 - pix18) >> 1) + (abs(pix8 - pix14) >> 1) +
			(abs(pix6 - pix12) >> 1) + (abs(pix2 - pix8) >> 1));
}

void VariableGradientsInterpolator::getGradientsForGreen(uint16_t x, uint16_t
		y, Gradients& gradients, const CcdData& ccdData) const {
	SETUP_5X5_PIXELS

	gradients.n = (uint16_t)(abs(pix3 - pix13) + abs(pix8 - pix18) +
			(abs(pix7 - pix17) >> 1) + (abs(pix9 - pix19) >> 1) +
			(abs(pix2 - pix12) >> 1) + (abs(pix4 - pix14) >> 1));
	gradients.ne = (uint16_t)(abs(pix9 - pix17) + abs(pix5 - pix13) +
			abs(pix4 - pix12)  + abs(pix10 - pix18));
	gradients.e = (uint16_t)(abs(pix14 - pix12) + abs(pix15 - pix13) +
			(abs(pix9 - pix7) >> 1) + (abs(pix19 - pix17) >> 1) +
			(abs(pix10 - pix8) >> 1) + (abs(pix20 - pix18) >> 1));
	gradients.se = (uint16_t)(abs(pix19 - pix7) + abs(pix25 - pix13) +
			abs(pix20 - pix8) + abs(pix24 - pix12));
	gradients.s = (uint16_t)(abs(pix18 - pix8) + abs(pix23 - pix13) +
			(abs(pix19 - pix9) >> 1) + (abs(pix17 - pix7) >> 1) +
			(abs(pix24 - pix14) >> 1) + (abs(pix22 - pix12) >> 1));
	gradients.sw = (uint16_t)(abs(pix17 - pix9) + abs(pix21 - pix13) +
			abs(pix22 - pix14) + abs(pix16 - pix8));
	gradients.w = (uint16_t)(abs(pix12 - pix14) + abs(pix11 - pix13) +
			(abs(pix17 - pix19) >> 1) + (abs(pix7 - pix9) >> 1) +
			(abs(pix16 - pix18) >> 1) + (abs(pix6 - pix8) >> 1));
	gradients.nw = (uint16_t)(abs(pix7 - pix19) + abs(pix1 - pix13) +
			abs(pix6 - pix18) + abs(pix2 - pix14));
}

VariableGradientsInterpolator::~VariableGradientsInterpolator() {
}
