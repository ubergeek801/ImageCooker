#include "WbgBlock.h"

#include <iostream>

#include "typereader.h"

WbgBlock::WbgBlock(ifstream& mrwFile, int offset) {
	mrwFile.seekg(offset, ifstream::beg);
	// the first four bytes are the four bytes of the block name
	_name[0] = mrwFile.get();
	_name[1] = mrwFile.get();
	_name[2] = mrwFile.get();
	_name[3] = mrwFile.get();
	// the next four bytes are the length of the block
	_length = readuint32(mrwFile);
	// the next byte is the denominator for the R channel
	_wbRDenominator = decodeWbDenominator(mrwFile.get());
	// the next byte is the denominator for the G channel
	_wbGDenominator = decodeWbDenominator(mrwFile.get());
	// the next byte is the denominator for the G' channel
	_wbGPrimeDenominator = decodeWbDenominator(mrwFile.get());
	// the next byte is the denominator for the B channel
	_wbBDenominator = decodeWbDenominator(mrwFile.get());
	// the next two bytes are the numerator for the R channel
	_wbRNumerator = readuint16(mrwFile);
	// the next two bytes are the numerator for the G channel
	_wbGNumerator = readuint16(mrwFile);
	// the next two bytes are the numerator for the G' channel
	_wbGPrimeNumerator = readuint16(mrwFile);
	// the next two bytes are the numerator for the B channel
	_wbBNumerator = readuint16(mrwFile);
}

uint16_t WbgBlock::decodeWbDenominator(char wbDenominatorCode) {
	// known values are 0->64, 1->128, 2->256, 3->512, 4->1024
	return (uint16_t)64 << wbDenominatorCode;
}

void WbgBlock::dumpRawInfo() const {
	cout << "    WBG block contents:" << endl;
	cout << "      name: '" << _name[0] << "' '" << _name[1] << "' '" <<
			_name[2] << "' '" << _name[3] << "'" << endl;
	cout << "      length: " << _length << endl;
	cout << "      R WB gain coefficient: " << _wbRNumerator << "/" <<
			_wbRDenominator << endl;
	cout << "      G WB gain coefficient: " << _wbGNumerator << "/" <<
			_wbGDenominator << endl;
	cout << "      G' WB gain coefficient: " << _wbGPrimeNumerator << "/" <<
			_wbGPrimeDenominator << endl;
	cout << "      B WB gain coefficient: " << _wbBNumerator << "/" <<
			_wbBDenominator << endl;
}
