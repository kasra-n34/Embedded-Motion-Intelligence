#ifndef SENSOR_TASK_H
#define SENSOR_TASK_H

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"

void SensorTask(void *pvParameters);
float RejectSpike(float newVal, float prevVal, float maxDelta);

#endif // SENSOR_TASK_H
