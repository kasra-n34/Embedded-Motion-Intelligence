#include "logger_task.h"
#include "sensor_data.h"
#include "uart.h"
#include "FreeRTOS.h"
#include "task.h"


// Access global sensor data
extern MotionData_t gMotionData;

void LoggerTask(void *pvParameters) {
    (void)pvParameters;

    while (1) {
        UART0_Print("AX:%d,AY:%d,AZ:%d | GX:%d,GY:%d,GZ:%d\r\n",
                    gMotionData.accelX,
                    gMotionData.accelY,
                    gMotionData.accelZ,
                    gMotionData.gyroX,
                    gMotionData.gyroY,
                    gMotionData.gyroZ);

        vTaskDelay(pdMS_TO_TICKS(100)); // 10 Hz logging
    }
}
