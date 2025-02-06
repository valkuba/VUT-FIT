/******************************************************************************/
/*                                                                            */
/* Reseni projektu z predmetu IMP  		                                      */
/*                                                                            */
/* ARM-KL05: LED náramkové hodinky na bázi RTC modulu      			          */
/*                                                                            */
/* Reseni vytvoril(a) a odevzdava: (Jakub Valeš, xvales04)                    */
/*                                                                            */
/******************************************************************************/

#include "MKL05Z4.h"


/* Activation of particular LED display (DS1 - DS4) */
#define D1 0x0700
#define D2 0x0B00
#define D3 0x0D00
#define D4 0x0E00


/* Encoding of digits as active segments on specific LED display (DS1 - DS4) */
#define N0 0x0707
#define N1 0x0006
#define N2 0x0B03
#define N3 0x0907
#define N4 0x0C06
#define N5 0x0D05
#define N6 0x0F05
#define N7 0x0007
#define N8 0x0F07
#define N9 0x0D07


/* Bit-level masks that help to enable/disable DP segment on LED display */
#define MASK_DOT_ON 0x0008
#define MASK_DOT_OFF 0xFFF7


#define PB4_ISF_MASK 0x10


#define TIME 60				// Value in seconds after which RTC will update time
uint8_t show_display = 0;	// 0 -> LED display off, 1 -> LED display on
uint8_t mode = 0;			// Set up time modes -> (0,1,2,3); Update RTC->TAR ->(4); Show time mode -> (5)

/* Variables for each segment of LED display showing digits of time
 * hours1 hours0. minutes1 mintes0 */
uint8_t minutes0 = 0;
uint8_t minutes1 = 0;
uint8_t hours0 = 0;
uint8_t hours1 = 0;

/* Just an ordinary delay loop */
void delay(long long bound) {
	long long i;
	for(i=0;i<bound;i++);
}


/* Let's turn off individual segments on the whole display */
void off() {
	PTB->PDOR = GPIO_PDOR_PDO(0x0000);
	PTA->PDOR = GPIO_PDOR_PDO(D1);
	PTA->PDOR = GPIO_PDOR_PDO(D2);
	PTA->PDOR = GPIO_PDOR_PDO(D3);
	PTA->PDOR = GPIO_PDOR_PDO(D4);
}


/* Basic initialization of GPIO features on PORTA and PORTB */
void ports_init ( void ) {
	SIM->COPC = SIM_COPC_COPT(0x00);							   // Just disable the usage of WatchDog feature
	SIM->SCGC5 = (SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK);  // Turn on clocks for PORTA and PORTB

	/* Set corresponding PORTA pins for GPIO functionality */
	PORTA->PCR[8] = ( 0|PORT_PCR_MUX(0x01) );  // display DS4
	PORTA->PCR[9] = ( 0|PORT_PCR_MUX(0x01) );  // display DS3
	PORTA->PCR[10] = ( 0|PORT_PCR_MUX(0x01) ); // display DS2
	PORTA->PCR[11] = ( 0|PORT_PCR_MUX(0x01) ); // display DS1

	/* Set corresponding PORTA port pins as outputs */
	PTA->PDDR = GPIO_PDDR_PDD( 0x0F00 );  // "1" configures given pin as an output

	NVIC_DisableIRQ(31);  // Disable the eventual generation of the interrupt caused by the control button

	/* Set corresponding PORTB pins for GPIO functionality */
	PORTB->PCR[0] = ( 0|PORT_PCR_MUX(0x01) );   // seg A
	PORTB->PCR[1] = ( 0|PORT_PCR_MUX(0x01) );   // seg B
	PORTB->PCR[2] = ( 0|PORT_PCR_MUX(0x01) );   // seg C
	PORTB->PCR[3] = ( 0|PORT_PCR_MUX(0x01) );   // seg DP
	PORTB->PCR[8] = ( 0|PORT_PCR_MUX(0x01) );   // seg D
	PORTB->PCR[9] = ( 0|PORT_PCR_MUX(0x01) );   // seg E
	PORTB->PCR[10] = ( 0|PORT_PCR_MUX(0x01) );  // seg F
	PORTB->PCR[11] = ( 0|PORT_PCR_MUX(0x01) );  // seg G

	/* Set corresponding PORTB port pins as outputs */
	PTB->PDDR = GPIO_PDDR_PDD( 0x0F0F ); // "1" configures given pin as an input
	PORTB->PCR[4] = ( 0 | PORT_PCR_ISF(1) | PORT_PCR_IRQC(0x0A) | PORT_PCR_MUX(0x01) |
					    PORT_PCR_PE(1) | PORT_PCR_PS(1)); // display SW1

	/* Let's clear any previously pending interrupt on PORTB and allow its subsequent generation */
	NVIC_ClearPendingIRQ(31);
	NVIC_EnableIRQ(31);
}


