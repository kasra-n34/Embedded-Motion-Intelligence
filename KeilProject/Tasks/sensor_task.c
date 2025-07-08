#include "sensor_task.h"
#include "imu.h"       // Your LSM6DSO read16() or sensor read functions
#include "FreeRTOS.h"
#include "task.h"
#include "sensor_data.h"

// Global shared sensor data
MotionData_t gMotionData;

void SensorTask(void *pvParameters) {
    (void)pvParameters;

    while (1) {
        gMotionData.accelX = read16(0x28);  // OUTX_L_A, OUTX_H_A
        gMotionData.accelY = read16(0x2A);
        gMotionData.accelZ = read16(0x2C);

        gMotionData.gyroX = read16(0x22);   // OUTX_L_G, OUTX_H_G
        gMotionData.gyroY = read16(0x24);
        gMotionData.gyroZ = read16(0x26);

        vTaskDelay(pdMS_TO_TICKS(100)); // 10 Hz polling rate
    }
}
