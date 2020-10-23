all: myCopy

myCopy: mycopy.o for_sizes.o
	g++ mycopy.o for_sizes.o -o myCopy

mycopy.o: mycopy.cpp
	g++ -c mycopy.cpp

for_sizes.o: for_sizes.cpp	
	g++ -c for_sizes.cpp
	
clean:
	rm -rf *.o myCopy 
