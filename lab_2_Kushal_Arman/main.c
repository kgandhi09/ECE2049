#include <msp430.h>
#include "peripherals.h"
#include "time.h"
#include <stdlib.h>
#include <stdio.h>
#define A 440
#define Bf 466
#define B 494
#define C 523
#define Cs 523
#define Cs 554
#define D 587
#define Ef 622
#define E 659
#define F 698
#define Fs 740
#define G 784
#define Gs 830
#define Af 831
#define A 880

// Function Prototypes
void runTimerA2();
void stopTimerA2();
void configureRegsiters();
void customBuzzerOn(int frequency);
int playNote(int note, float duration);
void swDelay(char numLoops);

long unsigned int timer=0;
long unsigned int leapCount = 0;

// Main
void main(void)

{
    configureRegisters();
    char temp_curr = 0;
    unsigned char currKey=0;        //declaring an unsigned char currKey with value 0 of size of 1 bytes
    unsigned char s1_key=0;
    int clear_flag = 1;                  //declaring an integer count with value 1 of size 2 bytes
    char number = 0;                //declaring a char number with value 0 of size of 1 bytes
    int i;                          //initializing an integer i
    unsigned char dispThree[3];     ////declaring an unsigned char array dispThree with 3 elements of size of 3 bytes
    float elapsed_sec = 0;
    int countdown = 1;
    float tolerance = 0;
    int slow_flag = 1;
    int counter = 0;


    _BIS_SR(GIE);                   //enable global interrupt

    P1SEL = P1SEL & ~BIT0;          // Select P1.0 for digital IO
    P1DIR |= BIT0;                  // Set P1.0 to output direction
    P4SEL = P4SEL & ~BIT7;          // Select P4.0 for digital IO
    P4DIR |= BIT7;                  // Set P4.0 to output direction

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

        //Welcome2 screen state which waits for '*' input from keypad
        case 0:
            Graphics_drawStringCentered(&g_sContext, "Welcome to Boombox'", AUTO_STRING_LENGTH, 64, 35, OPAQUE_TEXT);
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
            Graphics_drawStringCentered(&g_sContext, "1. Happy Birthday", AUTO_STRING_LENGTH, 64, 40, OPAQUE_TEXT);
            Graphics_drawStringCentered(&g_sContext, "2. ", AUTO_STRING_LENGTH, 64, 60, OPAQUE_TEXT);
            Graphics_flushBuffer(&g_sContext);
            currKey = getKey();
            if(currKey == '1' || currKey == '2'){
                temp_curr = currKey;
                clear_flag = 1;
                currKey = 'c';
            }
            else{
                currKey = '*';
            }
            break;

            //countdown state
        case 'c':
            if(clear_flag){
                //                dispThree[1] = '3';
                //                Graphics_clearDisplay(&g_sContext);
                //                Graphics_drawStringCentered(&g_sContext, dispThree , 3, 64, 50, OPAQUE_TEXT);
                Graphics_flushBuffer(&g_sContext);

            }
            clear_flag = 0;
            int i = 0;
            char temp;
            runTimerA2();
            while(1){
                elapsed_sec = (timer*1.1)/163;
                if(elapsed_sec > (countdown) && countdown<4){
                    if(countdown == 1){
                        temp = (countdown+2) + '0';
                        P1OUT = P1OUT ^ BIT0;
                    }
                    if(countdown == 2){
                        temp = countdown + '0';
                        P1OUT = P1OUT ^ BIT0;
                        P4OUT = P4OUT ^ BIT7;
                    }
                    if(countdown == 3){
                        temp = (countdown-2) + '0';
                        P1OUT = P1OUT ^ BIT0;
                        P4OUT = P4OUT ^ BIT7;
                    }
                    dispThree[1] = temp;
                    Graphics_clearDisplay(&g_sContext);
                    Graphics_drawStringCentered(&g_sContext, dispThree , 3, 64, 50, OPAQUE_TEXT);
                    Graphics_flushBuffer(&g_sContext);
                    countdown++;
                }
                if(elapsed_sec>4){
                    P1OUT = P1OUT ^ BIT0;
                    if(temp_curr == '1'){
                        currKey = 'X';
                        clear_flag = 1;
                    }
                    if(temp_curr == '2'){
                        currKey = 'Y';
                        clear_flag = 1;
                    }
                    break;
                }
                i++;
            }
            stopTimerA2(1);
            break;

        case 'X':
            counter++;
            if(clear_flag == 1){
                Graphics_clearDisplay(&g_sContext);
                clear_flag = 0;
            }

            int hbNotes[25] = {D,D,E,D,G,Fs,D,D,E,D,A,G,D,D,B,G,Fs,E,C,C,B,G,A,G,Gs};
            float hbDurations[25] = {0.25, 0.125,0.5,0.5,0.5,1,0.25,0.125,0.5,0.5,0.5,1,0.25,0.125,0.5,0.5,0.5,0.5,0.5,0.125,0.5,0.5,0.5,1,0.125};
            int j = 0;
            int buzzer_flag = 0;
            int count = 0;
            int init_flag = 1;
            unsigned char pause_temp = 0;

            while(1){
                s1_key = getKey();

                if(s1_key == '1' && count%2 == 0){
                    init_flag = 0;
                    stopTimerA2(1);
                    BuzzerOff();
                    s1_key = 0;
                    count++;

                }

                if(s1_key == '1' && count%2 == 1){
                    init_flag = 1;
                    s1_key = 0;
                    count++;
                }

                if(init_flag){
                    if(s1_key == '4'){
                        currKey = '*';
                        clear_flag = 1;
                        tolerance = 0;
                        elapsed_sec = 0;
                        countdown = 1;
                        stopTimerA2(1);
                        BuzzerOff();
                        break;
                    }
                    if(s1_key == '#'){
                        clear_flag = 1;
                        currKey = 0;
                        elapsed_sec = 0;
                        tolerance = 0;
                        countdown = 1;
                        stopTimerA2(1);
                        BuzzerOff();
                        break;
                    }
                    if(s1_key == '2' && slow_flag){
                        tolerance += 1.2;
                        slow_flag = 0;
                        s1_key = 0;
                    }
                    if(s1_key == '3' && slow_flag){
                        if(tolerance == 0){
                            tolerance = 1/(tolerance+1.00001);
                        }
                        else{
                            tolerance = tolerance/((tolerance/4)+1.00001);
                        }
                        slow_flag = 0;
                        s1_key = 0;
                    }
                    if(tolerance > 0){
                        buzzer_flag = playNote(hbNotes[j], hbDurations[j]*tolerance);
                    }
                    else{
                        buzzer_flag = playNote(hbNotes[j], hbDurations[j]);
                    }

                    if(buzzer_flag){
                        j++;
                        slow_flag = 1;
                        buzzer_flag = 0;
                    }
                    if(j > 24){
                        currKey = '*';
                        clear_flag = 1;
                        elapsed_sec = 0;
                        tolerance = 0;
                        countdown = 1;
                        stopTimerA2();
                        BuzzerOff();
                        break;
                    }
                }
            }
        }
    }
}


