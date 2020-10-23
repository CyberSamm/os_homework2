#include<unistd.h>
#include<fcntl.h>
#include<iostream>
#include<sys/types.h>


int main(int argc, char* argv[]){
	if(argc < 2){
		std::cout << "WRONG ANSWER";
		return -1;
	}
	int file = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP); 

	long size = 3000000;

	char* text = new char[size];
	
	for(int i = 0; i < size; ++i){
		text[i] = 's';
	}
	
	write(file, text, size);
	lseek(file, 2000000, SEEK_END);
	write(file, text, size);
	lseek(file, 3000000, SEEK_END);
	write(file, text, size);
	lseek(file, 2000000, SEEK_END);

	delete text;
	close(file);

	return 0;
}
