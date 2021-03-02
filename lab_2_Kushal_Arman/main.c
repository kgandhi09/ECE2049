#include <msp430.h>
#include "peripherals.h"
#include "time.h"
#include <stdlib.h>
#include <stdio.h>

// Function Prototypes
int check(char array1[], char array2[]);
void runTimerA2();
void stopTimerA2();
void configureRegsiters();

long unsigned int timer=0;
long unsigned int leapCount = 0;
int temp = 5;

void configureRegisters(){
    UCSCTL0 = 0x14B8;
    UCSCTL1 = 0x0020;
    UCSCTL2 = 0x101F;
    UCSCTL3 = 0x0000;
    UCSCTL4 = 0x0044;
    UCSCTL5 = 0x0000;
    UCSCTL6 = 0xC1CD;
    UCSCTL7 = 0x0403;
    UCSCTL8 = 0x0707;
}

// Main
void main(void)

{
    configureRegisters();
    char temp_curr = 0;
    unsigned char currKey=0;        //declaring an unsigned char currKey with value 0 of size of 1 bytes
    int clear_flag = 1;                  //declaring an integer count with value 1 of size 2 bytes
    char number = 0;                //declaring a char number with value 0 of size of 1 bytes
    int i;                          //initializing an integer i
    unsigned char dispThree[3];     ////declaring an unsigned char array dispThree with 3 elements of size of 3 bytes

    _BIS_SR(GIE);                   //enable global interrupt

    //setting 1st and 3rd element of dispThree array
    dispThree[0] = ' ';
    dispThree[2] = ' ';


    WDTCTL = WDTPW | WDTHOLD;
    P1SEL = P1SEL & ~BIT0;          // Select P1.0 for digital IO
    P1DIR |= BIT0;                  // Set P1.0 to output direction
    P4SEL = P4SEL & ~BIT7;          // Select P4.0 for digital IO
    P4DIR |= BIT7;                  // Set P4.0 to output direction
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
    Graphics_flushBuffer(&g_sContext);

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
                temp_curr = currKey;
                currKey = 'c';
            }
            else{
                currKey = '*';
            }
            break;
        //countdown state
        case 'c':
            currKey = temp_curr;
            char i = 3;
            while(1){
                runTimerA2();
                if(timer>32600){
                    Graphics_clearDisplay(&g_sContext);
                    Graphics_drawStringCentered(&g_sContext, i , AUTO_STRING_LENGTH, 64, 50, OPAQUE_TEXT);
                    Graphics_flushBuffer(&g_sContext);
                    stopTimerA2(1);
                }
                i--;
            }
        }
    }
}

void runTimerA2(void) {
    // This function configures and starts Timer A2
    TA2CTL = TASSEL_1 + MC_1 + ID_0;
    TA2CCR0 = 163;       // 1 second
    TA2CCTL0 = CCIE;     // TA2CCR0 interrupt enabled
}

void stopTimerA2(int reset) {   // This function stops Timer A2 andresets the global time variable smj
    TA2CTL = MC_0;              // stop timer
    TA2CCTL0 &= ~CCIE;          // TA2CCR0 interrupt disabled
    if(reset)
        timer=0;
}

#pragma vector=TIMER2_A0_VECTOR
__interrupt void TimerA2_ISR (void) {
    if(leapCount < 1024){
        timer++;
        leapCount++;
    }
    else{
        leapCount = 0;
    }
}

