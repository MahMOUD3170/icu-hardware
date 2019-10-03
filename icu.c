
#include "icu.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

volatile uint8 g_edgeCount = 0;
volatile uint16 g_timeHigh = 0;
volatile uint16 g_timePeriod = 0;
volatile uint16 g_timePeriodPlusHigh = 0;


/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER1_CAPT_vect)
{
	APP_edgeProcessing();
}


void Icu_init()
{/* Enable Global Interrupt */
	SREG|=(1<<7);
	/* Configure ICP1/PD6 as i/p pin */
	DDRD &= ~(1<<PD6);

	/* Timer1 always operates in Normal Mode */
	TCCR1A = (1<<FOC1A) | (1<<FOC1B);

	/*
	 * insert the required clock value in the first three bits (CS10, CS11 and CS12)
	 * of TCCR1B Register
	 */
	TCCR1B = (TCCR1B & 0xF8) | F_CPU_CLOCK;
	/*
	 * insert the required edge type in ICES1 bit in TCCR1B Register
	 */
	TCCR1B = (TCCR1B & 0xBF) | RISING;

	/* Initial Value for Timer1 */
	TCNT1 = 0;

	/* Initial Value for the input capture register */
	ICR1 = 0;

	/* Enable the Input Capture interrupt to generate an interrupt when edge is detected on ICP1/PD6 pin */
	TIMSK |= (1<<TICIE1);

}

/*


 * Description: Function to set the required edge detection.
 */
static void Icu_setEdgeDetectionType(const Icu_EdgeType a_edgeType)
{
	/*
	 * insert the required edge type in ICES1 bit in TCCR1B Register
	 */
	TCCR1B = (TCCR1B & 0xBF) | (a_edgeType<<6);
}

/*
 * Description: Function to get the Timer1 Value when the input is captured
 *              The value stored at Input Capture Register ICR1
 */
static uint16 Icu_getInputCaptureValue(void)
{
	return ICR1;
}

/*
 * Description: Function to clear the Timer1 Value to start count from ZERO
 */
static void Icu_clearTimerValue(void)
{
	TCNT1 = 0;
}

/*
 * Description: Function to disable the Timer1 to stop the ICU Driver
 */
static void Icu_DeInit(void)
{
	/* Clear All Timer1 Registers */
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	ICR1 = 0;

	/* Disable the Input Capture interrupt */
	TIMSK &= ~(1<<TICIE1);
}
static void APP_edgeProcessing(void)
{
	g_edgeCount++;
	if(g_edgeCount == 1)
	{
		/*
		 * Clear the timer counter register to start measurements from the
		 * first detected rising edge
		 */
		Icu_clearTimerValue();
		/* Detect falling edge */
		Icu_setEdgeDetectionType(FALLING);
	}
	else if(g_edgeCount == 2)
	{
		/* Store the High time value */
		g_timeHigh = Icu_getInputCaptureValue();
		/* Detect rising edge */
		Icu_setEdgeDetectionType(RISING);
	}
	else if(g_edgeCount == 3)
	{
		/* Store the Period time value */
		g_timePeriod = Icu_getInputCaptureValue();
		/* Detect falling edge */
		Icu_setEdgeDetectionType(FALLING);
	}
	else if(g_edgeCount == 4)
	{
		/* Store the Period time value + High time value */
		g_timePeriodPlusHigh = Icu_getInputCaptureValue();
		/* Clear the timer counter register to start measurements again */
		Icu_clearTimerValue();
		Icu_setEdgeDetectionType(RISING);
		Icu_DeInit();
			}
}
