#include "ultrasonic.h"
#include "displayPeople.h"

int main() {
		int dist_diff = 0;
		int baseline = initUltrasonic(GPIO_TRIGGER, GPIO_ECHO);
		int personcount = 0;
		int continous = 0, continous2 = 0;
		double Shit2Print;

		if(LCDinit() == -1){
				printf("Error in LCDinit\n");
				return -1;
		}

		printf("Baseline is set at %d\n", baseline);
		printf(".9 Baseline is set at %d\n", baseline * 9 / 10);

		displayNumPeople(&personcount);

		//				scanf("%d",&Shit2Print);
		//				printf("sdkhfk\t%d\n",Shit2Print);
		//				Shit2Print = (double)getDist();
		//				displayNumPeople( (int)Shit2Print);

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
						printf("%d\n",personcount);

						displayNumPeople(&personcount);

						continous = 0;
						// wait until reset 
						while(dist_diff < baseline * 9 / 10 && continous2 < 35) {
								dist_diff = getDist();
								if (dist_diff > baseline * 9 / 10) {
										continous2++;
								}
								else {
										continous2 = 0;
								}
						}
				}
		}
}
