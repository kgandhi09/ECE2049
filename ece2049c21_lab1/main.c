/************** ECE2049 DEMO CODE ******************/
/**************  28 March 2020   ******************/
/*********Redesigned by Hamayel Qureshi********/
/***************************************************/

#include <msp430.h>

/* Peripherals.c and .h are where the functions that implement
 * the LEDs and keypad, etc are. It is often useful to organize
 * your code by putting like functions together in files.
 * You include the header associated with that file(s)
 * into the main file of your project. */
#include "peripherals.h"
#include "time.h"
#include <stdlib.h>
#include <stdio.h>

// Function Prototypes
void swDelay(char numLoops);

// Main
void main(void)

{
    unsigned char currKey=0;

    int count = 1;
    char number = 0;
    int i;
    // Define some local variables


    WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer. Always need to stop this!!
                                 // You can then configure it properly, if desired


    // Useful code starts here
    initLeds();
    configDisplay();
    configKeypad();

    // *** Intro Screen ***
    Graphics_clearDisplay(&g_sContext); // Clear the display

//    // Write some text to the display
//    Graphics_drawStringCentered(&g_sContext, "Welcome to Simon", AUTO_STRING_LENGTH, 64, 50, OPAQUE_TEXT);
//    Graphics_drawStringCentered(&g_sContext, "Press * to start...", AUTO_STRING_LENGTH, 64, 60, OPAQUE_TEXT);
//    Graphics_drawStringCentered(&g_sContext, "ECE2049-C21!", AUTO_STRING_LENGTH, 64, 70, OPAQUE_TEXT);

    // Draw a box around everything because it looks nice
    Graphics_Rectangle box = {.xMin = 3, .xMax = 125, .yMin = 3, .yMax = 125 };
    Graphics_drawRectangle(&g_sContext, &box);

    // We are now done writing to the display.  However, if we stopped here, we would not
    // see any changes on the actual LCD.  This is because we need to send our changes
    // to the LCD, which then refreshes the display.
    // Since this is a slow operation, it is best to refresh (or "flush") only after
    // we are done drawing everything we need.


    Graphics_flushBuffer(&g_sContext);

    char arr[100] = 0;
    char gameInput[100] = 0;

    while (1)    // Forever loop
    {
        // Check if any keys have been pressed on the 3x4 keypad



        switch(currKey){
        case 0:
            Graphics_drawStringCentered(&g_sContext, "SIMON", AUTO_STRING_LENGTH, 64, 50, OPAQUE_TEXT);
            Graphics_flushBuffer(&g_sContext);
            currKey = getKey();
            break;

        case '*':
            Graphics_clearDisplay(&g_sContext); // Clear the display
            Graphics_drawStringCentered(&g_sContext, "3", AUTO_STRING_LENGTH, 64, 50, OPAQUE_TEXT);
            Graphics_flushBuffer(&g_sContext);
            swDelay(1);

            Graphics_clearDisplay(&g_sContext); // Clear the display
            Graphics_drawStringCentered(&g_sContext, "2", AUTO_STRING_LENGTH, 64, 50, OPAQUE_TEXT);
            Graphics_flushBuffer(&g_sContext);
            swDelay(1);

            Graphics_clearDisplay(&g_sContext); // Clear the display
            Graphics_drawStringCentered(&g_sContext, "1", AUTO_STRING_LENGTH, 64, 50, OPAQUE_TEXT);
            Graphics_flushBuffer(&g_sContext);
            swDelay(1);
            Graphics_clearDisplay(&g_sContext); // Clear the display
            swDelay(2);
            currKey = 1;
            break;

        case 1:
            number = ((rand()%3) + 1) + '0';
            arr[count-1] = number;
            arr[count] = '\0';

            Graphics_drawStringCentered(&g_sContext, arr, AUTO_STRING_LENGTH, 64, 50, OPAQUE_TEXT);
            Graphics_flushBuffer(&g_sContext);
            swDelay(2);
            Graphics_clearDisplay(&g_sContext); // Clear the display
            swDelay(1);

            for(i = 0; i < count; i++){
                while(getKey() == 0){
                    gameInput[i] = getKey();
                }
                gameInput[i+1] = '\0';
                Graphics_drawString(&g_sContext, gameInput, AUTO_STRING_LENGTH, 64, 50, OPAQUE_TEXT);
                Graphics_flushBuffer(&g_sContext);
                swDelay(2);
                Graphics_clearDisplay(&g_sContext); // Clear the display


            }
            currKey = 1;
            count++;
            break;


        }
    }
}


void swDelay(char numLoops)
{
	// This function is a software delay. It performs
	// useless loops to waste a bit of time
	//
	// Input: numLoops = number of delay loops to execute
	// Output: none
	//
	// smj, ECE2049, 25 Aug 2013
    // hamayel qureshi, 28 march 2020

	volatile unsigned int i,j;	// volatile to prevent removal in optimization
			                    // by compiler. Functionally this is useless code

	for (j=0; j<numLoops; j++)
    {
    	i = 50000 ;					// SW Delay
   	    while (i > 0)				// could also have used while (i)
	       i--;
    }
}
