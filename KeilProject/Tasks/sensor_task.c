#include "sensor_task.h"
#include "imu.h"
#include "FreeRTOS.h"
#include "task.h"
#include "sensor_data.h"
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

// Global shared sensor data
extern MotionData_t gMotionData;

float RejectSpike(float newVal, float prevVal, float maxDelta) {
    if (fabsf(newVal - prevVal) > maxDelta) {
        return prevVal;  // reject sudden jump
    }
    return newVal;
}


// === EMA Filter Definition ===
typedef struct {
    float value;
    bool initialized;
    float alpha; // Smoothing factor (0 < alpha < 1)
} EMAFilter;

void EMA_Init(EMAFilter* f, float alpha) {
    f->value = 0.0f;
    f->initialized = false;
    f->alpha = alpha;
}

float EMA_Apply(EMAFilter* f, float input) {
    if (!f->initialized) {
        f->value = input;
        f->initialized = true;
    } else {
        f->value = f->alpha * input + (1.0f - f->alpha) * f->value;
    }
    return f->value;
}


// Filters for raw accel and gyro data
static EMAFilter axFilter, ayFilter, azFilter;
static EMAFilter gxFilter, gyFilter, gzFilter;

void SensorTask(void *pvParameters) {
    (void)pvParameters;

    float alpha = 0.2f;
    EMA_Init(&axFilter, alpha); EMA_Init(&ayFilter, alpha); EMA_Init(&azFilter, alpha);
    EMA_Init(&gxFilter, alpha); EMA_Init(&gyFilter, alpha); EMA_Init(&gzFilter, alpha);

    while (1) {
        // Read raw sensor values
        int16_t rawAx = read16(0x28);
        int16_t rawAy = read16(0x2A);
        int16_t rawAz = read16(0x2C);
        int16_t rawGx = read16(0x22);
        int16_t rawGy = read16(0x24);
        int16_t rawGz = read16(0x26);
			 
        // Apply EMA filtering
        float ax = EMA_Apply(&axFilter, (float)rawAx);
        float ay = EMA_Apply(&ayFilter, (float)rawAy);
        float az = EMA_Apply(&azFilter, (float)rawAz);
        float gx = EMA_Apply(&gxFilter, (float)rawGx);
        float gy = EMA_Apply(&gyFilter, (float)rawGy);
        float gz = EMA_Apply(&gzFilter, (float)rawGz);

        // Write to shared data (mutex protected)
        if (xSemaphoreTake(xMotionDataMutex, portMAX_DELAY)) {
            gMotionData.accelX = rawAx;
            gMotionData.accelY = rawAy;
            gMotionData.accelZ = rawAz;
            gMotionData.gyroX = gx;
            gMotionData.gyroY = gy;
            gMotionData.gyroZ = gz;
            xSemaphoreGive(xMotionDataMutex);
        }

        vTaskDelay(pdMS_TO_TICKS(10)); // ~100 Hz
    }
}
