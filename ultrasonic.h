#ifndef ultrasonic_h
#define ultrasonic_h

#define GPIO_TRIGGER 18
#define GPIO_ECHO 24

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

// Initializes the ultrasonic sensor and finds the baseline distance
double initUltrasonic(int trigger, int echo) {
    char buffer[3];
    ssize_t bytes_written;
    int fd;

    // open the export file for writing and print to standard error if unable to open
    fd = open("/sys/class/gpio/export", 0_WRONLY);
    if (fd == -1) {
        
    }

}

// Disposes of the pins being used by the ultrasonic sensor
void disposeUltrasonic(int trigger, int echo);



#endif