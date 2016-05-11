#ifndef LOADER_H
#define LOADER_H

#include <string>
#include <fstream>
#include <iostream>
#include "node.h"

typedef union{
	char byte[4];
	int  num;
} Symbol;

class Loader{
public:
	Loader();
	Loader(std::string file);
	~Loader();

	void setFile(std::string file);
	void createNodes(Node** array);
private:
	std::ifstream in;
};

#endif // LOADER_H