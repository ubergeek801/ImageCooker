#ifndef _BILINEARINTERPOLATOR_H
#define _BILINEARINTERPOLATOR_H

#include "Interpolator.h"

class BilinearInterpolator : public Interpolator {

public:
	BilinearInterpolator();

	virtual void interpolate(uint16_t x, uint16_t y, const CcdData& ccdData,
			const Pixmap<uint16_t>& pixmap, RgbValue<uint16_t>& rgb);

	virtual ~BilinearInterpolator();
};

#endif
