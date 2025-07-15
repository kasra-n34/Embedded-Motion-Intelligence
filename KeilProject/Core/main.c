#include <stdint.h>
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "imu.h"
#include "uart.h"
#include "i2c.h"
#include "hw_memmap.h"
#include "hw_types.h"
#include "hw_gpio.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "sensor_data.h"
#include "sensor_task.h"  
#include "logger_task.h"
#include "ml_task.h"

#define LSM6DSO_ADDR 0x6A
#define WHO_AM_I_REG 0x0F
#define EXPECTED_WHOAMI 0x6C

uint32_t gSysClock;

int main(void) {
    // Clock config
    gSysClock = SysCtlClockFreqSet(
        SYSCTL_OSC_INT | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480,
        50000000);

    UART0_Init();
    UART0_Print("Booting...\r\n");

    I2C0_Init();
		UART0_Print("I2C initialized.\r\n");
	
    IMU_Init();  // LSM6DSO setup
		UART0_Print("IMU initialized.\r\n");

    // Verify IMU communication
    uint8_t whoami = I2C0_ReadRegister(LSM6DSO_ADDR, WHO_AM_I_REG);
    UART0_Print("WHO_AM_I = 0x%02X\r\n", whoami);
    if (whoami == EXPECTED_WHOAMI) {
        UART0_Print("? LSM6DSO detected successfully!\r\n");
    } else {
        UART0_Print("? WHO_AM_I mismatch. Check address or wiring.\r\n");
    }

    // LED GPIO Setup
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION));

    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= GPIO_PIN_0;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4); 
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1);

		// Create mutex
    xMotionDataMutex = xSemaphoreCreateMutex();
    if (xMotionDataMutex == NULL) {
        // Handle error (e.g., blink LED forever)
				UART0_Print("No data.\r\n");
        while(1);
    }
		
    xTaskCreate(SensorTask, "Sensor", 512, NULL, 2, NULL);
		xTaskCreate(LoggerTask, "Logger", 1024, NULL, 1, NULL);
		//xTaskCreate(MLTask, "MLTask", 256, NULL, 2, NULL);

		vTaskStartScheduler(); // Start FreeRTOS
		
		while (1); // should never reach here
}
