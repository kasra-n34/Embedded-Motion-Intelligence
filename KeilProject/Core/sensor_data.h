#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

#include <stdint.h>
#include "FreeRTOS.h"
#include "semphr.h"

 

typedef struct {
    int16_t accelX;
    int16_t accelY;
    int16_t accelZ;
    int16_t gyroX;
    int16_t gyroY;
    int16_t gyroZ;
} MotionData_t;

extern MotionData_t gMotionData;
extern SemaphoreHandle_t xMotionDataMutex;

#endif // SENSOR_DATA_H
