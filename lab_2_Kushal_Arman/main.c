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
    unsigned char currKey=0;        //declaring an unsigned char currKey with value 0 of size of 1 bytes
    int clear_flag = 1;                  //declaring an integer count with value 1 of size 2 bytes
    char number = 0;                //declaring a char number with value 0 of size of 1 bytes
    int i;                          //initializing an integer i
    unsigned char dispThree[3];     ////declaring an unsigned char array dispThree with 3 elements of size of 3 bytes


    //setting 1st and 3rd element of dispThree array
    dispThree[0] = ' ';
    dispThree[2] = ' ';


    WDTCTL = WDTPW | WDTHOLD;
    P1SEL = P1SEL & ~BIT0;          // Select P1.0 for digital IO
    P1DIR |= BIT0;                  // Set P1.0 to output direction
    P4SEL = P4SEL & ~BIT7;          // Select P4.0 for digital IO
    P4DIR |= BIT7;                  // Set P4.0 to output direction
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

    char arr[30] = 0;               ////declaring an char array arr with 30 element, all with value 0 of size 30 bytes
    char gameInput[30] = 0;         ////declaring an char array gameInput with 30 element, all with value 0 of size 30 bytes

    while (1)    // Forever loop
    {

        // a switch statement with expression currKey
        switch(currKey){

        //Welcom2 screen state which waits for '*' input from keypad
        case 0:
            Graphics_drawStringCentered(&g_sContext, "Press '*'", AUTO_STRING_LENGTH, 64, 50, OPAQUE_TEXT);
            Graphics_drawStringCentered(&g_sContext, "to begin...", AUTO_STRING_LENGTH, 64, 60, OPAQUE_TEXT);
            Graphics_flushBuffer(&g_sContext);
            currKey = getKey();
            break;

        //List of songs
        case '*':
            if(clear_flag){
                Graphics_clearDisplay(&g_sContext);
            }
            clear_flag = 0;
            Graphics_drawStringCentered(&g_sContext, "1. Song 1", AUTO_STRING_LENGTH, 64, 40, OPAQUE_TEXT);
            Graphics_drawStringCentered(&g_sContext, "2. Song 2", AUTO_STRING_LENGTH, 64, 60, OPAQUE_TEXT);
            Graphics_flushBuffer(&g_sContext);
            currKey = getKey();
            if(currKey == '1' || currKey == '2'){
                currKey = 'c';
            }
            else{
                currKey = '*';
            }
            break;

        }

        //countdown state
        case 'c':

    }
}

