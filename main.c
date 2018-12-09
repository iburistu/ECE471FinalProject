#include "ultrasonic.h"
#include "displayPeople.h"

int main() {
    int dist_diff = 0;
    int baseline = initUltrasonic(GPIO_TRIGGER, GPIO_ECHO);
    int personcount = 0;
    int continous = 0, continous2 = 0;
    
	if(LCDinit() == -1){
		printf("Error in LCDinit\n");
		return -1;
	}

    // infinte polling loop
    while(1) {
        dist_diff = getDist();
        // trigger if out of baseline by 5%
        if (dist_diff < baseline * 9 / 10) {
            continous++;
        }
        else {
            continous = 0;
        }
        if (continous > 5) {
            personcount++;
			if(displayNumPeople(personcount) == -1){
					printf("error is displayNumPeople\n");
					return -1;
			}
            continous = 0;
            // wait until reset 
            while(dist_diff < baseline * 9 / 10 && continous2 < 5) {
                dist_diff = getTime();
                if (dist_diff < baseline * 9 / 10) {
                    continous2++;
                }
                else {
                    continous2 = 0;
                }
            }
        }
    }

}
