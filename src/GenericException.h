#ifndef _GENERICEXCEPTION_H
#define _GENERICEXCEPTION_H

#include <exception>
#include <string>

using namespace std;

class GenericException : public exception {
public:
	GenericException(string cause) { _cause = cause; }
	GenericException() { _cause = ""; }

	string getCause() { return _cause; }

	virtual ~GenericException() throw () { }

private:
	string _cause;
};

#endif
