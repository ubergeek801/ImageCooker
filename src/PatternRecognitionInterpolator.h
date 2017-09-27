#ifndef _PATTERNRECOGNITIONINTERPOLATOR_H
#define _PATTERNRECOGNITIONINTERPOLATOR_H

#include "Interpolator.h"

class PatternRecognitionInterpolator : public Interpolator {

public:
	PatternRecognitionInterpolator();

	virtual void interpolate(uint16_t x, uint16_t y, const CcdData& ccdData,
			const Pixmap<uint16_t>& pixmap, RgbValue<uint16_t>& rgb);

	uint16_t interpolatePattern(uint16_t x, uint16_t y, const CcdData&
			ccdData) const;

	void sort(uint16_t* array) const;

	virtual ~PatternRecognitionInterpolator();
};

#endif
