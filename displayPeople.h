#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

int LCDinit(void);
int displayNumPeople(int* ppl);

int LCDinit(void) {

		int fd;
		int i;

        unsigned char buffer[17];

		/* Open i2c device */

		fd = open ("/dev/i2c-1", O_RDWR);			//open the file
		if (fd < 0){									//error checking 
				printf("error on open\n");
				return -1;
		}

		/* Set slave address */

		int result = ioctl(fd, I2C_SLAVE, 0x70);
		if (result < 0){							//error checking
				printf("error on ioctl\n");
				return -1;
		}

		/* Turn on oscillator */

		buffer[0] = (0x2 << 4) | (0x1);
		result = write(fd, buffer, 1);
		if (result != 1){						//error checking
				printf("Failed to write\n");
				return -1;
		}

		/* Turn on Display, No Blink */

		buffer[0] = 0x81;					//Instruction to turn blink off
		result = write(fd, buffer, 1);
		if (result != 1){                       //error checking
				printf("Failed to turn blink off\n");
				return -1;
		}


		buffer[0] = 0xEA;						//Light level 11
		result = write(fd, buffer, 1);
		if (result != 1){                       //error checking
				printf("Failed to turn blink off\n");
				return -1;
		}

		for (i = 0; i<11; i++){			//resetting everything to zero to avoid floats
				buffer[i] = 0x00;
		}
		
		close(fd);
}

int displayNumPeople(int* ppl){

		int people = *ppl;

	    unsigned char buffer[17];

		int display[12] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x67, 0x63, 0x40};

		int fd;
		int i;

        fd = open ("/dev/i2c-1", O_RDWR);           //open the file
        if (fd < 0){                                    //error checking
                printf("error on open\n");
                return -1;
        }

        /* Set slave address */

        int result = ioctl(fd, I2C_SLAVE, 0x70);
        if (result < 0){                            //error checking
                printf("error on ioctl\n");
                return -1;
        }
        /* Turn on oscillator */

        buffer[0] = (0x2 << 4) | (0x1);
        result = write(fd, buffer, 1);
        if (result != 1){                       //error checking
                printf("Failed to write\n");
                return -1;
        }

        /* Turn on Display, No Blink */

        buffer[0] = 0x81;                   //Instruction to turn blink off
        result = write(fd, buffer, 1);
        if (result != 1){                       //error checking
                printf("Failed to turn blink off\n");
                return -1;
        }


        buffer[0] = 0xEA;                       //Light level 11
        result = write(fd, buffer, 1);
        if (result != 1){                       //error checking
                printf("Failed to turn blink off\n");
                return -1;
        }

        for (i = 0; i<11; i++){         //resetting everything to zero to avoid floats
                buffer[i] = 0x00;
        }


		buffer[9] = display[people % 10];				

		people /= 10;
		if(people != 0){
				buffer[7] = display[people % 10];		//writes the decimal
		}

		people /= 10;
		if(people != 0){
				buffer[3] = display[people % 10];	//0x80 adds the dicimal point
		}

		people /= 10;
		if(people != 0){
				buffer[1] = display[people % 10];
		}

        write(fd,buffer,17);    //rewrite
		close(fd);


		return 0;
}
