#ifndef __CAR_H__
#define __CAR_H__

enum CAR_SPEED {
	CAR_SPEED_0,	// %0
	CAR_SPEED_1,	// %60
	CAR_SPEED_2,  	// %80
	CAR_SPEED_3		// %100
};

void car_init();
void car_stop();
void car_move_forward();
void car_move_backward();
void car_left_reversal();
void car_right_reversal();
void car_speed_up();
void car_speed_down();
void car_show_status();

#endif