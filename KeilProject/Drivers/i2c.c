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

    // Initialize and configure I2C0 master (false = 100kHz)
    I2CMasterInitExpClk(I2C0_BASE, gSysClock, false);
    I2CMasterEnable(I2C0_BASE);

    // Optional: give I2C devices time to stabilize
    SysCtlDelay(gSysClock / 100); // ~10ms delay
}

uint8_t I2C0_ReadRegister(uint8_t slaveAddr, uint8_t regAddr) {
    int timeout;

    // Write register address
    I2CMasterSlaveAddrSet(I2C0_BASE, slaveAddr, false);
    I2CMasterDataPut(I2C0_BASE, regAddr);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    timeout = 10000;
    while (I2CMasterBusy(I2C0_BASE) && --timeout);
    if (timeout == 0 || I2CMasterErr(I2C0_BASE)) {
        UART0_Print("I2C write timeout or error (read reg 0x%02X)\r\n", regAddr);
        return 0xEE;
    }

    // Small delay
    SysCtlDelay(gSysClock / 3000); // ~1ms

    // Read register content
    I2CMasterSlaveAddrSet(I2C0_BASE, slaveAddr, true); // Read
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
    timeout = 10000;
    while (I2CMasterBusy(I2C0_BASE) && --timeout);
    if (timeout == 0 || I2CMasterErr(I2C0_BASE)) {
        UART0_Print("I2C read timeout or error (reg 0x%02X)\r\n", regAddr);
        return 0xEF;
    }

    return (uint8_t)I2CMasterDataGet(I2C0_BASE);
}

void I2C0_WriteRegister(uint8_t slaveAddr, uint8_t regAddr, uint8_t data) {
    int timeout;

    I2CMasterSlaveAddrSet(I2C0_BASE, slaveAddr, false); // Write
    I2CMasterDataPut(I2C0_BASE, regAddr);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    timeout = 10000;
    while (I2CMasterBusy(I2C0_BASE) && --timeout);
    if (timeout == 0 || I2CMasterErr(I2C0_BASE)) {
        UART0_Print("I2C write error at reg 0x%02X (phase 1)\r\n", regAddr);
        return;
    }

    I2CMasterDataPut(I2C0_BASE, data);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    timeout = 10000;
    while (I2CMasterBusy(I2C0_BASE) && --timeout);
    if (timeout == 0 || I2CMasterErr(I2C0_BASE)) {
        UART0_Print("I2C write error at reg 0x%02X (phase 2)\r\n", regAddr);
        return;
    }
}

void I2C_ScanBus(void) {
    UART0_Print("?? Scanning I2C bus...\r\n");

    for (uint8_t addr = 0x08; addr <= 0x77; addr++) {
        I2CMasterSlaveAddrSet(I2C0_BASE, addr, false); // Write mode

        // Write dummy byte to provoke ACK/NACK
        I2CMasterDataPut(I2C0_BASE, 0x00);
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);

        int timeout = 10000;
        while (I2CMasterBusy(I2C0_BASE) && --timeout);

        if (I2CMasterErr(I2C0_BASE) == I2C_MASTER_ERR_NONE) {
            UART0_Print("I2C device found at 0x%02X\r\n", addr);
        }
    }
}

// New multi-byte read helper 
void I2C0_ReadMulti(uint8_t slaveAddr, uint8_t regAddr, uint8_t *data, uint8_t len) {
    // Set starting register (write phase)
    I2CMasterSlaveAddrSet(I2C0_BASE, slaveAddr, false);
    I2CMasterDataPut(I2C0_BASE, regAddr);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    while (I2CMasterBusy(I2C0_BASE));

    // Switch to read mode
    I2CMasterSlaveAddrSet(I2C0_BASE, slaveAddr, true);

    for (uint8_t i = 0; i < len; i++) {
        if (i == 0)
            I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
        else if (i == len - 1)
            I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
        else
            I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);

        while (I2CMasterBusy(I2C0_BASE));
        data[i] = I2CMasterDataGet(I2C0_BASE);
    }
}
