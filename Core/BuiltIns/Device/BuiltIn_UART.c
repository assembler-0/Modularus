#include <BuiltIns/Device/UART.h>
#include <System.h>
#include <KernelCLibrary.h>

#include "IOPort.h"

static uint16_t SerialPort = UART_COM1Base;
static SYSTEM_NODE* SerialNode = 0;

static long UART_Read(SYSTEM_FILE*, void*, uint64_t, SYSTEM_ERROR*);
static long UART_Write(SYSTEM_FILE*, const void*, uint64_t, SYSTEM_ERROR*);

static SYSTEM_OPERATIONS UARTOperations = 
{
    .Open = 0,      /* Default */
    .Close = 0,     /* Default */
    .Read = UART_Read,
    .Write = UART_Write,
    .Getattr = 0,   /* Default */
    .Setattr = 0    /* Default */
};

void
UART_PutCharacter(char Character, SYSTEM_ERROR* Error __unused)
{
    while ((inb(SerialPort + UART_RegisterLineStatus) & UART_LSRTHREmpty) == 0);
    
    outb(SerialPort + UART_RegisterData, (uint8_t)Character);
    
    if (Character == '\n')
    {
        while ((inb(SerialPort + UART_RegisterLineStatus) & UART_LSRTHREmpty) == 0);
        outb(SerialPort + UART_RegisterData, '\r');
    }
}

void
UART_PutString(const char* String, SYSTEM_ERROR* Error)
{
    while (*String)
    {
        UART_PutCharacter(*String, Error);
        String++;
    }
}

uint16_t
UART_GetPort(SYSTEM_ERROR* Error __unused)
{
    return SerialPort;
}

static long
UART_Read(SYSTEM_FILE* File __unused, void* Buffer __unused,  uint64_t Size __unused, SYSTEM_ERROR* Error __unused)
{  
    /*WriteOnly*/
    return 0;
}

static long
UART_Write(SYSTEM_FILE* File __unused, const void* Buffer, uint64_t Size, SYSTEM_ERROR* Error)
{
    const char* String = (const char*)Buffer;
    for (uint64_t Index = 0; Index < Size; Index++)
    {
        UART_PutCharacter(String[Index], Error);
    }

    return Size;
}

int
UART_KickStart(SYSTEM_ERROR* Error)
{
    #define ErrorOut_UART_KickStart(Code) \
        ErrorOut(Error, Code, General)

    outb(SerialPort + UART_RegisterInterrupt, 0);
    outb(SerialPort + UART_RegisterLineControl, 0x80);  /* DLAB = 1 */
    outb(SerialPort + UART_RegisterData, 0x03);         /* Divisor = 3 (115200/3=38400) */
    outb(SerialPort + UART_RegisterInterrupt, 0x00);    /* DLAB = 0 */
    outb(SerialPort + UART_RegisterLineControl, 0x03);
    outb(SerialPort + UART_RegisterFIFOControl, 0xC7);
    outb(SerialPort + UART_RegisterModemControl, 0x0B);
    inb(SerialPort + UART_RegisterLineStatus);
    outb(SerialPort + UART_RegisterInterrupt, 0x00);

    SerialNode = System_CreateFile("uart", &UARTOperations, 0, 0, Error);
    if (Probe4Error(SerialNode) || !SerialNode)
    {
        ErrorOut_UART_KickStart(Error->ErrorCode);
        return Error->ErrorCode;
    }
    
    int Result = System_AttachNode(SystemRoot, SerialNode, Error);
    if (Result != GeneralOK)
    {
        ErrorOut_UART_KickStart(Result);
        return Result;
    }
    
    return GeneralOK;
}

