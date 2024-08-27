#ifndef __I2CM_H
#define __I2CM_H

void I2CM_W_SCL(uint8_t BitVal);
void I2CM_W_SDA(uint8_t BitVal);
uint8_t I2CM_R_SDA(void);
void I2CM_Init(void);
void I2CM_Start(void);
void I2CM_Stop(void);
uint8_t I2CM_ReceiveAck(void);
void I2CM_SendAck(uint8_t AckBit);
void I2CM_SendByte(uint8_t byte);
uint8_t I2CM_ReceiveByte(uint8_t Ack);


#endif
