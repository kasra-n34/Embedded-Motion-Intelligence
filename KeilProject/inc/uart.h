#ifndef UART_H
#define UART_H

#include <stdint.h>

void UART0_Init(void);
void UART0_Print(const char *format, ...);
void UART0_PrintInt(int32_t num);
void UART0_PrintLn(void);

#endif
