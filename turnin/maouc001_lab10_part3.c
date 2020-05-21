/*      Author: Mahdi Aouchiche
 *      Partner(s) Name: 
 *      Lab Section: 22
 *      Assignment: Lab # 10  Exercise # 3 
 *      Exercise Description: [optional - include for your own benefit]
 *
 *      I acknowledge all content contained herein, excluding template or example
 *      code, is my own original work.
 */

// Demo video: https://drive.google.com/open?id=1QFY_1DZsY6cQ4fhkk2Tl6jfndnmA0JDN 

#include "timer.h"
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif


unsigned char threeLEDs; 	// updated by ThreeLEDsSM
unsigned char blinkingLED;	// updated by BlinkingLEDSM
unsigned char frequency;	// updated by ToggleSpeakerSM
unsigned char A2;		// represents the button/switch PIN A2
unsigned char output;   	// updated by CombineLEDsSM to port B

enum TL_States {TL_start, LEDs} TL_state;
void ThreeLEDsSM(){
        // Transitions
        switch(TL_state) {
                case TL_start:
                        threeLEDs = 0x01;       // init threeLeds to 0.
                        TL_state = LEDs;        // transtion to state LEDs
                        break;
                case LEDs:
                        threeLEDs = (threeLEDs == 0x04) ? 0x01 : threeLEDs << 1;
                        TL_state = LEDs;        // loop back to the same state
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

enum TS_States {TS_start, Toggle} TS_state;
void ToggleSpeakerSM(){
	// Transitions
        switch(TS_state) {
                case TS_start:
                        frequency = 0x01;
                        TS_state = Toggle;
                        break;
                case Toggle:
                        TS_state = Toggle;
                        break;
        }

        // State actions
        switch(TS_state) {
                case TS_start:
                        break;
                case Toggle:
			frequency = (A2) ? ~ frequency : 0x00;
                        break;
                default:
                        break;
        }
}


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
                        output = ( (threeLEDs | (blinkingLED & 0x01) << 3 | (frequency & 0x01) << 4 )  & 0x1F );
                        break;
                default:
                        break;
        }
}


int main () {

	const unsigned long Timer_Period = 2;
	const unsigned long BL_Period    = 1000;
	const unsigned long TL_Period    = 300;
	const unsigned long TS_Period	 = 2;
	unsigned long BL_ElapsedTime     = 0;
	unsigned long TL_ElapsedTime     = 0;
	unsigned long TS_ElapsedTime	 = 0;

        /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;      // input
        DDRB = 0xFF; PORTB = 0x00;      // output

        TimerSet(Timer_Period);         // set period to 100ms
        TimerOn();                      // turn the timer on

        /* Initial states */
        TL_state = TL_start;
        BL_state = BL_start;
        CL_state = CL_start;
	TS_state = TS_start;
        output = 0x01;

        while(1) {
		/*Update input*/
		A2 = ~PINA & 0x04;

                /* User code (i,e synchSM calls) */
		if(TL_ElapsedTime >= TL_Period){
			ThreeLEDsSM();
			TL_ElapsedTime = 0;
		}
		if(BL_ElapsedTime >= BL_Period){
			BlinkingLEDSM();
			BL_ElapsedTime = 0;
		}
		if(TS_ElapsedTime >= TS_Period){
			ToggleSpeakerSM();
			TS_ElapsedTime = 0;
		}

		CombineLEDsSM();

                PORTB = output;         // output to port B

                while (!TimerFlag) ;    // Wait 1 sec
                TimerFlag = 0;          // lower the flag

		TL_ElapsedTime += Timer_Period;
		BL_ElapsedTime += Timer_Period;
		TS_ElapsedTime += Timer_Period;
        }
        return 1;
}

