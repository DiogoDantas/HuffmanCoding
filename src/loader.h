#ifndef LOADER_H
#define LOADER_H

#include <string>
#include <fstream>
#include <iostream>
#include <cerrno>
#include "node.h"

typedef union{
	char byte[4];
	unsigned int  num;
} Symbol;

class Loader{
public:
	Loader();
	Loader(const std::string file);
	~Loader();

	void setFile(const std::string file);
	void createNodes(Node** array);
private:
	std::ifstream in;
};

#endif // LOADER_H