#include "logger_task.h"
#include "sensor_data.h"
#include "uart.h"
#include "FreeRTOS.h"
#include "task.h"

// Access global sensor data
extern MotionData_t gMotionData;

// Conversion factors based on ±2g and ±250 dps full scale
#define ACCEL_SCALE_2G     (9.81f * 2.0f / 32768.0f)   // m/s² per LSB
#define GYRO_SCALE_250DPS  (250.0f / 32768.0f)         // °/s per LSB

void LoggerTask(void *pvParameters) {
    (void)pvParameters;

    while (1) {
        float ax = gMotionData.accelX * ACCEL_SCALE_2G;
        float ay = gMotionData.accelY * ACCEL_SCALE_2G;
        float az = gMotionData.accelZ * ACCEL_SCALE_2G;

        float gx = gMotionData.gyroX * GYRO_SCALE_250DPS;
        float gy = gMotionData.gyroY * GYRO_SCALE_250DPS;
        float gz = gMotionData.gyroZ * GYRO_SCALE_250DPS;

        UART0_Print("AX:%.2f AY:%.2f AZ:%.2f | GX:%.2f GY:%.2f GZ:%.2f\r\n",
                    ax, ay, az, gx, gy, gz);

        vTaskDelay(pdMS_TO_TICKS(1000)); // 10 Hz logging
    }
}
