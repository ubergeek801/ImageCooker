#include "PadBlock.h"

#include <iostream>

#include "typereader.h"

PadBlock::PadBlock(ifstream& mrwFile, int offset) {
	mrwFile.seekg(offset, ifstream::beg);
	// the first four bytes are the four bytes of the block name
	_name[0] = mrwFile.get();
	_name[1] = mrwFile.get();
	_name[2] = mrwFile.get();
	_name[3] = mrwFile.get();
	// the next four bytes are the length of the block
	_length = readuint32(mrwFile);
	// seek to the end of the block
	mrwFile.seekg(offset + _length + 8, ifstream::beg);
}

void PadBlock::dumpRawInfo() const {
	cout << "    PAD block contents:" << endl;
	cout << "      name: '" << _name[0] << "' '" << _name[1] << "' '" <<
			_name[2] << "' '" << _name[3] << "'" << endl;
	cout << "      length: " << _length << endl;
}
