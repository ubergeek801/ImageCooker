#include "RawDecoder.h"

#include <fstream>
#include <iostream>
#include <stdint.h>

#include "types.h"
#include "GenericException.h"

RawDecoder::RawDecoder(string rawFilename) : _mrmBlock(NULL),
		_progressListener(NULL) {
	// attempt to open the specified file
	ifstream mrwFile(rawFilename.c_str(), ifstream::in | ifstream::
			binary);
	// if the stream isn't good at this point, the file either didn't
	// exist or is inaccessible
	if (!mrwFile.good()) {
		throw GenericException("File not found/not accessible: " +
				rawFilename);
	}

	_mrmBlock = new MrmBlock(mrwFile);

	// after the MRM block is created (it creates its "child" blocks as
	// well), the file pointer will be at the beginning of the CCD data
	uint16_t ccdWidth = _mrmBlock->getPrdBlock().getCcdWidth();
	uint16_t ccdHeight = _mrmBlock->getPrdBlock().getCcdHeight();
	// 2 bytes per sample
	_ccdStreamData = new char[ccdWidth * ccdHeight * 2];
	mrwFile.read(_ccdStreamData, ccdWidth * ccdHeight * 2);

	// we don't need the file anymore
	mrwFile.close();
}

CcdData* RawDecoder::createCcdData() const {
	uint16_t ccdWidth = _mrmBlock->getPrdBlock().getCcdWidth();
	uint16_t ccdHeight = _mrmBlock->getPrdBlock().getCcdHeight();
	CcdData* ccdData = new CcdData(ccdWidth, ccdHeight, (uint8_t*)
			_ccdStreamData);
	// apply white balance correction to CCD data
	short color;
	double wbRCoefficient = (double)_mrmBlock->getWbgBlock().
			getWbRNumerator() / (double)_mrmBlock->getWbgBlock().
			getWbRDenominator();
	double wbGCoefficient = (double)_mrmBlock->getWbgBlock().
			getWbGNumerator() / (double)_mrmBlock->getWbgBlock().
			getWbGDenominator();
	double wbBCoefficient = (double)_mrmBlock->getWbgBlock().
			getWbBNumerator() / (double)_mrmBlock->getWbgBlock().
			getWbBDenominator();
	for (int y = 0; y < ccdHeight; y++) {
		for (int x = 0; x < ccdWidth; x++) {
			color = ccdData->getPixelColor(x, y);
			switch (color) {
			case CCD_RED:
				ccdData->setValue(x, y, (uint16_t)(MIN((double)ccdData->
						getValue(x, y) * wbRCoefficient, 4095)));
				break;
			case CCD_GREEN_RED:
			case CCD_GREEN_BLUE:
				ccdData->setValue(x, y, (uint16_t)(MIN((double)ccdData->
						getValue(x, y) * wbGCoefficient, 4095)));
				break;
			case CCD_BLUE:
				ccdData->setValue(x, y, (uint16_t)(MIN((double)ccdData->
						getValue(x, y) * wbBCoefficient, 4095)));
				break;
			}
		}
		if (_progressListener != NULL) {
			_progressListener->updateProgress((double)(y + 1) / (double)
					ccdHeight);
		}
	}
	return ccdData;
}

void RawDecoder::dumpRawInfo() const {
	cout << "MRW raw contents:" << endl;
	if (_mrmBlock == NULL) {
		cout << "  No MRM block" << endl;
	} else {
		_mrmBlock->dumpRawInfo();
	}
}

RawDecoder::~RawDecoder() {
	delete _ccdStreamData;
	delete _mrmBlock;
}
