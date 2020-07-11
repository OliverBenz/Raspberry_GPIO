#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>	//mmap()
#include <unistd.h>

volatile unsigned *gpio;

// Functions
void setup_io();

int main(){
	return 0;
}

// Setup memory regions for GPIO access
void setup_io(){
	if((memfile = open("/dev/mem", 0_RDWR|0_SYNC)) < 0){
		printf("Could not open /dev/mem \n");
		exit(EXIT_FAILURE);
	}

	gpio_map = mmap(
		NULL,
		BLOCK_SIZE,
		PROT_READ|PROT_WRITE,
		MAP_SHARED,
		memfile,
		GPIO_BASE
	);

	close(memfile);

	if(gpio_map == MAP_FAILED){
		printf("mmap error %d\n", (int)gpio_map);
		exit(EXIT_FAILURE);
	}

	gpio = (volatile unsigned *)gpio_map;
}
