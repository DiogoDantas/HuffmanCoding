#include "bitstream.h"
#define complete_1 do {while(mask){ buffer |= mask; mask >>= 1; }}while(0)

BitStream::BitStream(std::string filename, int r):
	file(),
	buffer(0),
	mask(0x80),
	read(r)
{
	if(r) file.open(filename, std::fstream::in | std::fstream::binary);
	else file.open(filename, std::fstream::out | std::fstream::binary);

	if (!file.good()){
		perror("Erro ao abrir arquivo ");
		perror(filename.c_str());
		exit(1);
	}
}

BitStream::~BitStream(){
	if(mask != 0x80 && !read){
		complete_1;
		file.write((char*)&buffer, sizeof(Byte));
	}

	file.close();
}

int BitStream::size(){
	file.seekg(0, file.end);
	int length = file.tellg();
	file.seekg (0, file.beg);

	return length;
}

void BitStream::reset(){
	file.clear();
	file.seekg(0, file.beg);
}

int BitStream::eof() const{
	return file.eof();
}

int BitStream::readBit(){
	if(mask == 0x80)
		file.read((char*)&buffer, sizeof(Byte));

	int tmp = mask & buffer ? 1 : 0;
	//std::cerr << tmp;
	mask >>= 1;
	if(!mask) mask = 0x80;
	return tmp;
}

void BitStream::readBits(Byte *bits, int size){
	int lenght = (size-1)/(8*sizeof(Byte));
	Byte localMask = 0x80 >> (8 - (size-1)%(8*sizeof(Byte)) - 1);

	for(int i = lenght; i >=0; i--){
		bits[i] = 0x00;
		while(localMask){
			bits[i] |= readBit() ? localMask : 0x00;
			localMask >>= 1;
		}
	}
}

int BitStream::readByte(Byte *tmp){
	file.read((char*)tmp, sizeof(Byte));
	return file.eof();
}

int BitStream::readUShort(unsigned short *tmp){
	file.read((char*)tmp, sizeof(unsigned short));
	return file.eof();
}

int BitStream::readUInt(unsigned int *tmp){
	file.read((char*)tmp, sizeof(unsigned int));
	return file.eof();
}

int BitStream::readULong(unsigned long *tmp){
	file.read((char*)tmp, sizeof(unsigned long));
	return file.eof();
}

void BitStream::writeBit(Byte bit){
	if(bit)
		buffer |= mask;

	mask >>= 1;
	if(!mask){
		file.write((char*)&buffer, sizeof(Byte));
		mask = 0x80;
		buffer = 0x00;
	}
}

void BitStream::writeBits(Byte *bits, int size){
	int lenght = (size-1)/(8*sizeof(Byte));
	Byte localMask = 0x80 >> (8 - (size-1)%(8*sizeof(Byte)) - 1);

	for(int i = lenght; i >= 0; i--){
		while(localMask){
			writeBit(localMask & bits[i]);
			localMask >>= 1;
		}
		localMask = 0x80;
	}
}

void BitStream::writeByte(Byte data){
	file.write((char*)&data, sizeof(Byte));
}

void BitStream::writeUShort(unsigned short data){
	file.write((char*)&data, sizeof(unsigned short));
}

void BitStream::writeUInt(unsigned int data){
	file.write((char*)&data, sizeof(unsigned int));
}

void BitStream::writeULong(unsigned long data){
	file.write((char*)&data, sizeof(unsigned long));
}