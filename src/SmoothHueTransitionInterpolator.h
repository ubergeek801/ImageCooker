#ifndef _SMOOTHHUETRANSITIONINTERPOLATOR_H
#define _SMOOTHHUETRANSITIONINTERPOLATOR_H

#include "Interpolator.h"

class SmoothHueTransitionInterpolator : public Interpolator {

public:
	SmoothHueTransitionInterpolator();

	bool doInterpolateGreen() { return true; }

	uint16_t interpolateGreen(uint16_t x, uint16_t y, const CcdData& ccdData);

	virtual void interpolate(uint16_t x, uint16_t y, const CcdData& ccdData,
			const Pixmap<uint16_t>& pixmap, RgbValue<uint16_t>& rgb);

	virtual ~SmoothHueTransitionInterpolator();

protected:
	uint16_t interpolateDiagonalNeighbors(uint16_t x, uint16_t y, const
			CcdData& ccdData, const Pixmap<uint16_t>& pixmap) const;
};

#endif
