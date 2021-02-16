#include <msp430.h> 
#include "peripherals.h"

/**
 * main.c
 */
int main(void)
{
    unsigned char currKey=0,

    initLeds();
    configDisplay();
    configKeypad();

    // *** Intro Screen ***
    Graphics_clearDisplay(&g_sContext); // Clear the display

    // Write some text to the display
    Graphics_drawStringCentered(&g_sContext, "Welcome to Simon", AUTO_STRING_LENGTH, 64, 50, OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext, "Press * to start..", AUTO_STRING_LENGTH, 64, 70, OPAQUE_TEXT);


	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	while(1){
	    currKey = getKey();

	    //start the game
	    if(currKey == "*"){
	        Graphics_flushBuffer(&g_sContext);
	        Graphics_drawStringCentered(&g_sContext, "Game will start in 3", AUTO_STRING_LENGTH, 64, 50, OPAQUE_TEXT);
	    }

	}

	return 0;
}
