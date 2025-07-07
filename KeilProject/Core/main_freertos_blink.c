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
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

#define LSM6DSO_ADDR 0x6A
#define WHO_AM_I_REG 0x0F
#define EXPECTED_WHOAMI 0x6C


uint32_t gSysClock;

int16_t read16(uint8_t baseAddr) {
    uint8_t lsb = I2C0_ReadRegister(LSM6DSO_ADDR, baseAddr);
    uint8_t msb = I2C0_ReadRegister(LSM6DSO_ADDR, baseAddr + 1);
    return (int16_t)((msb << 8) | lsb);
}



void BlinkTask(void *pvParameters) {
    while (1) {
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_PIN_0);
        vTaskDelay(pdMS_TO_TICKS(500));
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

int main(void) {
    gSysClock = SysCtlClockFreqSet(
        SYSCTL_OSC_INT | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480,
        50000000);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);

    xTaskCreate(BlinkTask, "Blink", 128, NULL, 1, NULL);

    vTaskStartScheduler(); // Start FreeRTOS

    while (1); // Never reaches here
}