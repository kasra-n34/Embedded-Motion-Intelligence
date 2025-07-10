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

        // ANSI escape codes: clear screen and move cursor to top-left
        UART0_Print("\033[2J");  // Clear screen
        UART0_Print("\033[H");   // Move cursor to home position (top-left)

        // Static layout
        UART0_Print("   === Real-Time IMU Output ===   \r\n");
        UART0_Print("-----------------------------------\r\n");
        UART0_Print("Accel X: %.2f m/s²\r\n", ax);
        UART0_Print("Accel Y: %.2f m/s²\r\n", ay);
        UART0_Print("Accel Z: %.2f m/s²\r\n", az);
        UART0_Print("\r\n");
        UART0_Print("Gyro  X: %.2f deg/s\r\n", gx);
        UART0_Print("Gyro  Y: %.2f deg/s\r\n", gy);
        UART0_Print("Gyro  Z: %.2f deg/s\r\n", gz);

        vTaskDelay(pdMS_TO_TICKS(500)); // 10 Hz logging
    }
}
