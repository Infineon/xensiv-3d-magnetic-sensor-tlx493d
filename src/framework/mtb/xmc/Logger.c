// std includes
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

// project cpp includes

// project c includes
#include "Logger.h"
#include "tlx493d_types.h"

// MTB includes
#include "cybsp.h"
#include "cy_utils.h"
#include "cy_retarget_io.h"


#define BUFFER_SIZE  512



void printRegisters(TLx493D_t *sensor, const char *headLine) {
    printf("\n");
    printf(headLine);
    printf("\n"); 

    for(uint8_t i = 0; i < sensor->regMapSize; ++i) {
        printf("0x%02x\n", sensor->regMap[i]);
    }

    printf("\n");
}

void printDouble(double d) {
    printf("%f", d);
}


static void logMessage(const char *prefix, const char *format, va_list vaList) {
    char buffer[BUFFER_SIZE];

    size_t prefixSize = strlen(prefix);
    memcpy(buffer, prefix, prefixSize);
    int ret = vsprintf(buffer + prefixSize, format, vaList);

    if( (ret + prefixSize) > BUFFER_SIZE ) {
        printf("FATAL : Buffer overflow (> %d characters) because message too long !\n", BUFFER_SIZE);
    }

    printf("%s\n", buffer);
}

void println(const char *format, ...) {
    printf("\n");    
    va_list ap;
    va_start(ap, format);
    logMessage("", format, ap);
    va_end(ap);
    printf("\n");
}



void print(const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    logMessage("", format, ap);
    va_end(ap);
}


void info(const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    logMessage("INFO : ", format, ap);
    va_end(ap);
}


void warn(const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    logMessage("WARNING : ", format, ap);
    va_end(ap);
}


void error(const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    logMessage("ERROR : ", format, ap);
    va_end(ap);
}


void flush(){ //todo: decide if this is the place for flush or I2C implementation file
   // Reset buffer and clear all Status Flags
    XMC_USIC_CH_TXFIFO_Flush(CYBSP_DEBUG_UART_HW);
    XMC_USIC_CH_RXFIFO_Flush(CYBSP_DEBUG_UART_HW);
    XMC_USIC_CH_SetTransmitBufferStatus(CYBSP_DEBUG_UART_HW, XMC_USIC_CH_TBUF_STATUS_SET_IDLE);
	
	XMC_I2C_CH_ClearStatusFlag(CYBSP_DEBUG_UART_HW, 0xFFFFFFFF);
} 

