#include "ultrasonic.h"
#include "displayPeople.h"

int main() {
    double timediff = 0.0;
    double baseline = initUltrasonic(GPIO_TRIGGER, GPIO_ECHO);
    int personcount = 0;
    
	if(LCDinit() == -1){
		printf("Error in LCDinit\n");
		return -1;
	}

    // infinte polling loop
    while(1) {
        timediff = getTime();

        // trigger if out of baseline by 5%
        if (timediff < .8 * baseline) {
            // sleep to avoid false positives
            usleep(1000);
            // if still out of bounds, increment personcount
            if (timediff < .8 * baseline) {
                personcount++;
				if(displayNumPeople(personcount) == -1){
					printf("error is displayNumPeople\n");
					return -1;
				}
                // wait until reset 
                while(timediff < .8 * baseline) {timediff = getTime();}
            }
        }
	}
}
