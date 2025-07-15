#include "ml_task.h"
#include "sensor_data.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "uart.h"  // For UARTprintf if you use it

// External shared motion data
extern MotionData_t gMotionData;

void MLTask(void *pvParameters) {
    (void)pvParameters;

    while (1) {
        // Copy latest sensor values (optionally use mutex or copy struct if needed)
        float ax = gMotionData.accelX ;
        float ay = gMotionData.accelY;
        float az = gMotionData.accelZ;

        float gx = gMotionData.gyroX;
        float gy = gMotionData.gyroY;
        float gz = gMotionData.gyroZ;

        // Run inference (stub)
        int result = run_inference(ax, ay, az, gx, gy, gz);

        // Output result
        UART0_Print("ML Inference Result: %d\r\n", result);

        // TODO: Send result to LEDTask via queue/semaphore if needed

        vTaskDelay(pdMS_TO_TICKS(500));  // Adjust frequency as needed
    }
}

// Placeholder inference function (replace with actual model call)
int run_inference(float ax, float ay, float az, float gx, float gy, float gz) {
    // Stub logic — use thresholds or dummy classifier
    float accel_magnitude = ax * ax + ay * ay + az * az;
    if (accel_magnitude < 0.5f) return 0; // Still
    else return 1; // Moving
}
