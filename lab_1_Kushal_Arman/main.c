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
    int count = 1;                  //declaring an integer count with value 1 of size 2 bytes
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

        //Welcom screen state which waits for '*' input from keypad
        case 0:
            Graphics_drawStringCentered(&g_sContext, "SIMON", AUTO_STRING_LENGTH, 64, 50, OPAQUE_TEXT);
            Graphics_flushBuffer(&g_sContext);
            currKey = getKey();
            break;

        //Countdown state, once '*' is pressed, LCD display 3..2..1.. with delay of 1 second(s)
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


         //Main game play state
        case 1:
            number = ((rand()%4) + 1) + '0'; //generating a random number between 1-4

            arr[count-1] = number; // storing randomly generated number in the arr[]
            arr[count] = '\0';     // storing a null character at the end to determine end of the string


            // a while loop to diplay one value at one time with spatial allignment
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


            //while loop to take the user input, light up LED, make the buzzer sound and display the numbers input
            int i = 0;
            while(1){
                currKey = getKey();
                if(currKey){
                    //code reused from previous lab for LED and Buzzer sound.
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

                    //checking algorithm
                    if(gameInput[i] != arr[i]){ // if pressed a wrong input then generated
                        currKey = 2;   //end screen state
                        break;
                    }
                    else{ //else
                        currKey = 1; //current state - main game state
                    }

                    //display the value with spatial alignment
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

        //Emd game state which displays a humility message, and naturally transit to the welcome screen/ init state of the game.
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
