/*      Author: Mahdi Aouchiche
 *      Partner(s) Name: 
 *      Lab Section: 22
 *      Assignment: Lab # 10  Exercise # 1 
 *      Exercise Description: [optional - include for your own benefit]
 *
 *      I acknowledge all content contained herein, excluding template or example
 *      code, is my own original work.
 */

// Demo video: https://drive.google.com/open?id=1DSwcJ6L18qF9RlEfTwmjcQ2Q15aswJN9

#include "timer.h"
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif


static unsigned char threeLEDs; 		// updated by ThreeLEDsSM
static unsigned char blinkingLED;		// updated by BlinkingLEDSM

enum TL_States {TL_start, LEDs} TL_state;
void ThreeLEDsSM(){
	// Transitions
	switch(TL_state) {
		case TL_start:
			threeLEDs = 0x01;	// init threeLeds to 0.
			TL_state = LEDs;	// transtion to state LEDs
			break;
		case LEDs:
			threeLEDs = (threeLEDs == 0x04) ? 0x01 : threeLEDs << 1;
			TL_state = LEDs; 	// loop back to the same state  
			break;
		default:
			TL_state = TL_start;
			break;	
	}

	// State actions
	switch(TL_state) {
		case TL_start:
			break;
		case LEDs:
			break;
		default:
			break;
	} 
}

enum BL_States {BL_start, LED} BL_state;
void BlinkingLEDSM(){
	// Transitions
	switch(BL_state) {
		case BL_start:
			blinkingLED = 0x00;
			BL_state = LED;
			break;
		case LED:
			blinkingLED = ~blinkingLED;
			BL_state = LED; 
			break;
	}

	// State actions
	switch(BL_state) {
		case BL_start:
			break;
		case LED: 
			break;
		default:
			break;
	} 
}

unsigned char output;	// output to port B 
enum CL_States {CL_start, ALL_LEDs} CL_state;
void CombineLEDsSM(){
	// Transitions
	switch(CL_state) {
		case CL_start:
			output = 0x01;
			CL_state = ALL_LEDs;
			break;
		case ALL_LEDs:
			CL_state = ALL_LEDs;
			break;
		default:
			CL_state = CL_start;
			break;	
	}

	// State actions
	switch(CL_state) {
		case CL_start:
			break;
		case ALL_LEDs:
			output = ((threeLEDs | (blinkingLED & 0x01) << 3)) & 0x0F; 
			break;
		default:
			break;	
	} 
}

int main () {
        /* Insert DDR and PORT initializations */
    	DDRB = 0xFF; PORTB = 0x00;  	// output

        TimerSet(1000);         	// set period to 1 second
        TimerOn();              	// turn the timer on
	
	/* Initial states */
	TL_state = TL_start;
	BL_state = BL_start;
	CL_state = CL_start;
	output = 0x01;

	while(1) {
		/* User code (i,e synchSM calls) */
		ThreeLEDsSM(); 
		BlinkingLEDSM();
		CombineLEDsSM();

                PORTB = output;         // output to port B
        
		while (!TimerFlag) ;	// Wait 1 sec
                TimerFlag = 0;          // lower the flag
        }
        return 1;
}
