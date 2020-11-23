#include "stdlib.h"
#include "stdio.h"
#include "pigpio.h"

int main(){
    if(gpioInitialise() == PI_INIT_FAILED){
        printf("Could not initialize library.\n");
        return EXIT_FAILURE;
    }

    // Source code
    gpioTerminate();

    return 0;
}