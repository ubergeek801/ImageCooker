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

#ifndef _PIXELGROUPINGINTERPOLATOR_H
#define _PIXELGROUPINGINTERPOLATOR_H

#include "Interpolator.h"

class PixelGroupingInterpolator : public Interpolator {

public:
	PixelGroupingInterpolator();

	bool doInterpolateGreen() { return true; }

	uint16_t interpolateGreen(uint16_t x, uint16_t y, const CcdData& ccdData);

	virtual void interpolate(uint16_t x, uint16_t y, const CcdData& ccdData,
			const Pixmap<uint16_t>& pixmap, RgbValue<uint16_t>& rgb);

	virtual ~PixelGroupingInterpolator();

protected:
	uint16_t interpolateDiagonalNeighbors(uint16_t x, uint16_t y, const
			CcdData& ccdData, const Pixmap<uint16_t>& pixmap) const;

	uint16_t interpolateHorizontalNeighbors(uint16_t x, uint16_t y, const
			CcdData& ccdData, const Pixmap<uint16_t>& pixmap) const;

	uint16_t interpolateVerticalNeighbors(uint16_t x, uint16_t y, const
			CcdData& ccdData, const Pixmap<uint16_t>& pixmap) const;

	uint16_t interpolateGreenAtBlueOrRed(uint16_t x, uint16_t y, const
			CcdData& ccdData) const;

	long hueTransit(long g1, long g2, long g3, long v1, long v3) const;
};

#endif
