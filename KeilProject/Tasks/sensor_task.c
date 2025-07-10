#include "sensor_task.h"
#include "imu.h"
#include "FreeRTOS.h"
#include "task.h"
#include "sensor_data.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// Global shared sensor data
MotionData_t gMotionData;

#define FILTER_WINDOW_SIZE 10

typedef struct {
    float buffer[FILTER_WINDOW_SIZE];
    uint8_t index;
    bool filled;
} MovingAverageFilter;

// Filters for raw accel and gyro data
static MovingAverageFilter axFilter, ayFilter, azFilter;
static MovingAverageFilter gxFilter, gyFilter, gzFilter;

void MA_Init(MovingAverageFilter* f) {
    memset(f->buffer, 0, sizeof(f->buffer));
    f->index = 0;
    f->filled = false;
}

float MA_Apply(MovingAverageFilter* f, float newValue) {
    f->buffer[f->index] = newValue;
    f->index = (f->index + 1) % FILTER_WINDOW_SIZE;
    if (f->index == 0) f->filled = true;

    uint8_t count = f->filled ? FILTER_WINDOW_SIZE : f->index;
    if (count == 0) return newValue;  // safety

    float sum = 0;
    for (uint8_t i = 0; i < count; i++) {
        sum += f->buffer[i];
    }
    return sum / count;
}

void SensorTask(void *pvParameters) {
    (void)pvParameters;

    // Initialize filters
    MA_Init(&axFilter); MA_Init(&ayFilter); MA_Init(&azFilter);
    MA_Init(&gxFilter); MA_Init(&gyFilter); MA_Init(&gzFilter);

    while (1) {
        // Read raw sensor values
        int16_t rawAx = read16(0x28);
        int16_t rawAy = read16(0x2A);
        int16_t rawAz = read16(0x2C);

        int16_t rawGx = read16(0x22);
        int16_t rawGy = read16(0x24);
        int16_t rawGz = read16(0x26);

        // Apply moving average filter to raw values and store
        gMotionData.accelX = MA_Apply(&axFilter, (float)rawAx);
        gMotionData.accelY = MA_Apply(&ayFilter, (float)rawAy);
        gMotionData.accelZ = MA_Apply(&azFilter, (float)rawAz);

        gMotionData.gyroX  = MA_Apply(&gxFilter, (float)rawGx);
        gMotionData.gyroY  = MA_Apply(&gyFilter, (float)rawGy);
        gMotionData.gyroZ  = MA_Apply(&gzFilter, (float)rawGz);

        vTaskDelay(pdMS_TO_TICKS(10)); // 10 Hz
    }
}
