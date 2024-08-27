#ifndef __MPU6050_H
#define __MPU6050_H

uint8_t MPU6050_Write(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf);
uint8_t  MPU6050_Read(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf);
void MPU6050_Write_Reg(uint8_t reg,uint8_t dat);
uint8_t MPU6050_Read_Reg(uint8_t reg);
uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr);
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr);
uint8_t MPU_Set_LPF(uint16_t lpf);
uint8_t MPU_Set_Rate(uint16_t rate);
void MPU6050_Init(void);

#endif
