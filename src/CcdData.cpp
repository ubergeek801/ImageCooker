#include "CcdData.h"

CcdData::CcdData(uint16_t width, uint16_t height, uint8_t* msbCcdData) :
		_width(width), _height(height) {
	// copy the CCD data (which is in MSB form) to the byte order used
	// on our architecture
	uint32_t size = width * height;
	_ccdData = new uint16_t[size];
	for (int i = 0; i < size; i++) {
		_ccdData[i] = ((uint16_t)(msbCcdData[i * 2]) << 8) |
				msbCcdData[i * 2 + 1];
	}
}

CcdData::~CcdData() {
	delete _ccdData;
}
