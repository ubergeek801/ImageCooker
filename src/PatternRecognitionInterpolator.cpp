#include "PatternRecognitionInterpolator.h"

#include <stdlib.h>

#include "InterpolationUtils.h"

#define EDGE_PATTERN 0
#define STRIPE_PATTERN 1
#define CORNER_PATTERN 2
#define SOLID_PATTERN 3

PatternRecognitionInterpolator::PatternRecognitionInterpolator() {
}

void PatternRecognitionInterpolator::interpolate(uint16_t x, uint16_t y,
		const CcdData& ccdData, const Pixmap<uint16_t>& pixmap,
		RgbValue<uint16_t>& rgb) {
	// red and blue values are interpolated using the same algorithm as
	// bilinear; green values are interpolated according to pattern
	// matching
	CcdColor ccdColor = ccdData.getPixelColor(x, y);
	switch (ccdColor) {
	case CCD_BLUE:
		rgb.setRgb(InterpolationUtils::averageDiagonalNeighbors(x, y,
				ccdData), interpolatePattern(x, y, ccdData), ccdData.
				getValue(x, y));
		break;
	case CCD_RED:
		rgb.setRgb(ccdData.getValue(x, y), interpolatePattern(x, y,
				ccdData), InterpolationUtils::
				averageDiagonalNeighbors(x, y, ccdData));
		break;
	case CCD_GREEN_BLUE:
		rgb.setRgb(InterpolationUtils::averageVerticalNeighbors(x, y,
				ccdData), ccdData.getValue(x, y), InterpolationUtils::
				averageHorizontalNeighbors(x, y, ccdData));
		break;
	case CCD_GREEN_RED:
		rgb.setRgb(InterpolationUtils::averageHorizontalNeighbors(x, y,
				ccdData), ccdData.getValue(x, y), InterpolationUtils::
				averageVerticalNeighbors(x, y, ccdData));
		break;
	}
}

uint16_t PatternRecognitionInterpolator::interpolatePattern(uint16_t x,
		uint16_t y, const CcdData& ccdData) const {
	uint16_t n = ccdData.getValue(x, y - 1);
	uint16_t e = ccdData.getValue(x + 1, y);
	uint16_t s = ccdData.getValue(x, y + 1);
	uint16_t w = ccdData.getValue(x - 1, y);
	double avg = ((double)n + (double)e + (double)s + (double)w) / 4.0;
	// Determine what pattern we're working with.
	unsigned char pattern;
	int numAboveAvg = 0;
	if ((double)n > avg) {
		numAboveAvg++;
	}
	if ((double)e > avg) {
		numAboveAvg++;
	}
	if ((double)s > avg) {
		numAboveAvg++;
	}
	if ((double)w > avg) {
		numAboveAvg++;
	}
	if (numAboveAvg == 1 || numAboveAvg == 3) {
		// If there is exactly one or exactly three pixels above the
		// average, then we have an edge pattern.
		pattern = EDGE_PATTERN;
	} else if (numAboveAvg == 0 || numAboveAvg == 4) {
		// If there aren't any pixels above the average, then they must
		// all equal the average. It shouldn't be possible for all of
		// the pixels to be above the average, but we include that case
		// here just in case there's some weird floating point behavior.
		pattern = SOLID_PATTERN;
	} else {
		// There are exactly two pixels above the average, and thus two
		// at or below the average. This corresponds to a stripe or
		// corner pattern; now find out which. Basically if n and s are
		// above average, or e and w are above average, then we have a
		// stripe pattern, otherwise it's a corner.
		if (((double)n > avg && (double)s > avg) || ((double)e > avg &&
				(double)w > avg)) {
			pattern = STRIPE_PATTERN;
		} else {
			pattern = CORNER_PATTERN;
		}
	}
	uint16_t array[4] = { n, e, s, w };
	InterpolationUtils::uint16sort(array);
	uint16_t B = array[2]; // B is defined to be the 2nd-largest value
	uint16_t C = array[1]; // C is defined to be the 3rd-largest value
	switch (pattern) {
	case EDGE_PATTERN:
		// the interpolated value is the average of the two median
		// values "B" and "C"
		return (B + C) >> 1;
	case SOLID_PATTERN:
		// in this case, all green neighbors have the same value, so
		// rather than interpolate, just return any one of them
		return n;
	case STRIPE_PATTERN:
		{
			uint16_t estimate = ((n + e + s + w) >> 1) - ((ccdData.
					getValue(x + 1, y - 2) + ccdData.getValue(x + 2, y -
					1) + ccdData.getValue(x + 2, y + 1) + ccdData.
					getValue(x + 1, y + 2) + ccdData.getValue(x - 1, y +
					2) + ccdData.getValue(x - 2, y + 1) + ccdData.
					getValue(x - 2, y - 1) + ccdData.getValue(x - 1, y -
					2)) >> 3);
			return MAX(MIN(estimate, B), C);
		}
	case CORNER_PATTERN:
		{
			uint16_t estimate = ((n + e + s + w) >> 1) + ((ccdData.
					getValue(x + 1, y - 2) + ccdData.getValue(x + 2, y -
					1) + ccdData.getValue(x - 1, y + 2) + ccdData.
					getValue(x - 2, y - 1)) >> 2);
			return MAX(MIN(estimate, B), C);
		}
	}
}

PatternRecognitionInterpolator::~PatternRecognitionInterpolator() {
}
