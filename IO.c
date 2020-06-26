////newwwww// IO.c
// This software configures the switch and LED
// You are allowed to use any switch and any LED, 
// although the Lab suggests the SW1 switch PF4 and Red LED PF1
// Runs on LM4F120 or TM4C123
// Program written by: put your names here
// Date Created: October 25, 2017
// Last Modified: 10/31/17
// Lab number: 7


#include "tm4c123gh6pm.h"
#include <stdint.h>

//------------IO_Init------------
// Initialize GPIO Port for a switch and an LED
// PF4 is switch (negative logic) and PF2 is heartbeat
// Input: none
// Output: none
void IO_Init(void) {volatile uint32_t delay;
  SYSCTL_RCGCGPIO_R |= 0x00000020;  // 1) activate clock for Port F
  delay = SYSCTL_RCGCGPIO_R;        // allow time for clock to start
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
  // only PF0 needs to be unlocked, other bits can't be locked
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog on PF
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PF4-0
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 in, PF3-1 out
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) disable alt funct on PF7-0
  GPIO_PORTF_PUR_R = 0x11;          // enable pull-up on PF0 and PF4
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital I/O on PF4-0
}

//------------IO_HeartBeat------------
// Toggle the output state of the  LED.
// Input: none
// Output: none
void IO_HeartBeat(void) {
	if((GPIO_PORTF_DATA_R&0x04) == 0){
		GPIO_PORTF_DATA_R |= 0x04;
	}else{ GPIO_PORTF_DATA_R &= ~0x04;}
}


//------------IO_Touch------------
// wait for release and press of the switch
// Delay to debounce the switch (20 ms)
// Input: none
// Output: none
void IO_Touch(void) {uint32_t volatile time; uint8_t n = 20;
	while((GPIO_PORTF_DATA_R&0x10) == 0x10){} // stay in while until PF4 is low
	while(n){
    time = 72724*2/91;  // 1msec, tuned at 80 MHz
    while(time){
	  	time--;
    }
    n--;
  }                            // delay 1 ms 20 times
 	while((GPIO_PORTF_DATA_R&0x10) == 0x00){} // stay in while until PF4 is high
 // --UUU-- wait for release; delay for 20ms; and then wait for press
}  

