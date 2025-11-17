#ifndef __MPU6050__H__
#define __MPU6050__H__

void MPU6050_init();
void MPU6050_write_reg(uint8_t reg_addr, uint8_t data);
uint8_t MPU6050_read_reg(uint8_t reg_addr);
uint8_t MPU6050_get_id();
void MPU6050_get_data(int16_t *AccX, int16_t *AccY, int16_t *AccZ, int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ);

#endif