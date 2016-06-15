#ifndef BIT_STREAM_H
#define BIT_STREAM_H

#include <fstream>
#include <iostream>
#include <bitset>
typedef unsigned char Byte;

class BitStream{
public:
	BitStream(std::string filename, int r);
	~BitStream();

	int readBit();
	void readBits(Byte *bits, int size);
	int readByte(Byte *tmp);
	int readUShort(unsigned short *tmp);
	int readUInt(unsigned int *tmp);
	int readULong(unsigned long *tmp);

	void writeBit(Byte bit);
	void writeBits(Byte *bits, int size);
	void writeByte(Byte data);
	void writeUShort(unsigned short data);
	void writeUInt(unsigned int data);
	void writeULong(unsigned long data);

	int size();
private:
	std::fstream file;
	Byte buffer;
	Byte mask;
	int read;
};

#endif