#ifndef _EDGESENSING1INTERPOLATOR_H
#define _EDGESENSING1INTERPOLATOR_H

#include "Interpolator.h"

class EdgeSensing1Interpolator : public Interpolator {

public:
	EdgeSensing1Interpolator();

	bool doInterpolateGreen() { return true; }

	uint16_t interpolateGreen(uint16_t x, uint16_t y, const CcdData& ccdData);

	virtual void interpolate(uint16_t x, uint16_t y, const CcdData& ccdData,
			const Pixmap<uint16_t>& pixmap, RgbValue<uint16_t>& rgb);

	virtual ~EdgeSensing1Interpolator();

protected:
	uint16_t interpolateDiagonalNeighbors(uint16_t x, uint16_t y, const
			CcdData& ccdData, const Pixmap<uint16_t>& pixmap) const;

	uint16_t interpolateImmediateNeighbors(uint16_t x, uint16_t y, const
			CcdData& ccdData) const;
};

#endif
