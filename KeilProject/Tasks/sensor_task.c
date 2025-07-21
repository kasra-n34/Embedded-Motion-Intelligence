#include "sensor_task.h"
#include "imu.h"
#include "FreeRTOS.h"
#include "task.h"
#include "sensor_data.h"
#include <stdint.h>
#include <stdbool.h>

// Global shared sensor data
extern MotionData_t gMotionData;

void SensorTask(void *pvParameters) {
    (void)pvParameters;

    while (1) {
        // Read raw sensor values directly (16-bit signed)
        int16_t rawAx = read16(0x28); 
        int16_t rawAy = read16(0x2A);
        int16_t rawAz = read16(0x2C);
        int16_t rawGx = read16(0x22);
        int16_t rawGy = read16(0x24);
        int16_t rawGz = read16(0x26);

        // Write to global shared data (thread-safe)
        if (xSemaphoreTake(xMotionDataMutex, portMAX_DELAY)) {
            gMotionData.accelX = rawAx;
            gMotionData.accelY = rawAy;
            gMotionData.accelZ = rawAz;
            gMotionData.gyroX  = rawGx;
            gMotionData.gyroY  = rawGy;
            gMotionData.gyroZ  = rawGz;
            xSemaphoreGive(xMotionDataMutex);
        }

        vTaskDelay(pdMS_TO_TICKS(20)); // ~50 Hz sensing
    }
}
