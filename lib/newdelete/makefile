lib: newdelete.o
	avr-ar rcs newdelete.a newdelete.o
	@rm newdelete.o

newdelete.o: src/newdelete.cpp
	avr-g++ -g -Os -mmcu=atmega328p -Iinc $^ -c