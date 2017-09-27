#include <fstream>
#include <netinet/in.h>
#include <stdint.h>

using namespace std;

uint16_t readuint16(ifstream& stream) {
	char buffer[2];
	stream.read(buffer, 2);
	return ((unsigned char)buffer[0] << 8) + ((unsigned char)buffer[1]);
}

uint32_t readuint32(ifstream& stream) {
	char buffer[4];
	stream.read(buffer, 4);
	return ((unsigned char)buffer[0] << 24) + ((unsigned char)buffer[1]
			<< 16) + ((unsigned char)buffer[2] << 8) + ((unsigned
			char)buffer[3]);
}
