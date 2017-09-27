#ifndef _NOINTERPOLATOR_H
#define _NOINTERPOLATOR_H

#include "Interpolator.h"

class NoInterpolator : public Interpolator {

public:
	NoInterpolator();

	virtual void interpolate(uint16_t x, uint16_t y, const CcdData& ccdData,
			const Pixmap<uint16_t>& pixmap, RgbValue<uint16_t>& rgb);

	virtual ~NoInterpolator();
};

#endif
