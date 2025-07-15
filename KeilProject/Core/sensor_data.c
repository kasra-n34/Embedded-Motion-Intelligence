#include "sensor_data.h"

// Global shared sensor data
MotionData_t gMotionData;

// ? Define the mutex (actual storage allocated here)
SemaphoreHandle_t xMotionDataMutex = NULL;
