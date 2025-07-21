#include "logger_task.h"
#include "sensor_data.h"
#include "uart.h"
#include "FreeRTOS.h"
#include "task.h"

// Access global sensor data
extern MotionData_t gMotionData;

#define ACCEL_SCALE_2G     (9.81f * 2.0f / 32768.0f)   // m/s² per LSB
#define GYRO_SCALE_250DPS  (250.0f / 32768.0f)         // deg/s per LSB

// Manually set before each recording session
static int currentLabel = 1;  // 0=stationary, 1=walking, 2=running...

void LoggerTask(void *pvParameters) {
    (void)pvParameters;

    // Print CSV header once
    UART0_Print("timestamp_ms,accelX_mps2,accelY_mps2,accelZ_mps2,gyroX_dps,gyroY_dps,gyroZ_dps,label\r\n");

    while (1) {
        int16_t rawAx = 0, rawAy = 0, rawAz = 0;
        int16_t rawGx = 0, rawGy = 0, rawGz = 0;

        // Safely copy the latest sensor data
        if (xSemaphoreTake(xMotionDataMutex, portMAX_DELAY)) {
            rawAx = gMotionData.accelX;
            rawAy = gMotionData.accelY;
            rawAz = gMotionData.accelZ;
            rawGx = gMotionData.gyroX;
            rawGy = gMotionData.gyroY;
            rawGz = gMotionData.gyroZ;
            xSemaphoreGive(xMotionDataMutex);
        }

        // Convert to physical units
        float ax_mps2 = rawAx * ACCEL_SCALE_2G;
        float ay_mps2 = rawAy * ACCEL_SCALE_2G;
        float az_mps2 = rawAz * ACCEL_SCALE_2G;
        float gx_dps  = rawGx * GYRO_SCALE_250DPS;
        float gy_dps  = rawGy * GYRO_SCALE_250DPS;
        float gz_dps  = rawGz * GYRO_SCALE_250DPS;

        // Timestamp in ms
        TickType_t timestamp = xTaskGetTickCount();

        // Print CSV row
        UART0_Print("%lu,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%d\r\n",
                    (unsigned long)timestamp,
                    ax_mps2, ay_mps2, az_mps2,
                    gx_dps, gy_dps, gz_dps,
                    currentLabel);

        vTaskDelay(pdMS_TO_TICKS(20)); // ~50 Hz logging (same as sensing)
    }
}
