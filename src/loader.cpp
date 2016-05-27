#include "loader.h"

Loader::Loader(){}

Loader::Loader(const std::string file){
	in.open(file, std::fstream::in | std::fstream::binary);
	if (!in.good()){
		perror("open");
		exit(1);
	}
}

Loader::~Loader(){
	in.close();
}

void Loader::setFile(const std::string file){
	in.open(file, std::fstream::in | std::fstream::binary);
}

void Loader::createNodes(Node** array){
	*array = new Node[256];

	for(int i = 0; i < 256; i++){
		(*array)[i].setCode((unsigned char)i);
	}

	Symbol s;
	s.num = 0;

	while(in.get(s.byte[0])){
		(*array)[s.num].increasePriority();
	}

	return;
	for(int i = 0; i < 256; i++){
		std::cout << (*array)[i].getCode() << '\t' << (*array)[i].getPriority() << std::endl;
	}
}