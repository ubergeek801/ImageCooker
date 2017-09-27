#include "RifBlock.h"

#include <iostream>
#include <math.h>

#include "typereader.h"

const char* CAMERA_TYPE_STRINGS[] = {
	"DiMAGE 7/7u",
	"DiMAGE 7i/7Hi"
};

// indexed from -1 !
const char* SHARPNESS_STRINGS[] = {
	"Soft",
	"Normal",
	"Hard"
};

const char* WHITE_BALANCE_STRINGS[] = {
	"Auto",
	"Daylight",
	"Cloudy",
	"Tungsten",
	"Flourescent"
};

const char* SUBJECT_PROGRAM_STRINGS[] = {
	"None",
	"Portrait",
	"Text",
	"Night Portrait",
	"Sunset",
	"Sports Action"
};

const char* COLOR_MODE_STRINGS[] = {
	"Normal",
	"Black and White",
	"Vivid Color",
	"Solarization",
	"Adobe RGB"
};

RifBlock::RifBlock(ifstream& mrwFile, int offset) {
	mrwFile.seekg(offset, ifstream::beg);
	// the first four bytes are the four bytes of the block name
	_name[0] = mrwFile.get();
	_name[1] = mrwFile.get();
	_name[2] = mrwFile.get();
	_name[3] = mrwFile.get();
	// the next four bytes are the length of the block
	_length = readuint32(mrwFile);
	// the next byte is the camera type
	_cameraType = mrwFile.get();
	// the next byte is the saturation
	_saturation = mrwFile.get();
	// the next byte is the contrast
	_contrast = mrwFile.get();
	// the next byte is the sharpness
	_sharpness = mrwFile.get();
	// the next byte is the white balance
	_whiteBalance = mrwFile.get();
	// the next byte is the subject program
	_subjectProgram = mrwFile.get();
	// the next byte is the ISO setting
	_iso = decodeIso(mrwFile.get());
	// the next byte is the color mode
	_colorMode = mrwFile.get();
	// the 56th (+8) byte is the color filter
	mrwFile.seekg(offset + 56 + 8, ifstream::beg);
	_colorFilter = mrwFile.get();
	// the next byte is the B&W filter
	_bwFilter = mrwFile.get();
	// don't know if we read all the values in the block, so seek just in
	// case
	mrwFile.seekg(offset + _length + 8, ifstream::beg);
}

string RifBlock::decodeCameraType() const {
	if (_cameraType >= sizeof(CAMERA_TYPE_STRINGS)) {
		return "Invalid";
	}
	return CAMERA_TYPE_STRINGS[_cameraType];	
}

string RifBlock::decodeSharpness() const {
	if (_sharpness < -1 || _sharpness >= sizeof(SHARPNESS_STRINGS) - 1) {
		return "Invalid";
	}
	return SHARPNESS_STRINGS[_sharpness + 1];
}

string RifBlock::decodeWhiteBalance() const {
	if (_whiteBalance >= sizeof(WHITE_BALANCE_STRINGS)) {
		return "Invalid";
	}
	return WHITE_BALANCE_STRINGS[_whiteBalance];
}

string RifBlock::decodeSubjectProgram() const {
	if (_subjectProgram >= sizeof(SUBJECT_PROGRAM_STRINGS)) {
		return "Invalid";
	}
	return SUBJECT_PROGRAM_STRINGS[_subjectProgram];
}

string RifBlock::decodeColorMode() const {
	if (_colorMode >= sizeof(COLOR_MODE_STRINGS)) {
		return "Invalid";
	}
	return COLOR_MODE_STRINGS[_colorMode];
}

uint16_t RifBlock::decodeIso(uint8_t isoCode) const {
	return (uint16_t)(pow(2.0, ((double)isoCode / 8.0) - 1.0) * 3.125);
}

void RifBlock::dumpRawInfo() const {
	cout << "    RIF block contents:" << endl;
	cout << "      name: '" << _name[0] << "' '" << _name[1] << "' '" <<
			_name[2] << "' '" << _name[3] << "'" << endl;
	cout << "      length: " << _length << endl;
	cout << "      camera type: " << decodeCameraType() << endl;
	cout << "      saturation: " << (_saturation > 0 ? "+" : "") <<
			(int16_t)_saturation << endl;
	cout << "      contrast: " << (_contrast > 0 ? "+" : "") << (int16_t)
			_contrast << endl;
	cout << "      sharpness: " << decodeSharpness() << endl;
	cout << "      white balance: " << decodeWhiteBalance() << endl;
	cout << "      subject program: " << decodeSubjectProgram() << endl;
	cout << "      ISO: " << _iso << endl;
	cout << "      color mode: " << decodeColorMode() << endl;
	cout << "      color filter: " << (uint16_t)_colorFilter << endl;
	cout << "      B&W filter: " << (uint16_t)_bwFilter << endl;
}
