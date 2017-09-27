#ifndef _WBGBLOCK_H
#define _WBGBLOCK_H

#include <fstream>
#include <stdint.h>

#include "types.h"

using namespace std;

class WbgBlock {
private:
	char _name[4];
	uint32_t _length;

	uint16_t _wbRDenominator;
	uint16_t _wbGDenominator;
	uint16_t _wbGPrimeDenominator;
	uint16_t _wbBDenominator;
	uint16_t _wbRNumerator;
	uint16_t _wbGNumerator;
	uint16_t _wbGPrimeNumerator;
	uint16_t _wbBNumerator;

public:
	WbgBlock(ifstream& mrwFile, int offset);

	const char* getName() const { return _name; }

	uint32_t getLength() const { return _length; }

	uint16_t getWbRDenominator() const { return _wbRDenominator; }

	uint16_t getWbGDenominator() const { return _wbGDenominator; }

	uint16_t getWbGPrimeDenominator() const { return _wbGPrimeDenominator; }

	uint16_t getWbBDenominator() const { return _wbBDenominator; }

	uint16_t getWbRNumerator() const { return _wbRNumerator; }

	uint16_t getWbGNumerator() const { return _wbGNumerator; }

	uint16_t getWbGPrimeNumerator() const { return _wbGPrimeNumerator; }

	uint16_t getWbBNumerator() const { return _wbBNumerator; }

	void dumpRawInfo() const;

protected:
	uint16_t decodeWbDenominator(char wbDenominatorCode);

private:
	WbgBlock();
};

#endif
