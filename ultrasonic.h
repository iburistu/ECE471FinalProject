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
    char buffer[6];
    ssize_t bytes_written;
    int fd;

    // open the export file for writing and print to standard error if unable to open
    // setting GPIO_TRIGGER
    fd = open("/sys/class/gpio/export", 0_WRONLY);
    if (fd == -1) {
        fprintf(stderr, "Failed to open export file for writing!\n");
	    return(-1.0);
    }

    bytes_written = snprintf(buffer, 3, "%d", trigger);
    write(fd, buffer, bytes_written);
    close(fd);

    // setting GPIO_ECHO
    fd = open("/sys/class/gpio/export", 0_WRONLY);
    if (fd == -1) {
        fprintf(stderr, "Failed to open export file for writing!\n");
	    return(-1.0);
    }

    bytes_written = snprintf(buffer, 3, "%d", echo);
    write(fd, buffer, bytes_written);
    close(fd);

    /* set GPIO_TRIGGER direction to output */
    
    // generate path string for open()
    char string_buffer[34];

    sprintf(string_buffer, "/sys/class/gpio/gpio%d/direction", trigger);

    // open the direction file for writing and print to standard error if unable to open
    fd = open(string_buffer, O_WRONLY);
    if (-1 == fd) {
	fprintf(stderr, "Failed to open direction file for writing!\n");
	return(-1);
    }
    
    // write then close
    bytes_written = snprintf(buffer, 5, "%s\n", "out");
    write(fd, buffer, bytes_written);
    close(fd);

    /* set GPIO_ECHO direction to input */

    // generate path string for open()
    sprintf(string_buffer, "/sys/class/gpio/gpio%d/direction", echo);

    // open the direction file for writing and print to standard error if unable to open
    fd = open(string_buffer, O_WRONLY);
    if (-1 == fd) {
	fprintf(stderr, "Failed to open direction file for writing!\n");
	return(-1);
    }
    
    // write then close
    bytes_written = snprintf(buffer, 4, "%s\n", "in");
    write(fd, buffer, bytes_written);
    close(fd);

    // continue infinitely until a distance has been read and verified
    double distance;
    while(1) {


        // send out a single 'chirp' through the trigger pin

        // sleep 0.01 ms, or 10 us

        // save the 

    }
}

// Disposes of the pins being used by the ultrasonic sensor
void disposeUltrasonic(int trigger, int echo);



#endif