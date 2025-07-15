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
        float ax, ay, az, gx, gy, gz;

        // Safely read the shared data
        if (xSemaphoreTake(xMotionDataMutex, portMAX_DELAY)) {
            ax = gMotionData.accelX;
            ay = gMotionData.accelY;
            az = gMotionData.accelZ;
            gx = gMotionData.gyroX;
            gy = gMotionData.gyroY;
            gz = gMotionData.gyroZ;
            xSemaphoreGive(xMotionDataMutex);
        }

        // Scale to physical units
        float ax_mps2 = ax * ACCEL_SCALE_2G;
        float ay_mps2 = ay * ACCEL_SCALE_2G;
        float az_mps2 = az * ACCEL_SCALE_2G;
        float gx_dps  = gx * GYRO_SCALE_250DPS;
        float gy_dps  = gy * GYRO_SCALE_250DPS;
        float gz_dps  = gz * GYRO_SCALE_250DPS;
				
				//UART0_Print("\033[2J");  // Clear screen

        // Print to UART
        UART0_Print("=== RAW OUTPUT ===\r\n");
        UART0_Print("Accel: X=%.2f m/s^2, Y=%.2f m/s^2, Z=%.2f m/s^2\r\n", ax_mps2, ay_mps2, az_mps2);
        UART0_Print("Gyro : X=%.2f deg/s, Y=%.2f deg/s, Z=%.2f deg/s\r\n\r\n", gx_dps, gy_dps, gz_dps);

				
        vTaskDelay(pdMS_TO_TICKS(500)); // ~2 Hz
    }
}

