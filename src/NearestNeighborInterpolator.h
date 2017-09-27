#ifndef _NEARESTNEIGHBORINTERPOLATOR_H
#define _NEARESTNEIGHBORINTERPOLATOR_H

#include "Interpolator.h"

class NearestNeighborInterpolator : public Interpolator {

public:
	NearestNeighborInterpolator();

	virtual void interpolate(uint16_t x, uint16_t y, const CcdData& ccdData,
			const Pixmap<uint16_t>& pixmap, RgbValue<uint16_t>& rgb);

	virtual ~NearestNeighborInterpolator();
};

#endif
