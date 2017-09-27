#include "TtwBlock.h"

#include <iostream>

#include "typereader.h"

TtwBlock::TtwBlock(ifstream& mrwFile, int offset) {
	mrwFile.seekg(offset, ifstream::beg);
	// the first four bytes are the four bytes of the block name
	_name[0] = mrwFile.get();
	_name[1] = mrwFile.get();
	_name[2] = mrwFile.get();
	_name[3] = mrwFile.get();
	// the next four bytes are the length of the block
	_length = readuint32(mrwFile);
	// the next two bytes are the byte order (always "MM")
	mrwFile.read(_byteOrder, 2);
	_byteOrder[2] = '\0';
	// the next two bytes are the magic number (always 42)
	_magicNumber = readuint16(mrwFile);
	// the next four bytes are the first IFD offset
	_firstIfdOffset = readuint32(mrwFile);
	// the next two bytes are the number of IFD entries
	_numIfdEntries = readuint16(mrwFile);
	// the next _numIfdEntries * 12 bytes are the IFD entries themselves
	_ifdEntries = new char[_numIfdEntries * 12];
	mrwFile.read(_ifdEntries, _numIfdEntries * 12);
	// The next four bytes are the offset of the next IFD. There should
	// only be one IFD, so this should be zero.
	_nextIfdOffset = readuint32(mrwFile);
	// The next n bytes are the values of the EXIF tags in the IFDs; we
	// ignore them for now but have to seek beyond them
	mrwFile.seekg(offset + _length + 8, ifstream::beg);
}

void TtwBlock::dumpRawInfo() const {
	cout << "    TTW block contents:" << endl;
	cout << "      name: '" << _name[0] << "' '" << _name[1] << "' '" <<
			_name[2] << "' '" << _name[3] << "'" << endl;
	cout << "      length: " << _length << endl;
	cout << "      byte order: '" << _byteOrder << "'" << endl;
	cout << "      magic number: " << _magicNumber << endl;
	cout << "      first IFD offset: " << _firstIfdOffset << endl;
	cout << "      number of IFD entries: " << _numIfdEntries << endl;
	cout << "      next IFD offset: " << _nextIfdOffset << endl;
}