int playNote(int note, float duration){
    int buzzer_flag  = 0;
    runTimerA2();
    float elapsed_sec = (timer*1.1)/163;
    customBuzzerOn(note);
    if(elapsed_sec > duration){
        BuzzerOff();
        stopTimerA2(1);
        buzzer_flag = 1;
    }
    else{
        buzzer_flag = 0;
    }
    return buzzer_flag;

}


void customBuzzerOn(int frequency)
{
    // Initialize PWM output on P3.5, which corresponds to TB0.5
    P3SEL |= BIT5; // Select peripheral output mode for P3.5
    P3DIR |= BIT5;

    TB0CTL  = (TBSSEL__ACLK|ID__1|MC__UP);  // Configure Timer B0 to use ACLK, divide by 1, up mode
    TB0CTL  &= ~TBIE;                       // Explicitly Disable timer interrupts for safety

    // Now configure the timer period, which controls the PWM period
    // Doing this with a hard coded values is NOT the best method
    // We do it here only as an example. You will fix this in Lab 2.
    TB0CCR0   = (32768/frequency)-1;                    // Set the PWM period in ACLK ticks
    TB0CCTL0 &= ~CCIE;                  // Disable timer interrupts

    // Configure CC register 5, which is connected to our PWM pin TB0.5
    TB0CCTL5  = OUTMOD_7;                   // Set/reset mode for PWM
    TB0CCTL5 &= ~CCIE;                      // Disable capture/compare interrupts
    TB0CCR5   = TB0CCR0/2;                  // Configure a 50% duty cycle
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
