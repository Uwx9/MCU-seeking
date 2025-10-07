#include <REGX52.H>
#include "key.h"
#include "interrupt.h"
#include "delay.h"
#include "nixie.h"
#include "stepmotor.h"

extern int percent;
unsigned char key_num = 0, speed = 0;

void main(void)
{
	stepmotor_open();
	while (1) {			
		key_num = Key();
		if (key_num == 1) {
			speed++;
			speed %= 4;
			if (speed == 0) percent = 0;
			else if (speed == 1) percent = 60;
			else if (speed == 2) percent = 80;
			else if (speed == 3) percent = 100;
		}
		Nixie(1, speed);
	}
}

