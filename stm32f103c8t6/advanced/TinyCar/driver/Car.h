#ifndef __CAR_H__
#define __CAR_H__

enum CAR_SPEED {
	CAR_SPEED_0,	// %0
	CAR_SPEED_1,	// %60
	CAR_SPEED_2,  	// %80
	CAR_SPEED_3		// %100
};

void car_init();
void car_reset_speed(enum CAR_SPEED _CarSpeed);
void car_turnleft();
void car_stop_trunleft();
void car_turn_right();
void car_stop_trunleft();


#endif