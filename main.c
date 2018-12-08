#include "ultrasonic.h"

int main() {
    double timediff = 0.0;
    double baseline = initUltrasonic(GPIO_TRIGGER, GPIO_ECHO);
    int personcount = 0;
    
    // infinte polling loop
    while(1) {
        timediff = getTime();

        // trigger if out of baseline by 5%
        if (!((timediff * 1.05 > baseline) && (timediff * 0.95 < baseline))) {
            // sleep to avoid false positives
            usleep(1000);
            // if still out of bounds, increment personcount
            if (!((timediff * 1.05 > baseline) && (timediff * 0.95 < baseline))) {
                personcount++;
                // 
                while(!((timediff * 1.05 > baseline) && (timediff * 0.95 < baseline))) {timediff = getTime();}
            }
        }
    }
}