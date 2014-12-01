/*
 * beaglebone_leds.h
 */

#ifndef BEAGLEBONE_LEDS_H_
#define BEAGLEBONE_LEDS_H_


//#define SIMULATED 1


#if 0
#define LED1 "/sys/class/leds/beaglebone::usr0"
#define LED2 "/sys/class/leds/beaglebone::usr1"
#define LED3 "/sys/class/leds/beaglebone::usr2"
#define LED4 "/sys/class/leds/beaglebone::usr3"
#endif

#if 1
#define LED1 "/sys/class/leds/beaglebone:green:usr0"
#define LED2 "/sys/class/leds/beaglebone:green:usr1"
#define LED3 "/sys/class/leds/beaglebone:green:usr2"
#define LED4 "/sys/class/leds/beaglebone:green:usr3"
#endif

void
flashLED(char* led);

void
switchLED(char* led, int value);

void
initLEDs();

#endif /* BEAGLEBONE_LEDS_H_ */
