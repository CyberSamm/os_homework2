#include<iostream>
#include<fcntl.h>
#include<sys/types.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>

#include"for_sizes.h"

#define BUFFER_SIZE 16


int main(int argc, char* argv[]){

	if(argc < 3){
		std::cout << "Wrong input. \n";
		exit(1);
	}	
	

	const char* source = argv[1];
	
	// open source file only for read
	int source_fd = open(source, O_RDONLY);
	
	// any errors
	if(source_fd == -1){
		std::cout << "Something went wrong while opening source file. /n";
		std::cout << strerror(errno) << "\n";
		exit(errno);
	}
	
	// display logical size and phisical size	
	display_sizes(source_fd, "source");	
	
	const char* target = argv[2];
	  
	// open target file only for write, create if not target file and truncate this file 
	int target_fd = open(target, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP);
	
	// any errors
	if(target_fd == -1){
		std::cout << "Something went wrong while opening target file. /n";
		std::cout << strerror(errno) << "\n";
		exit(errno);
	}  
	
	// a buffer for coping file 
	char* buf = new char[BUFFER_SIZE + 1];

	long long cursor = 0;
	
	// if source starts with hole
	if(read(source_fd, buf, BUFFER_SIZE) == 0){
		// find hole size
		long long startofDATA = lseek(source_fd, cursor, SEEK_DATA);
 	
 		// create hole in target
		lseek(target_fd, startofDATA, SEEK_END);
	}
		
	while (true){
		
		long long startofDATA = lseek(source_fd, cursor, SEEK_DATA);
		
		if(startofDATA == -1){
			if(errno == ENXIO){
				break;
			}
		}
		
		long long endofDATA = lseek(source_fd, startofDATA, SEEK_HOLE);
		
		if(endofDATA == -1){
			if(errno == ENXIO){
				break;
			}
		}
		
		// update cursor value
		cursor = lseek(source_fd, startofDATA, SEEK_SET);
		
		// size of data section
		long long sizeofDATA = endofDATA - startofDATA; 	
		
		long long readBytes = 0;
		
		while(true){
			
			if(sizeofDATA < BUFFER_SIZE){
				readBytes = read(source_fd, buf, sizeofDATA);
				// something went wrong
				if(readBytes == -1){
					std::cout << "Something went wrong while reading file. /n";
					std::cout << strerror(errno) << "\n";
					exit(errno);	
				}
			}
			else{
				readBytes = read(source_fd, buf, BUFFER_SIZE);
				// something went wrong
				if(readBytes == -1){
					std::cout << "Something went wrong while reading file. /n";
					std::cout << strerror(errno) << "\n";
					exit(errno);	
				}
			}
			
			sizeofDATA -= readBytes;
				
			// stop while
			if(readBytes == 0){
				break;
			}
			
			// write from buf to target
			long long writeBytes = write(target_fd, buf, readBytes);
			
			// something went wrong 
			if(writeBytes == -1){
				std::cout << "Something went wrong while writing file. /n";
				std::cout << strerror(errno) << "\n";
				exit(errno);	
			}
		}
		
		long long startofHOLE = lseek(source_fd, startofDATA, SEEK_HOLE);
		
		if(startofHOLE == -1){
			if(errno == ENXIO){
				break;
			}
		}
		
		long long endofHOLE = lseek(source_fd, startofHOLE, SEEK_DATA);
		
		if(endofHOLE == -1){
			if(errno == ENXIO){
				break;
			}
		}
		
		long long sizeofHOLE = endofHOLE - startofHOLE;
		
		// write hole to target
		lseek(target_fd, sizeofHOLE, SEEK_END);
		
		// update cursor value
		cursor = startofHOLE;
	}
	
	std::cout << "Successful copy file!\n\n";
	
	
	
	display_sizes(target_fd, "target");
		
	// close files
	close(source_fd);
	close(target_fd);
	
	// clearing memory
	delete buf;
	
	return 0;
}
