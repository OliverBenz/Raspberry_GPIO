/*
	Reference: https://elinux.org/RPi_GPIO_Code_Samples
*/
#define BCM2708_PERI_BASE        0x20000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>	//mmap()
#include <unistd.h>

#define BLOCK_SIZE (4*1024)

volatile unsigned *gpio;

// GPIO setup macros
// Always use INP_GPIO before using OUT_GPIO or SET_GPIO_ALT
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)				// sets	  bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10)				// clears bits which are 1 ignores bits which are 0

#define GET_GPIO(g) (*(gpio+13)&(1<<g))	// 0 if LOW, (1<<g) if HIGH

#define GPIO_PULL *(gpio+37)			// Pull up/pull down
#define GPIO_PULLCLK0 *(gpio+38)		// Pull up/pull down clock

// Functions
void setup_io();

int main(int argc, char *argv[]){
	setup_io();

	// Set GPIO 20 to output
	INP_GPIO(20);
	OUT_GPIO(20);

	while(1){
		GPIO_SET = 1<<20;
		sleep(5);
		GPIO_CLR = 1<<20;
		sleep(5);
	}

	return 0;
}

// Setup memory regions for GPIO access
void setup_io(){
	int memfile = 0;
	void *gpio_map = NULL;

	// Open /dev/mem for mmap()
	if((memfile = open("/dev/mem", O_RDWR|O_SYNC)) < 0){
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
