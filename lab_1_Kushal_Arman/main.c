#include <msp430.h>
#include "peripherals.h"
#include "time.h"
#include <stdlib.h>
#include <stdio.h>

// Function Prototypes
void swDelay(char numLoops);
int check(char array1[], char array2[]);



// Main
void main(void)

{
    unsigned char currKey=0;
    int count = 1;
    char number = 0;
    int i;
    unsigned char dispThree[3];
    dispThree[0] = ' ';
    dispThree[2] = ' ';
    // Define some local variables

    WDTCTL = WDTPW | WDTHOLD;
    P1SEL = P1SEL & ~BIT0;          // Select P1.0 for digital IO
    P1DIR |= BIT0;                  // Set P1.0 to output direction
    P4SEL = P4SEL & ~BIT7;          // Select P1.0 for digital IO
    P4DIR |= BIT7;                  // Set P1.0 to output direction
    __disable_interrupt();// Stop watchdog timer. Always need to stop this!!
                                 // You can then configure it properly, if desired
    // Useful code starts here
    initLeds();
    configDisplay();
    configKeypad();

    // *** Intro Screen ***
    Graphics_clearDisplay(&g_sContext); // Clear the display

    // Draw a box around everything because it looks nice
    Graphics_Rectangle box = {.xMin = 3, .xMax = 125, .yMin = 3, .yMax = 125 };
    Graphics_drawRectangle(&g_sContext, &box);

    // We are now done writing to the display.  However, if we stopped here, we would not
    // see any changes on the actual LCD.  This is because we need to send our changes
    // to the LCD, which then refreshes the display.
    // Since this is a slow operation, it is best to refresh (or "flush") only after
    // we are done drawing everything we need.

    Graphics_flushBuffer(&g_sContext);

    char arr[30] = 0;
    char gameInput[30] = 0;

    while (1)    // Forever loop
    {
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
            number = ((rand()%4) + 1) + '0';
            arr[count-1] = number;
            arr[count] = '\0';

            int j = 0;
            while(1){
                dispThree[1] = arr[j];
                if(arr[j] == '1'){
                    Graphics_drawStringCentered(&g_sContext, dispThree, 3, 14, 50, OPAQUE_TEXT);
                    Graphics_flushBuffer(&g_sContext);
                    swDelay(1);
                    Graphics_clearDisplay(&g_sContext);
                }
                if(arr[j] == '2'){
                    Graphics_drawStringCentered(&g_sContext, dispThree, 3, 32, 50, OPAQUE_TEXT);
                    Graphics_flushBuffer(&g_sContext);
                    swDelay(1);
                    Graphics_clearDisplay(&g_sContext);
                }
                if(arr[j] == '3'){
                    Graphics_drawStringCentered(&g_sContext, dispThree, 3, 64, 50, OPAQUE_TEXT);
                    Graphics_flushBuffer(&g_sContext);
                    swDelay(1);
                    Graphics_clearDisplay(&g_sContext);
                }
                if(arr[j] == '4'){
                    Graphics_drawStringCentered(&g_sContext, dispThree, 3, 80, 50, OPAQUE_TEXT);
                    Graphics_flushBuffer(&g_sContext);
                    swDelay(1);
                    Graphics_clearDisplay(&g_sContext);
                }
                j++;
                if(j>=count){
                    break;
                }
            }
            swDelay(2);
            Graphics_clearDisplay(&g_sContext); // Clear the display
            swDelay(1);

            int i = 0;
            while(1){
                currKey = getKey();
                if(currKey){
                    P1OUT = P1OUT ^ BIT0;
                    P4OUT = P4OUT ^ BIT7;
                    BuzzerOn();
                    swDelay(1);
                    BuzzerOff();
                    P1OUT = P1OUT ^ BIT0;
                    P4OUT = P4OUT ^ BIT7;
                    gameInput[i] = currKey;
                    gameInput[i+1] = '\0';

                    dispThree[1] = gameInput[i];

                    if(gameInput[i] != arr[i]){
                        currKey = 2;
                        break;
                    }
                    else{
                        currKey = 1;
                    }

                    if(dispThree[1] == '1'){
                        Graphics_drawStringCentered(&g_sContext, dispThree, 3, 14, 50, OPAQUE_TEXT); //displaying player input
                        Graphics_flushBuffer(&g_sContext);
                        swDelay(1);
                        Graphics_clearDisplay(&g_sContext);
                    }
                    if(dispThree[1] == '2'){
                        Graphics_drawStringCentered(&g_sContext, dispThree, 3, 32, 50, OPAQUE_TEXT);
                        Graphics_flushBuffer(&g_sContext);
                        swDelay(1);
                        Graphics_clearDisplay(&g_sContext);
                    }
                    if(dispThree[1] == '3'){
                        Graphics_drawStringCentered(&g_sContext, dispThree, 3, 64, 50, OPAQUE_TEXT);
                        Graphics_flushBuffer(&g_sContext);
                        swDelay(1);
                        Graphics_clearDisplay(&g_sContext);
                    }
                    if(dispThree[1] == '4'){
                        Graphics_drawStringCentered(&g_sContext, dispThree, 3, 80, 50, OPAQUE_TEXT);
                        Graphics_flushBuffer(&g_sContext);
                        swDelay(1);
                        Graphics_clearDisplay(&g_sContext);
                    }
                    i++;
                }
                if(i >= count){
                    break;
                }
            }
            swDelay(2);
            count++;
            break;

        case 2:
            Graphics_drawStringCentered(&g_sContext, "Simon Says Loser!", AUTO_STRING_LENGTH, 64, 50, OPAQUE_TEXT); //displaying player input
            Graphics_flushBuffer(&g_sContext);
            swDelay(2);
            Graphics_clearDisplay(&g_sContext);
            char arr[30] = 0;
            char gameInput[30] = 0;
            count = 1;
            currKey = 0;
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

    volatile unsigned int i,j;  // volatile to prevent removal in optimization
                                // by compiler. Functionally this is useless code

    for (j=0; j<numLoops; j++)
    {
        i = 50000 ;                 // SW Delay
        while (i > 0)               // could also have used while (i)
           i--;
    }
}
