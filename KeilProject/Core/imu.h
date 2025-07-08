#ifndef IMU_H
#define IMU_H

#include <stdint.h>

#define LSM6DSO_ADDR 0x6A

int16_t read16(uint8_t baseAddr);
void IMU_Init(void);

#endif // IMU_H
