#ifndef _INTERPOLATOR_H
#define _INTERPOLATOR_H

#include <stdint.h>

#include "CcdData.h"
#include "Pixmap.h"
#include "RgbValue.h"

class Interpolator {

public:
	virtual bool doInterpolateGreen() { return false; }

	virtual uint16_t interpolateGreen(uint16_t x, uint16_t y, const CcdData&
			ccdData) { return 0; }

	virtual void postInterpolateGreen(const CcdData& ccdData, const
			Pixmap<uint16_t>& pixmap) { }

	virtual void interpolate(uint16_t x, uint16_t y, const CcdData& ccdData,
			const Pixmap<uint16_t>& pixmap, RgbValue<uint16_t>& rgb) = 0;

	virtual void postInterpolate(const CcdData& ccdData, Pixmap<uint16_t>&
			pixmap) { }

	virtual ~Interpolator();

protected:
	Interpolator();
};

#endif
