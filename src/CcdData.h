#ifndef _CCDDATA_H
#define _CCDDATA_H

#include <stdint.h>

#include "types.h"

class CcdData {
private:
	uint16_t _width;
	uint16_t _height;
	uint16_t* _ccdData;

public:
	CcdData(uint16_t width, uint16_t height, uint8_t* msbCcdData);

	uint16_t getWidth() const { return _width; }

	uint16_t getHeight() const { return _height; }

	uint16_t getValue(uint16_t x, uint16_t y) const { return _ccdData[x +
			y * _width]; }

	void setValue(uint16_t x, uint16_t y, uint16_t colorValue) {
			_ccdData[x + y * _width] = colorValue; }

	CcdColor getPixelColor(uint16_t x, uint16_t y) const {
		// odd lines are GBGB; even lines are RGRG
		return y & 1 ? (x & 1 ? CCD_BLUE : CCD_GREEN_BLUE) : (x & 1 ?
				CCD_GREEN_RED : CCD_RED); }

	virtual ~CcdData();

private:
	CcdData();
};

#endif
