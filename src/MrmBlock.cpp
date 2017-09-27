#include "MrmBlock.h"

#include <iostream>

#include "typereader.h"

MrmBlock::MrmBlock(ifstream& mrwFile) : _prdBlock(NULL), _ttwBlock(NULL),
		_wbgBlock(NULL), _rifBlock(NULL), _padBlock(NULL) {
	mrwFile.seekg(0, ifstream::beg);
	// the first four bytes are the four bytes of the block name
	_name[0] = mrwFile.get();
	_name[1] = mrwFile.get();
	_name[2] = mrwFile.get();
	_name[3] = mrwFile.get();
	// the next four bytes are the length of the remaining blocks
	_length = readuint32(mrwFile);

	_prdBlock = new PrdBlock(mrwFile, mrwFile.tellg());
	_ttwBlock = new TtwBlock(mrwFile, mrwFile.tellg());
	_wbgBlock = new WbgBlock(mrwFile, mrwFile.tellg());
	_rifBlock = new RifBlock(mrwFile, mrwFile.tellg());
	_padBlock = new PadBlock(mrwFile, mrwFile.tellg());
}

void MrmBlock::dumpRawInfo() const {
	cout << "  MRM block contents:" << endl;
	cout << "    name: '" << _name[0] << "' '" << _name[1] << "' '" <<
			_name[2] << "' '" << _name[3] << "'" << endl;
	cout << "    length: " << _length << endl;
	if (_prdBlock == NULL) {
		cout << "    No PRD block" << endl;
	} else {
		_prdBlock->dumpRawInfo();
	}
	if (_ttwBlock == NULL) {
		cout << "    No TTW block" << endl;
	} else {
		_ttwBlock->dumpRawInfo();
	}
	if (_wbgBlock == NULL) {
		cout << "    No WBG block" << endl;
	} else {
		_wbgBlock->dumpRawInfo();
	}
	if (_rifBlock == NULL) {
		cout << "    No RIF block" << endl;
	} else {
		_rifBlock->dumpRawInfo();
	}
	if (_padBlock == NULL) {
		cout << "    No PAD block" << endl;
	} else {
		_padBlock->dumpRawInfo();
	}
}

MrmBlock::~MrmBlock() {
	delete _prdBlock;
	delete _ttwBlock;
	delete _wbgBlock;
	delete _rifBlock;
	delete _padBlock;
}
