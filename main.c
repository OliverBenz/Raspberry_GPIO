/*
	Reference: https://elinux.org/RPi_GPIO_Code_Samples
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>	//mmap()
#include <unistd.h>

// GPIO setup macros
// Always use INP_GPIO before using OUT_GPIO or SET_GPIO_ALT
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))

#define BLOCK_SIZE (4*1024)

volatile unsigned *gpio;

// Functions
void setup_io();

int main(int argc, char *argv[]){
	setup_io();

	// Set GPIO 20 to output
	INP_GPIO(20);
	OUT_GPIO(20);

	while(1){

	}

	return 0;
}

// Setup memory regions for GPIO access
void setup_io(){
	// Open /dev/mem for mmap()
	if((memfile = open("/dev/mem", 0_RDWR|0_SYNC)) < 0){
		printf("Could not open /dev/mem \n");
		exit(EXIT_FAILURE);
	}

	gpio_map = mmap(
		NULL,					// Any address in space
		BLOCK_SIZE,				// Map length
		PROT_READ|PROT_WRITE,	// Enable reading and writing
		MAP_SHARED,				// Shared with other processes
		memfile,				// Mapped file
		GPIO_BASE				// Offset to GPIO peripheral
	);

	close(memfile);

	// Check if map successful
	if(gpio_map == MAP_FAILED){
		printf("mmap error %d\n", (int)gpio_map);
		exit(EXIT_FAILURE);
	}

	gpio = (volatile unsigned *)gpio_map;
}
