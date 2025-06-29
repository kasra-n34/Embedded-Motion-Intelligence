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

int main(void) {
    gSysClock = SysCtlClockFreqSet(
        SYSCTL_OSC_INT | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480,
        50000000); // ~50 MHz

    UART0_Init();
    UART0_Print("Booting...\r\n");

    I2C0_Init();
	
		UART0_Print("?? Scanning I2C bus...\r\n");
		I2C_ScanBus();
	
		uint8_t whoami = I2C0_ReadRegister(LSM6DSO_ADDR, WHO_AM_I_REG);
		UART0_Print("WHO_AM_I = 0x%02X\r\n", whoami);

		if (whoami == EXPECTED_WHOAMI) {
				UART0_Print("? LSM6DSO detected successfully!\r\n");
		} else {
				UART0_Print("? WHO_AM_I mismatch. Check address or wiring.\r\n");
		}
		
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
        // Turn ON LEDs
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_PIN_0 | GPIO_PIN_4);
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_0 | GPIO_PIN_1);
        SysCtlDelay(gSysClock / 3); // ~0.33s

        // Turn OFF LEDs
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, 0);
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0);
        SysCtlDelay(gSysClock / 3); // ~0.33s
    }
}