/* Service routine invoked upon the press of a control button */
void PORTB_IRQHandler( void )
{
	delay(100);

	if (PORTB->ISFR & GPIO_PDIR_PDI(PB4_ISF_MASK)) {
		if (!(PTB->PDIR & GPIO_PDIR_PDI(PB4_ISF_MASK))) {
			/*Button is clicked, turn off interrupts form RTC so display can fully work */
			NVIC_DisableIRQ(RTC_IRQn);

			/* Set value to show or hide display */
			show_display = !show_display;
			/* Set up time modes for each digits, after value 3 set time mode has ended */
			if(mode <= 3)
				mode += 1;

			/* Turn on RTC interrupts again */
			NVIC_ClearPendingIRQ(RTC_IRQn);
			NVIC_EnableIRQ(RTC_IRQn);
		}
		PORTB->PCR[4] |= PORT_PCR_ISF(0x01);  // Confirmation of interrupt after button press
	}
}


/* Single digit shown on a particular section of the display  */
void sn(int number, uint32_t display) {

	uint32_t n;

	switch (number) {
	case 0:
		n = N0; break;
    case 1:
    	n = N1; break;
    case 2:
    	n = N2; break;
    case 3:
    	n = N3; break;
    case 4:
    	n = N4; break;
    case 5:
    	n = N5; break;
    case 6:
    	n = N6; break;
    case 7:
    	n = N7; break;
    case 8:
    	n = N8; break;
    case 9:
    	n = N9; break;
    default:
    	n = N0;
  }

	/* Show dot after the second number of hours (between hours and minutes) */
	if (display == D2)
		n |= MASK_DOT_ON;
	else
		n &= MASK_DOT_OFF;

	PTA->PDOR = GPIO_PDOR_PDO(display);
	PTB->PDOR = GPIO_PDOR_PDO(n);

	delay(10);
	off();
}

/* Put microcontroller into low-energy mode when the display is off */
void wait_mode() {
    /* Disable all interrupts */
    __disable_irq();

    /* Enter Wait mode - low energy mode */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;		// Sets System Control Register bit into SleepDeep Mode
    __WFI();  								// Wait for interrupt (assembly command), low energy mode

    /* Enable interrupts back again */
    __enable_irq();
}

/* Show segment of a display */
void display_time() {
	PTA->PDOR = GPIO_PDOR_PDO(0x0000);
	PTB->PDOR = GPIO_PDOR_PDO(0x0000);

	if(mode <= 3) {
		/* Set up time mode, setting up time digit by digit */
		if(mode >= 0)
			sn(hours1, D1);				// dozens of hours
		if(mode >= 1)
			sn(hours0, D2);				// units of hours
		if(mode >= 2)
			sn(minutes1, D3);
		if(mode >= 3)
			sn(minutes0, D4);
	}

	/* After set up time mode ended if show_display is 1 (set up by the PORTB interrupt) light up LED display */
	/* mode > 4 means that seting time mode has ended and RTC->TAR is set to correct value */
	if(show_display && mode > 4) {
		/* Show all segments of LED display - explained in documentation*/
		sn(minutes0, D4);
		sn(minutes1, D3);
		sn(hours0, D2);
		sn(hours1, D1);
	}

	/* If the display is not up, set microcontroller into low energy mode */
	if(!show_display && mode > 4) {
		wait_mode();
	}
}

