#include "PrdBlock.h"

#include <iostream>

#include "typereader.h"

PrdBlock::PrdBlock(ifstream& mrwFile, int offset) {
	mrwFile.seekg(offset, ifstream::beg);
	// the first four bytes are the four bytes of the block name
	_name[0] = mrwFile.get();
	_name[1] = mrwFile.get();
	_name[2] = mrwFile.get();
	_name[3] = mrwFile.get();
	// the next four bytes are the length of the block
	_length = readuint32(mrwFile);
	// the next eight bytes are the version "string"
	char version[9];
	mrwFile.read(version, 8);
	version[8] = '\0';
	_version = version;
	// the next two bytes are the CCD size y-dimension
	_ccdHeight = readuint16(mrwFile);
	// the next two bytes are the CCD size x-dimension
	_ccdWidth = readuint16(mrwFile);
	// the next two bytes are the image size y-dimension
	_imageHeight = readuint16(mrwFile);
	// the next two bytes are the image size x-dimension
	_imageWidth = readuint16(mrwFile);
	// the next two bytes are "unknown 1" (always seems to be 0x100C)
	_unknown1 = readuint16(mrwFile);
	// the next two bytes are "unknown 2" (always seems to be 0x5200)
	_unknown2 = readuint16(mrwFile);
	// the next two bytes are "unknown 3" (always seems to be 0x0000)
	_unknown3 = readuint16(mrwFile);
	// the next two bytes are "unknown 4" (always seems to be 0x0001)
	_unknown4 = readuint16(mrwFile);

	// these values for the image y-dimension are considered to be a
	// bug, as they are inconsistent with other values (which subtract 8
	// from the CCD x and y dimensions to get image x and y dimensions)
	if (_imageHeight == 1544 && getCameraModel() == "DiMAGE 5") {
		_imageHeight = 1536;
	} else if (_imageHeight == 1928 && getCameraModel() ==
			"DiMAGE 7/7u") {
		_imageHeight = 1920;
	}
}

string PrdBlock::getCameraModel() const {
	if (_version == "27730001") {
		return "DiMAGE 5";
	} else if (_version == "27660001") {
		return "DiMAGE 7/7u";
	} else if (_version == "27790001") {
		return "DiMAGE 7i";
	} else if (_version == "27780001") {
		return "DiMAGE 7Hi";
	}
	// version wasn't one of the recognized types
	return "<Unknown Model>";
}

void PrdBlock::dumpRawInfo() const {
	cout << "    PRD block contents:" << endl;
	cout << "      name: '" << _name[0] << "' '" << _name[1] << "' '" <<
			_name[2] << "' '" << _name[3] << "'" << endl;
	cout << "      length: " << _length << endl;
	cout << "      version: '" << _version << "' (" << getCameraModel()
			<< ")" << endl;
	cout << "      CCD size: " << _ccdWidth << "x" << _ccdHeight <<
			endl;
	cout << "      image size: " << _imageWidth << "x" << _imageHeight
			<< endl;
	cout << "      unknown 1: " << _unknown1 << endl;
	cout << "      unknown 2: " << _unknown2 << endl;
	cout << "      unknown 3: " << _unknown3 << endl;
	cout << "      unknown 4: " << _unknown4 << endl;
}
