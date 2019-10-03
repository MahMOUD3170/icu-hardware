
#ifndef ICU_H_
#define ICU_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"





extern volatile uint16 g_timeHigh ;
extern volatile uint16 g_timePeriod ;
extern volatile uint16 g_timePeriodPlusHigh ;

extern volatile uint8 g_edgeCount ;
/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}Icu_Clock;

typedef enum
{
	FALLING,RISING
}Icu_EdgeType;

typedef struct
{
	Icu_Clock clock;
	Icu_EdgeType edge;
}Icu_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description : Function to initialize the ICU driver
 * 	1. Set the required clock.
 * 	2. Set the required edge detection.
 * 	3. Enable the Input Capture Interrupt.
 * 	4. Initialize Timer1 Registers
 */
extern void Icu_init();

/*
/*
 * Description: Function to set the required edge detection.
 */
static void Icu_setEdgeDetectionType(const Icu_EdgeType edgeType);

/*
 * Description: Function to get the Timer1 Value when the input is captured
 *              The value stored at Input Capture Register ICR1
 */
static uint16 Icu_getInputCaptureValue(void);

/*
 * Description: Function to clear the Timer1 Value to start count from ZERO
 */
static void Icu_clearTimerValue(void);

/*
 * Description: Function to disable the Timer1 to stop the ICU Driver
 */
static void Icu_DeInit(void);
static void APP_edgeProcessing(void);
#endif /* ICU_H_ */
