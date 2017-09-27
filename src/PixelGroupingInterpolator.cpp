// Pixel Grouping Interpolator Reference Implementation
// Copyright (C) 2003  Chuan-kai Lin <b86506063@ntu.edu.tw>

// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or (at
// your option) any later version.

// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.

#include <stdlib.h>

#include "types.h"
#include "InterpolationUtils.h"
#include "PixelGroupingInterpolator.h"

PixelGroupingInterpolator::PixelGroupingInterpolator() {
}

uint16_t PixelGroupingInterpolator::interpolateGreen(uint16_t x, uint16_t y,
		const CcdData& ccdData) {
	CcdColor ccdColor = ccdData.getPixelColor(x, y);
	switch (ccdColor) {
	case CCD_BLUE:
	case CCD_RED:
		return interpolateGreenAtBlueOrRed(x, y, ccdData);
	case CCD_GREEN_BLUE:
	case CCD_GREEN_RED:
		return ccdData.getValue(x, y);
	}

	// shouldn't be possible to reach here
	return 0;
}

void PixelGroupingInterpolator::interpolate(uint16_t x, uint16_t y, const
		CcdData& ccdData, const Pixmap<uint16_t>& pixmap,
		RgbValue<uint16_t>& rgb) {
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

uint16_t PixelGroupingInterpolator::interpolateDiagonalNeighbors(uint16_t x,
		uint16_t y, const CcdData& ccdData, const Pixmap<uint16_t>& pixmap)
		const {
	long v1 = (long)ccdData.getValue(x - 2, y - 2);
	long v2 = (long)ccdData.getValue(x - 1, y - 1);
	long v3 = (long)ccdData.getValue(x - 2, y + 2);
	long v4 = (long)ccdData.getValue(x - 1, y + 1);
	long v5 = (long)ccdData.getValue(x, y);
	long v6 = (long)ccdData.getValue(x + 1, y - 1);
	long v7 = (long)ccdData.getValue(x + 2, y - 2);
	long v8 = (long)ccdData.getValue(x + 1, y + 1);
	long v9 = (long)ccdData.getValue(x + 2, y + 2);
	long g2 = (long)pixmap.getGreenValue(x - 1, y - 1);
	long g4 = (long)pixmap.getGreenValue(x - 1, y + 1);
	long g5 = (long)pixmap.getGreenValue(x, y);
	long g6 = (long)pixmap.getGreenValue(x + 1, y - 1);
	long g8 = (long)pixmap.getGreenValue(x + 1, y + 1);
	long dv = abs(v1 - v5) + abs(v5 - v9) + abs(v2 - v8) + abs(g2 - g5)
			+ abs(g5 - g8);
	long dh = abs(v3 - v5) + abs(v5 - v7) + abs(v4 - v6) + abs(g4 - g5)
			+ abs(g5 - g6);
	long v;
	if (dv < dh) {
		v = hueTransit(g2, g5, g8, v2, v8);
	} else {
		v = hueTransit(g4, g5, g6, v4, v6);
	}
	return (uint16_t)CAP12(v);
}

uint16_t PixelGroupingInterpolator::interpolateHorizontalNeighbors(uint16_t
		x, uint16_t y, const CcdData& ccdData, const Pixmap<uint16_t>&
		pixmap) const {
	long v1 = ccdData.getValue(x - 1, y);
	long v2 = ccdData.getValue(x, y);
	long v3 = ccdData.getValue(x + 1, y);
	long g1 = (long)pixmap.getGreenValue(x - 1, y);
	long g2 = (long)pixmap.getGreenValue(x, y);
	long g3 = (long)pixmap.getGreenValue(x + 1, y);
	long v = hueTransit(g1, g2, g3, v1, v3);
	return (uint16_t)CAP12(v);
}

uint16_t PixelGroupingInterpolator::interpolateVerticalNeighbors(uint16_t x,
		uint16_t y, const CcdData& ccdData, const Pixmap<uint16_t>& pixmap)
		const {
	long v1 = ccdData.getValue(x, y - 1);
	long v2 = ccdData.getValue(x, y);
	long v3 = ccdData.getValue(x, y + 1);
	long g1 = (long)pixmap.getGreenValue(x, y - 1);
	long g2 = (long)pixmap.getGreenValue(x, y);
	long g3 = (long)pixmap.getGreenValue(x, y + 1);
	long v = hueTransit(g1, g2, g3, v1, v3);
	return (uint16_t)CAP12(v);
}

uint16_t PixelGroupingInterpolator::interpolateGreenAtBlueOrRed(uint16_t x,
		uint16_t y, const CcdData& ccdData) const {
	long v1 = (long)ccdData.getValue(x, y - 2);
	long v2 = (long)ccdData.getValue(x, y - 1);
	long v3 = (long)ccdData.getValue(x - 2, y);
	long v4 = (long)ccdData.getValue(x - 1, y);
	long v5 = (long)ccdData.getValue(x, y);
	long v6 = (long)ccdData.getValue(x + 1, y);
	long v7 = (long)ccdData.getValue(x + 2, y);
	long v8 = (long)ccdData.getValue(x, y + 1);
	long v9 = (long)ccdData.getValue(x, y + 2);
	long dn = abs(v1 - v5) << 1 + abs(v2 - v8);
	long de = abs(v5 - v7) << 1 + abs(v4 - v6);
	long dw = abs(v3 - v5) << 1 + abs(v4 - v6);
	long ds = abs(v5 - v9) << 1 + abs(v2 - v8);
	long gradients[4] = { dn, de, dw, ds };
	InterpolationUtils::longsort(gradients);
	long dm = gradients[0]; // get the smallest gradient
	long v;
	if (dm == dn) {
		v = (3 * v2 + v5 + v8 - v1) >> 2;
	} else if (dm == de) {
		v = (3 * v6 + v5 + v4 - v7) >> 2;
	} else if (dm == dw) {
		v = (3 * v4 + v5 + v6 - v3) >> 2;
	} else {
		v = (3 * v8 + v5 + v2 - v9) >> 2;
	}
	return (uint16_t)CAP12(v);
}

long PixelGroupingInterpolator::hueTransit(long g1, long g2, long g3,
		long v1, long v3) const {
	if ((g2 <= g1 && g2 <= g3) || (g2 >= g1 && g2 >= g3)) {
		return (long)(((v1 + v3) / 2.0) + ((g2 - g1 + g2 - g3) / 4.0));
	} else {
		if (g3 == g1) {
			// prevent division by zero by replacing the (g3 - g1)
			// denominator with 1 (which is very close to 0)
			return v1 + (v3 - v1) * (g2 - g1);
		} else {
			return v1 + (v3 - v1) * (g2 - g1) / (g3 - g1);
		}
	}
}

PixelGroupingInterpolator::~PixelGroupingInterpolator() {
}
