/*
 * beaglebone_leds.c
 */

#include <stdio.h>
#include "beaglebone_leds.h"

#ifndef SIMULATED
static void
writeToFile(char* filename, char* text)
{
    FILE* file = fopen(filename, "w");
    fprintf(file, "%s", text);
    fclose(file);
}
#endif /* SIMULATED */

void
flashLED(char* led)
{
#ifndef SIMULATED
    char filename[255];

    sprintf(filename, "%s/trigger", led);
    writeToFile(filename, "timer");

    sprintf(filename, "%s/delay_on", led);
    writeToFile(filename, "200");

    sprintf(filename, "%s/delay_off", led);
    writeToFile(filename, "200");
#else
    printf("FLASH: %s\n", led);
#endif
}

void
switchLED(char* led, int value)
{
#ifndef SIMULATED
    char filename[255];
    char valueStr[10];

    sprintf(filename, "%s/trigger", led);
    writeToFile(filename, "none");

    sprintf(filename, "%s/brightness", led);
    sprintf(valueStr, "%i", value);

    writeToFile(filename, valueStr);
#else
    printf("SWITCH: %s %i\n", led, value);
#endif
}

void
initLEDs()
{
    switchLED(LED1, 0);
    switchLED(LED2, 0);
    switchLED(LED3, 0);
    switchLED(LED4, 0);
}
