#include<iostream>
#include<fcntl.h>
#include<sys/types.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>

#include<string>
#include"for_sizes.h"

void display_sizes(int file, std::string filename){
	
	// display Local Size
	long long logicalSize = lseek(file, 0, SEEK_END);
	std::cout << "Local size of " << filename << " file = " << logicalSize << "\n";
	
	// pointer in file
	long long cursor = 0;
	
	long long physicalSize = 0;

	// calculate physical size
	while(true){
		// lenght from the start of file to the DATA section after cursor
		long long startofDATA = lseek(file, cursor, SEEK_DATA);
		
		// if cursor == end of file lseek return -1
		// this means that we have reached the end of the file
		if(startofDATA == -1){
			if(errno == ENXIO){
				break;
			}
		}
		// lenght from the start of file to the HOLE section after cursor
		long long endofDATA = lseek(file, startofDATA, SEEK_HOLE);
		
		if(endofDATA == -1){
			if(errno == ENXIO){
				break;
			}
		}		
				
		// increase physicalSize
		long long sizeofDATA = endofDATA - startofDATA;
		physicalSize += sizeofDATA;
		
		// update cursor value
		cursor = endofDATA;
		
		// this means that we have reached the end of the file
		if(cursor == logicalSize){
			break;
		} 
	}
	
	// display Physical Size
	std::cout << "Physical size of " << filename << " file = " << physicalSize << "\n\n";
	
}
