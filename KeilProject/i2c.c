#include "i2c.h"
#include "driverlib/i2c.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "hw_memmap.h"
#include "uart.h"

extern uint32_t gSysClock;

void I2C0_Init(void) {
    // Enable I2C0 and GPIOB peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C0) ||
           !SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB));

    // Configure GPIO pins for I2C0: PB2 = I2C0SCL, PB3 = I2C0SDA
    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);  // SCL pin
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);     // SDA pin

    // Initialize and configure I2C0 master
    I2CMasterInitExpClk(I2C0_BASE, gSysClock, false); // false = 100kHz standard mode
    I2CMasterEnable(I2C0_BASE);
}

uint8_t I2C0_ReadRegister(uint8_t slaveAddr, uint8_t regAddr) {
    I2CMasterSlaveAddrSet(I2C0_BASE, slaveAddr, false); // Write
    I2CMasterDataPut(I2C0_BASE, regAddr);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    int timeout = 10000;
    while (I2CMasterBusy(I2C0_BASE) && --timeout);
    if (timeout == 0 || I2CMasterErr(I2C0_BASE)) {
        UART0_Print("I2C write timeout or error\r\n");
        return 0xEE;
    }

    SysCtlDelay(gSysClock / 3000); // 1ms safety

    I2CMasterSlaveAddrSet(I2C0_BASE, slaveAddr, true); // Read
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
    timeout = 10000;
    while (I2CMasterBusy(I2C0_BASE) && --timeout);
    if (timeout == 0 || I2CMasterErr(I2C0_BASE)) {
        UART0_Print("I2C read timeout or error\r\n");
        return 0xEF;
    }

    return (uint8_t)I2CMasterDataGet(I2C0_BASE);
}




void I2C0_WriteRegister(uint8_t slaveAddr, uint8_t regAddr, uint8_t data) {
    I2CMasterSlaveAddrSet(I2C0_BASE, slaveAddr, false);
    I2CMasterDataPut(I2C0_BASE, regAddr);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    while (I2CMasterBusy(I2C0_BASE));

    I2CMasterDataPut(I2C0_BASE, data);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    while (I2CMasterBusy(I2C0_BASE));
}


void I2C_ScanBus(void) {
    for (uint8_t addr = 0x08; addr <= 0x77; addr++) {
        I2CMasterSlaveAddrSet(I2C0_BASE, addr, false); // Write mode
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
        while (I2CMasterBusy(I2C0_BASE));
        if (I2CMasterErr(I2C0_BASE) == I2C_MASTER_ERR_NONE) {
            UART0_Print("I2C device found at 0x%02X\r\n", addr);
        }
    }
}

