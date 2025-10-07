#include "REGX52.H"
#include "INTRINS.H"
#include "IR.h"
#include "stepmotor.h"
#include "stdint.h"
#include "nixie.h"

extern int percent;
extern uint8_t IR_command;
int speed = 0;

void main()
{
	IR_init();
	stepmotor_open();
	while (1) {
		if (IR_command == 0x0c) speed = 1;
		else if (IR_command == 0x18) speed = 2;
		else if (IR_command == 0x5e) speed = 3;
		else speed = 0;

		if (speed == 0) {
			Nixie(0, 0);
			percent = 0;
		} 
		else if (speed == 1) {
			Nixie(0, 1);
			percent = 60;
		}
		else if (speed == 2) {
			Nixie(0, 2);
			percent = 80;
		}
		else if (speed == 3){
			Nixie(0, 3);
			percent = 100;
		}
	}
}