#ifndef _PRDBLOCK_H
#define _PRDBLOCK_H

#include <fstream>
#include <stdint.h>
#include <string>

#include "types.h"

using namespace std;

class PrdBlock {
private:
	char _name[4];
	uint32_t _length;

	string _version;
	uint16_t _ccdHeight;
	uint16_t _ccdWidth;
	uint16_t _imageHeight;
	uint16_t _imageWidth;
	uint16_t _unknown1;
	uint16_t _unknown2;
	uint16_t _unknown3;
	uint16_t _unknown4;

public:
	PrdBlock(ifstream& mrwFile, int offset);

	const char* getName() const { return _name; }

	uint32_t getLength() const { return _length; }

	string getVersion() const { return _version; }

	uint16_t getCcdHeight() const { return _ccdHeight; }

	uint16_t getCcdWidth() const { return _ccdWidth; }

	uint16_t getImageHeight() const { return _imageHeight; }

	uint16_t getImageWidth() const { return _imageWidth; }

	uint16_t getUnknown1() const { return _unknown1; }

	uint16_t getUnknown2() const { return _unknown2; }

	uint16_t getUnknown3() const { return _unknown3; }

	uint16_t getUnknown4() const { return _unknown4; }

	string getCameraModel() const;

	void dumpRawInfo() const;

private:
	PrdBlock();
};

#endif
