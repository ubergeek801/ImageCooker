#include "BayerPpmDecoder.h"

#include <cstdlib>
#include <fstream>
#include <iostream>

#include "types.h"
#include "GenericException.h"

BayerPpmDecoder::BayerPpmDecoder(string ppmFilename) :
		_ppmFilename(ppmFilename) {
}

CcdData* BayerPpmDecoder::createCcdData() const {
	// attempt to open the specified file
	ifstream ppmFile(_ppmFilename.c_str(), ifstream::in);
	// if the stream isn't good at this point, the file either didn't
	// exist or is inaccessible
	if (!ppmFile.good()) {
		throw GenericException("File not found/not accessible: " +
				_ppmFilename);
	}

	string ignore;
	ppmFile >> ignore; //version
	uint16_t ccdWidth, ccdHeight;
	string widthString, heightString;
	ppmFile >> widthString;
	ppmFile >> heightString;
	ppmFile >> ignore; // max value
	ccdWidth = atoi(widthString.c_str());
	ccdHeight = atoi(heightString.c_str());
	
	// 2 bytes per sample
	uint8_t* ccdStreamData = new uint8_t[ccdWidth * ccdHeight * 2];
	CcdData* ccdData = new CcdData(ccdWidth, ccdHeight, ccdStreamData);

	string redValue, greenValue, blueValue;
	for (uint16_t y = 0; y < ccdHeight; y++) {
		for (uint16_t x = 0; x < ccdWidth; x++) {
			ppmFile >> redValue;
			ppmFile >> greenValue;
			ppmFile >> blueValue;
			CcdColor color = ccdData->getPixelColor(x, y);
			// for now, CCD values are expected to be 12 bits, so
			// promote the 8-bit value from the PPM file to 12 bits
			// below
			switch (color) {
			case CCD_RED:
				ccdData->setValue(x, y, atoi(redValue.c_str()) << 4);
				break;
			case CCD_GREEN_RED:
			case CCD_GREEN_BLUE:
				ccdData->setValue(x, y, atoi(greenValue.c_str()) << 4);
				break;
			case CCD_BLUE:
				ccdData->setValue(x, y, atoi(blueValue.c_str()) << 4);
				break;
			}
		}
	}

	// we don't need the file anymore
	ppmFile.close();

	return ccdData;
}

BayerPpmDecoder::~BayerPpmDecoder() {
}
