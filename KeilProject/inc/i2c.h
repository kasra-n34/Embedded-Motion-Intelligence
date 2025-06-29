#ifndef I2C_H
#define I2C_H

#include <stdint.h>

void I2C0_Init(void);
uint8_t I2C0_ReadRegister(uint8_t slaveAddr, uint8_t regAddr);
void I2C0_WriteRegister(uint8_t slaveAddr, uint8_t regAddr, uint8_t data);
void I2C_ScanBus(void);

#endif
