#ifndef ML_TASK_H
#define ML_TASK_H

#include <stdint.h>

void MLTask(void *pvParameters);

// Optionally, expose inference result function or state
int run_inference(float ax, float ay, float az, float gx, float gy, float gz);

#endif // ML_TASK_H
