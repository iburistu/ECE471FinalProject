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
#include <time.h>
#include <inttypes.h>

// Function to write to a GPIO
int GPIOWrite(int pin, int value)
{
    char string_buffer[34];
    char buffer[3];
    ssize_t bytes_written;
    int fd;

    // generate path string for open()
    sprintf(string_buffer, "/sys/class/gpio/gpio%d/value",pin);

    // open the direction file for writing and print to standard error if unable to open
    fd = open(string_buffer, O_WRONLY);
    if (-1 == fd) {
	fprintf(stderr, "Failed to open GPIO%d\n",pin);
	return(-1);
    }

    // write value to pin
    bytes_written = snprintf(buffer, 2, "%d", value);
    write(fd, buffer, bytes_written);
    close(fd);
    return(0);
}

// Function to read value from pin
int GPIORead(int pin)
{
    char string_buffer[34];
    char buffer[3];
    int fd;

    // generate path string for open()
    sprintf(string_buffer, "/sys/class/gpio/gpio%d/value", pin);

    // open the direction file for reading and print to standard error if unable to open
    fd = open(string_buffer, O_RDONLY);
    if (-1 == fd) {
	fprintf(stderr, "Failed to open GPIO%d\n",pin);
	return(-1);
    }

    // read the file on error print to standard error
    if (read(fd, buffer, 3) == -1) {
	fprintf(stderr, "Failed to read from GPIO%d\n",pin);
	return(-1);
    }

    // close and return the read contents
    close(fd);
    return(atoi(buffer));
}


// Initializes the ultrasonic sensor and finds the baseline distance
double initUltrasonic(int trigger, int echo) {
    char buffer[6];
    ssize_t bytes_written;
    int fd;

    // open the export file for writing and print to standard error if unable to open
    // setting GPIO_TRIGGER
    fd = open("/sys/class/gpio/export", O_WRONLY);
    if (fd == -1) {
        fprintf(stderr, "Failed to open export file for writing!\n");
	    return(-1.0);
    }

    bytes_written = snprintf(buffer, 3, "%d", trigger);
    write(fd, buffer, bytes_written);
    close(fd);

    // setting GPIO_ECHO
    fd = open("/sys/class/gpio/export", O_WRONLY);
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
    double timediff;
    double timediff_temp = 0.0;
    int continous = 0;
    struct timespec start, end;
//    volatile int busywait = 0;
    while(1) {
        // send out a single 'chirp' through the trigger pin
        GPIOWrite(GPIO_TRIGGER,1);

        // sleep 0.01 ms, or 10 us
        usleep(10);

        // turn off the chirp
        GPIOWrite(GPIO_TRIGGER,0);

        // save the start time 
        clock_gettime(CLOCK_MONOTONIC, &start);

        // save the response time
        while (!GPIORead(GPIO_ECHO)) {}
        
        while (GPIORead(GPIO_ECHO)) {
            clock_gettime(CLOCK_MONOTONIC, &end);            
        }

        // save the difference in times
        timediff = end.tv_nsec - start.tv_nsec;
        printf("%lf\n", timediff);

        // if the time stays relatively constant (+/- 5%), return the time
        if ((timediff_temp * 1.05 > timediff) && (timediff_temp * .95 < timediff)) {
            continous++;
        }
        else {
            continous = 0;
            timediff_temp = timediff;
            sleep(1);
        }

        if (continous == 5) {
            return timediff;
        }
    }
}



// Disposes of the pins being used by the ultrasonic sensor
void disposeUltrasonic(int trigger, int echo) {
    char buffer[3];
    ssize_t bytes_written;
    int fd;

    // open the unexport file for writing and print to standard error if unable to open
    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (-1 == fd) {
	fprintf(stderr, "Failed to open unexport file for writing!\n");
        exit(0);
    }
    // write the pin number to unexport
    bytes_written = snprintf(buffer, 3, "%d", trigger);
    write(fd, buffer, bytes_written);
    close(fd);    

    // open the unexport file for writing and print to standard error if unable to open
    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (-1 == fd) {
	fprintf(stderr, "Failed to open unexport file for writing!\n");
        exit(0);
    }
    // write the pin number to unexport
    bytes_written = snprintf(buffer, 3, "%d", echo);
    write(fd, buffer, bytes_written);
    close(fd);
}

double getTime(void) {
    double timediff;
    struct timespec start, end;

    // send out a single 'chirp' through the trigger pin
    GPIOWrite(GPIO_TRIGGER,1);

    // sleep 0.01 ms, or 10 us
    usleep(10);

    // turn off the chirp
    GPIOWrite(GPIO_TRIGGER,0);

    // save the start time 
    clock_gettime(CLOCK_MONOTONIC, &start);

    // save the response time
    while (!GPIORead(GPIO_ECHO)) {}
        
    while (GPIORead(GPIO_ECHO)) {
        clock_gettime(CLOCK_MONOTONIC, &end);            
    }

    timediff = end.tv_nsec - start.tv_nsec;

    return timediff;
}

#endif