/* Initialize RTC module - RTC module counts time and after certain amount of seconds it triggers an interrupt */
void RTC_Init( void ) {
		/* Enable the RTC clock gate */
	    SIM->SCGC6 |= SIM_SCGC6_RTC_MASK;

	    RTC->SR &= ~RTC_SR_TCE_MASK;

	    //* Enable software access to RTC registers */
	    RTC->CR |= RTC_CR_SWR_MASK;
	    RTC->CR &= ~RTC_CR_SWR_MASK;

	    /* Reset TCR */
	    RTC->TCR = 0x0000;

	    /* Enable the oscillator and wait for stabilization */
	    RTC->CR |= RTC_CR_OSCE_MASK;
	    while (!(RTC->SR & RTC_SR_TIF_MASK)) {}

	    RTC->SR &= ~RTC_SR_TIF_MASK;

	    /* Set the minimal value to TSR register */
	    RTC_TSR = 0;

	    /* Time Alarm Register - now for one second for the setting up time mode
	       After the RTC_IRQHandler sets it up for one minute*/
	    RTC->TAR = 1;

	    /* Enable the Time Alarm Interrupt */
	    RTC->IER |= RTC_IER_TAIE_MASK;

	    NVIC_ClearPendingIRQ(RTC_IRQn);
	    NVIC_EnableIRQ(RTC_IRQn);

	    RTC->SR |= RTC_SR_TCE_MASK;
}

/* Handling the interrupt caused by RTC module also drives the logic of passing the digits between segments*/
void RTC_IRQHandler(void) {
	if(RTC->SR & RTC_SR_TAF_MASK) {
		/* Mode for setting up time ended, change the TAR to 1 minute */
		// Show time mode
		if(mode > 4) {
			RTC_TAR += TIME;				// Add to RTC_TAR value after which it triggers next interrupt
			// After interrupt by RTC update minutes
			minutes0 += 1;

			// Set up conversions between each digit
			if(minutes0 >= 10) {
				minutes0 = 0;
				minutes1++;
			}
			if(minutes1 >= 6) {
				minutes1 = 0;
				hours0++;
			}
			if(hours0 >= 10) {
				hours0 = 0;
				hours1 ++;
			}
			if(hours1 == 2 && hours0 == 4) {
				hours0 = 0;
				hours1 = 0;
			}
			RTC_SR &= ~RTC_SR_TAF_MASK;  // Clear the alarm flag
		}
		else {
			/* Set up time mode */
			/* Digits are changing after one seconds */
			RTC_TAR += 1;

			if(mode == 0)
				hours1++;
			else if(mode == 1)
				hours0++;
			else if(mode == 2)
				minutes1++;
			else if(mode == 3)
				minutes0++;

			/* Handel conversions between digits */
			if(minutes0 >= 10)
				minutes0 = 0;
			if(minutes1 >= 6)
				minutes1 = 0;
			if(hours0 >= 10 || (hours1 == 2 && hours0 >= 4))
				hours0 = 0;
			if(hours1 >= 3)
				hours1 = 0;
		}
		/* Set up time mode has ended, I need to update the RTC->TAR so it triggers interrupt after correct time*/
		if(mode == 4) {
			RTC->TAR += TIME - 1;		// add value of TIME (60) to RTC->TAR, because there is already value of 1 I need to subtract it
			mode += 1;
		}
	}

}


int main(void) {
    ports_init();
    RTC_Init();

    for (;;) {
    	display_time();
    }

    return 0;
}


////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
