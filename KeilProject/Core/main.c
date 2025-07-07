#include <stdint.h>
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "uart.h"
#include "i2c.h"
#include "hw_memmap.h"
#include "hw_types.h"
#include "hw_gpio.h"

#define LSM6DSO_ADDR 0x6A
#define WHO_AM_I_REG 0x0F
#define EXPECTED_WHOAMI 0x6C


uint32_t gSysClock;

int16_t read16(uint8_t baseAddr) {
    uint8_t lsb = I2C0_ReadRegister(LSM6DSO_ADDR, baseAddr);
    uint8_t msb = I2C0_ReadRegister(LSM6DSO_ADDR, baseAddr + 1);
    return (int16_t)((msb << 8) | lsb);
}

int main(void) {
    gSysClock = SysCtlClockFreqSet(
        SYSCTL_OSC_INT | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480,
        50000000); // ~50 MHz

    UART0_Init();
    UART0_Print("Booting...\r\n");

    I2C0_Init();
	
		//UART0_Print("?? Scanning I2C bus...\r\n");
		//I2C_ScanBus();
	
		uint8_t whoami = I2C0_ReadRegister(LSM6DSO_ADDR, WHO_AM_I_REG);
		UART0_Print("WHO_AM_I = 0x%02X\r\n", whoami);

		if (whoami == EXPECTED_WHOAMI) {
				UART0_Print("? LSM6DSO detected successfully!\r\n");
		} else {
				UART0_Print("? WHO_AM_I mismatch. Check address or wiring.\r\n");
		}
		
	// Use conservative ODR and check each write
	I2C0_WriteRegister(LSM6DSO_ADDR, 0x10, 0x40); // CTRL1_XL: Accel 104Hz ±2g
	SysCtlDelay(gSysClock / 3000); // 1ms

	I2C0_WriteRegister(LSM6DSO_ADDR, 0x11, 0x40); // CTRL2_G: Gyro 104Hz 250 dps
	SysCtlDelay(gSysClock / 3000); // 1ms

	I2C0_WriteRegister(LSM6DSO_ADDR, 0x12, 0x44); // CTRL3_C: BDU=1, IF_INC=1
	SysCtlDelay(gSysClock / 3000); // 1ms

	// Debug: read back values
	uint8_t r1 = I2C0_ReadRegister(LSM6DSO_ADDR, 0x10);
	uint8_t r2 = I2C0_ReadRegister(LSM6DSO_ADDR, 0x11);
	uint8_t r3 = I2C0_ReadRegister(LSM6DSO_ADDR, 0x12);
	UART0_Print("CTRL1_XL = 0x%02X | CTRL2_G = 0x%02X | CTRL3_C = 0x%02X\r\n", r1, r2, r3);


		int16_t ax = read16(0x28);
		int16_t ay = read16(0x2A);
		int16_t az = read16(0x2C);
		int16_t gx = read16(0x22);
		int16_t gy = read16(0x24);
		int16_t gz = read16(0x26);


	UART0_Print("ACCEL X:%d Y:%d Z:%d | GYRO X:%d Y:%d Z:%d\r\n", ax, ay, az, gx, gy, gz);

		
    // Enable LED GPIO peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION));

    // Unlock PF0 (if needed)
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= GPIO_PIN_0;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;

    // Set LED pins as output
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4); // RED + GREEN
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1); // BLUE + WHITE

    while (1) {
			
			ax = read16(0x28); // OUTX_L_A
			ay = read16(0x2A); // OUTY_L_A
			az = read16(0x2C); // OUTZ_L_A

			gx = read16(0x22); // OUTX_L_G
			gy = read16(0x24); // OUTY_L_G
			gz = read16(0x26); // OUTZ_L_G


    UART0_Print("AX:%d AY:%d AZ:%d | GX:%d GY:%d GZ:%d\r\n", ax, ay, az, gx, gy, gz);
    SysCtlDelay(gSysClock / 5); // ~100ms delay
    }
}
