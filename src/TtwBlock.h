#ifndef _TTWBLOCK_H
#define _TTWBLOCK_H

#include <fstream>
#include <stdint.h>
#include <string>

#include "types.h"

using namespace std;

class TtwBlock {
private:
	char _name[4];
	uint32_t _length;

	char _byteOrder[3];
	uint16_t _magicNumber;
	uint32_t _firstIfdOffset;
	uint16_t _numIfdEntries;
	char* _ifdEntries;
	uint32_t _nextIfdOffset;

public:
	TtwBlock(ifstream& mrwFile, int offset);

	const char* getName() const { return _name; }

	uint32_t getLength() const { return _length; }

	string getByteOrder() const { return _byteOrder; }

	uint16_t getMagicNumber() const { return _magicNumber; }

	uint32_t getFirstIfdOffset() const { return _firstIfdOffset; }

	uint16_t getNumIfdEntries() const { return _numIfdEntries; }

	char* getIfdEntries() const { return _ifdEntries; }

	uint32_t getNextIfdOffset() const { return _nextIfdOffset; }

	void dumpRawInfo() const;

private:
	TtwBlock();
};

#endif
