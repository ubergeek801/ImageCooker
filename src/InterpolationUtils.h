#ifndef INTERPOLATIONUTILS_H
#define INTERPOLATIONUTILS_H

#include <stdint.h>
#include <stdlib.h>

#include "CcdData.h"
#include "Pixmap.h"
#include "RgbValue.h"

/**
 * InterpolationUtils is a Class Utility that provides common
 * calculations used in multiple interpolation strategies. For example,
 * a number of interpolators use bilinear calculations, so the bilienar
 * calculations are provided here for convenience.
 */
class InterpolationUtils {
public:

	/**
	 * Averages the four pixels diagonally neighboring (that is, the
	 * pixels northeast, southeast, southwest, and northwest of) the
	 * pixel at (x, y).
	 */
	static uint16_t averageDiagonalNeighbors(uint16_t x, uint16_t y, const
			CcdData& ccdData) {
		return ((uint32_t)ccdData.getValue(x - 1, y - 1) + (uint32_t)
				ccdData.getValue(x - 1, y + 1) + (uint32_t)ccdData.
				getValue(x + 1, y - 1) + (uint32_t)ccdData.getValue(x + 1,
				y + 1)) >> 2;
	}

	/**
	 * Averages the four pixels immediately neighboring (that is, the
	 * pixels north, east, south and west of) the pixel at (x, y).
	 */
	static uint16_t averageImmediateNeighbors(uint16_t x, uint16_t y, const
			CcdData& ccdData) {
		return ((uint32_t)ccdData.getValue(x - 1, y) + (uint32_t)ccdData.
				getValue(x, y + 1) + (uint32_t)ccdData.getValue(x + 1, y)
				+ (uint32_t)ccdData.getValue(x, y - 1)) >> 2;
	}

	/**
	 * Averages the two pixels horizontally neighboring (that is, the
	 * pixels east and west of) the pixel at (x, y).
	 */
	static uint16_t averageHorizontalNeighbors(uint16_t x, uint16_t y, const
			CcdData& ccdData) {
		return ((uint32_t)ccdData.getValue(x - 1, y) + (uint32_t)ccdData.
				getValue(x + 1, y)) >> 1;
	}

	/**
	 * Averages the two pixels vertically neighboring (that is, the
	 * pixels north and south of) the pixel at (x, y).
	 */
	static uint16_t averageVerticalNeighbors(uint16_t x, uint16_t y, const
			CcdData& ccdData) {
		return ((uint32_t)ccdData.getValue(x, y - 1) + (uint32_t)ccdData.
				getValue(x, y + 1)) >> 1;
	}

	/**
	 * Averages the two pixels horizontally neighboring (that is, the
	 * pixels east and west of) the pixel at (x, y) using a "smooth hue
	 * transition" calculation. Assumes that green values in the given
	 * Pixmap have already been populated.
	 */
	static uint16_t smoothHueAverageHorizontalNeighbors(uint16_t x, uint16_t
			y, const CcdData& ccdData, const Pixmap<uint16_t>& pixmap) {
	    return MIN((uint16_t)((double)pixmap.getGreenValue(x, y) *
				(((double)ccdData.getValue(x - 1, y) / (double)pixmap.
				getGreenValue(x - 1, y)) + ((double)ccdData.getValue(x +
				1, y) / (double)pixmap.getGreenValue(x + 1, y))) / 2.0),
				4095);
	}

	/**
	 * Averages the two pixels horizontally neighboring (that is, the
	 * pixels east and west of) the pixel at (x, y) using a "smooth hue
	 * transition" calculation. Assumes that green values in the given
	 * Pixmap have already been populated.
	 */
	static uint16_t smoothHueAverageVerticalNeighbors(uint16_t x, uint16_t y,
			const CcdData& ccdData, const Pixmap<uint16_t>& pixmap) {
		return MIN((uint16_t)((double)pixmap.getGreenValue(x, y) *
				(((double)ccdData.getValue(x, y - 1) / (double)pixmap.
				getGreenValue(x, y - 1)) + ((double)ccdData.getValue(x,
				y + 1) / (double)pixmap.getGreenValue(x, y + 1))) /
				2.0), 4095);
	}

	/**
	 * For a pixel (x, y) on the CCD (0 < x < CCD x-max - 1; 0 < y < CCD
	 * y-max - 1), provides the RGB value at that pixel using bilinear
	 * interpolation to estimate the two unknown values.
	 */
	static void interpolateBilinear(uint16_t x, uint16_t y, const CcdData&
			ccdData, RgbValue<uint16_t>& rgb) {
		CcdColor ccdColor = ccdData.getPixelColor(x, y);
		switch (ccdColor) {
		case CCD_BLUE:
			rgb.setRgb(averageDiagonalNeighbors(x, y, ccdData),
					averageImmediateNeighbors(x, y, ccdData),
					ccdData.getValue(x, y));
			break;
		case CCD_RED:
			rgb.setRgb(ccdData.getValue(x, y),
					averageImmediateNeighbors(x, y, ccdData),
					averageDiagonalNeighbors(x, y, ccdData));
			break;
		case CCD_GREEN_BLUE:
			rgb.setRgb(averageVerticalNeighbors(x, y, ccdData),
					ccdData.getValue(x, y),
					averageHorizontalNeighbors(x, y, ccdData));
			break;
		case CCD_GREEN_RED:
			rgb.setRgb(averageHorizontalNeighbors(x, y, ccdData),
					ccdData.getValue(x, y),
					averageVerticalNeighbors(x, y, ccdData));
			break;
		}
	}

	static void uint16sort(uint16_t* array) {
		qsort(array, sizeof(array), sizeof(uint16_t), &uint16compare);
	}

	static void longsort(long* array) {
		qsort(array, sizeof(array), sizeof(long), &longcompare);
	}

protected:
	static int uint16compare(const void* p1, const void* p2) {
		return (int)(*(uint16_t*)p1) - (int)(*(uint16_t*)p2);
	}

	static int longcompare(const void* p1, const void* p2) {
		long l1 = *(long*)p1;
		long l2 = *(long*)p2;
		if (l1 > l2) {
			return 1;
		} else if (l1 < l2) {
			return -1;
		} else {
			return 0;
		}
	}

private:
	InterpolationUtils();

	~InterpolationUtils();
};

#endif
