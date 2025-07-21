#include "imu.h"
#include "i2c.h"
#include "uart.h"
#include "FreeRTOS.h"   // For vTaskDelay if using FreeRTOS
#include "task.h"       // For pdMS_TO_TICKS
#include "driverlib/sysctl.h"


#define WHO_AM_I_REG    0x0F
#define EXPECTED_WHOAMI 0x6C

// Reads 16-bit value from two adjacent registers (low, then high)
int16_t read16(uint8_t baseAddr) {
    uint8_t lsb = I2C0_ReadRegister(LSM6DSO_ADDR, baseAddr);
    uint8_t msb = I2C0_ReadRegister(LSM6DSO_ADDR, baseAddr + 1);
    return (int16_t)((msb << 8) | lsb);
}

void IMU_Init(void) {
    uint8_t whoami;
    int retryCount = 0;

    do {
        whoami = I2C0_ReadRegister(LSM6DSO_ADDR, WHO_AM_I_REG);

        if (whoami != EXPECTED_WHOAMI) {
            UART0_Print("IMU not detected (WHO_AM_I=0x%02X). Retrying... (%d)\r\n",
                        whoami, retryCount++);
            
            // ? Wait before retrying (works in both bare-metal and FreeRTOS)
            #ifdef FreeRTOS
                vTaskDelay(pdMS_TO_TICKS(500));  // 500 ms
            #else
                SysCtlDelay((gSysClock / 3) / 2); // ~0.5 s
            #endif
        }
    } while (whoami != EXPECTED_WHOAMI);

    UART0_Print("IMU detected! WHO_AM_I=0x%02X\r\n", whoami);

    // ? Configure accelerometer and gyroscope once detected
    I2C0_WriteRegister(LSM6DSO_ADDR, 0x10, 0x60);  // 208 Hz, 2g
    I2C0_WriteRegister(LSM6DSO_ADDR, 0x11, 0x60);  // 208 Hz, 250 dps
    I2C0_WriteRegister(LSM6DSO_ADDR, 0x12, 0x44);  // Auto-increment
}
