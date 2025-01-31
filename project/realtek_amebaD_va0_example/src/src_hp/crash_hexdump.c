#include <CrashCatcher.h>
#include "FreeRTOSConfig.h"

extern void LOGUART_PutChar(uint8_t c);
extern uint8_t LOGUART_GetChar(uint8_t    PullMode);

static void printString(const char* pString);

const CrashCatcherMemoryRegion* CrashCatcher_GetMemoryRegions(void)
{
    static const CrashCatcherMemoryRegion regions[] = {
        {0x10004000, 0x10005000, CRASH_CATCHER_BYTE},
        {0x1003CF00, 0x1003D800, CRASH_CATCHER_BYTE},
        {0xFFFFFFFF, 0xFFFFFFFF, CRASH_CATCHER_BYTE}
    };
    return regions;
}

void CrashCatcher_putc(int c)
{
    LOGUART_PutChar((uint8_t)c);
}

int CrashCatcher_getc(void)
{
    return (int)LOGUART_GetChar(0);
}

static void printString(const char* pString)
{
    while (*pString)
        CrashCatcher_putc(*pString++);
}


CrashCatcherReturnCodes CrashCatcher_DumpEnd(void)
{
    printString("\r\nEnd of dump\r\n");

    while (1);
    return CRASH_CATCHER_EXIT;
}
