#ifndef _RIFBLOCK_H
#define _RIFBLOCK_H

#include <fstream>
#include <stdint.h>
#include <string>

#include "types.h"

using namespace std;

class RifBlock {
private:
	char _name[4];
	uint32_t _length;

	uint8_t _cameraType;
	int8_t _saturation;
	int8_t _contrast;
	int8_t _sharpness;
	uint8_t _whiteBalance;
	uint8_t _subjectProgram;
	uint16_t _iso;
	uint8_t _colorMode;
	uint8_t _colorFilter;
	uint8_t _bwFilter;

public:
	RifBlock(ifstream& mrwFile, int offset);

	const char* getName() const { return _name; }

	uint32_t getLength() const { return _length; }

	uint16_t getCameraType() const { return _cameraType; }

	int16_t getSaturation() const { return _saturation; }

	int16_t getContrast() const { return _contrast; }

	int16_t getSharpness() const { return _sharpness; }

	uint16_t getWhiteBalance() const { return _whiteBalance; }

	uint16_t getSubjectProgram() const { return _subjectProgram; }

	uint16_t getIso() const { return _iso; }

	uint16_t getColorMode() const { return _colorMode; }

	int16_t getColorFilter() const { return _colorFilter; }

	uint16_t getBwFilter() const { return _bwFilter; }

	string decodeCameraType() const;

	string decodeSharpness() const;

	string decodeWhiteBalance() const;

	string decodeSubjectProgram() const;

	string decodeColorMode() const;

	void dumpRawInfo() const;

protected:
	uint16_t decodeIso(uint8_t isoCode) const;

private:
	RifBlock();
};

#endif
