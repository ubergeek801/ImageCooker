#ifndef _SMOOTHHUETRANSITIONLOGINTERPOLATOR_H
#define _SMOOTHHUETRANSITIONLOGINTERPOLATOR_H

#include "Interpolator.h"

class SmoothHueTransitionLogInterpolator : public Interpolator {

private:
	Pixmap<double>* _logPixmap;

public:
	SmoothHueTransitionLogInterpolator();

	bool doInterpolateGreen() { return true; }

	virtual uint16_t interpolateGreen(uint16_t x, uint16_t y, const CcdData&
			ccdData);

	virtual void postInterpolateGreen(const CcdData& ccdData, const
			Pixmap<uint16_t>& pixmap);

	virtual void interpolate(uint16_t x, uint16_t y, const CcdData& ccdData,
			const Pixmap<uint16_t>& pixmap, RgbValue<uint16_t>& rgb);

	virtual ~SmoothHueTransitionLogInterpolator();

protected:
	double interpolateDiagonalNeighbors(uint16_t x, uint16_t y);

	double interpolateHorizontalNeighbors(uint16_t x, uint16_t y);

	double interpolateVerticalNeighbors(uint16_t x, uint16_t y);
};

#endif
