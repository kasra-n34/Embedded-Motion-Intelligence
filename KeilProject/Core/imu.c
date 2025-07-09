#include "imu.h"
#include "i2c.h"

#define WHO_AM_I_REG    0x0F
#define EXPECTED_WHOAMI 0x6C

// Reads 16-bit value from two adjacent registers (low, then high)
int16_t read16(uint8_t baseAddr) {
    uint8_t lsb = I2C0_ReadRegister(LSM6DSO_ADDR, baseAddr);
    uint8_t msb = I2C0_ReadRegister(LSM6DSO_ADDR, baseAddr + 1);
    return (int16_t)((msb << 8) | lsb);
}

void IMU_Init(void) {
    uint8_t whoami = I2C0_ReadRegister(LSM6DSO_ADDR, WHO_AM_I_REG);
    if (whoami != EXPECTED_WHOAMI) {
        // Optionally handle error here (e.g. flash LED or log)
        while (1);
    }

    // Configure accelerometer (CTRL1_XL)
    I2C0_WriteRegister(LSM6DSO_ADDR, 0x10, 0x60);  // 208 Hz, 2g, BW = 100 Hz
    // Configure gyroscope (CTRL2_G)
    I2C0_WriteRegister(LSM6DSO_ADDR, 0x11, 0x60);  // 208 Hz, 250 dps
    // Control register 3 (CTRL3_C)
    I2C0_WriteRegister(LSM6DSO_ADDR, 0x12, 0x44);  // IF_INC: auto-increment enabled
}
