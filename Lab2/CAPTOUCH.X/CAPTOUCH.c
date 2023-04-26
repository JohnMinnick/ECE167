/*
 John Minnick
 4/26/2023
 ECE167
 Lab 2 
 CAPTOUCH.c
 */

// https://gist.github.com/bmccormack/d12f4bf0c96423d03f82 was referenced for moving average filter code

#include <CAPTOUCH.h>
#include <stdio.h>

#define CAP_TOUCH_LIMIT 750 // need to dial in this value
#define BUFFERLENGTH 16 // for moving average 


typedef enum {
    RISINGEDGE = 0,
    FALLINGEDGE
} SQR_WAVE_STATES;

SQR_WAVE_STATES currentState;

int buff[BUFFERLENGTH] = {0}; // init buffer with zero entry 

double time_Start;
double time_End;
double time_Period;

int avg;
int count;
int signal_Freq;

char CAPTOUCH_Init(void) {
    // following block inits the timer
    T2CON = 0;
    T2CONbits.TCKPS = 0b011;
    PR2 = 0xFFFF;
    T2CONbits.ON = 1;

    //this block inits input capture
    IC4CON = 0;
    IC4CONbits.ICTMR = 1;
    IC4CONbits.ICM = 0b010;

    IFS0bits.IC4IF = 0;
    IPC4bits.IC4IP = 7;
    IEC0bits.IC4IE = 1;
    IC4CONbits.ON = 1;
    // whatever else you need to do to initialize your module
    // ok :)
    currentState = RISINGEDGE; // init RISINGEDGE
    avg = 0; // init 0
    count = 0; // init 0

}

char CAPTOUCH_IsTouched(void) {
    if (avg < CAP_TOUCH_LIMIT) { // limit check 
        return TRUE;
    }
    return FALSE;
}

void __ISR(_INPUT_CAPTURE_4_VECTOR) InputCapture_Handler(void) {
    IFS0bits.IC4IF = 0;
    // IC4BUF contains the timer value when the rising edge occurred.
    //state machine goes here
    switch (currentState) {
        case RISINGEDGE:
            time_Start = IC4BUF; // read IC4BUF into start time
            currentState = FALLINGEDGE; // state swap
            break;
        case FALLINGEDGE:
            time_End = IC4BUF; // read/capture IC4BUF into end time 
            time_Period = time_End - time_Start; // calculate period length ||| NOTE: may need to be scaled 
            time_Period = time_Period * 0.0000002; // scalar for period conversion
            // need to handle incorrect readings edge case
            if (0 > time_Period) {
                time_Start = time_End; // reset start time to most recent time poll;
                currentState = RISINGEDGE; // state swap
                break; // exit switch 
            } else { // probably can do without the ELSE but shouldnt make any functionality diff
                signal_Freq = 1 / time_Period; // calculate freq
                //vvv  implement lil moving average filter code here vvv
                MovingAvgCalc(signal_Freq, BUFFERLENGTH, &avg, &buff, &count); // updates AVG internally, no need to return -> kinda weird but i got help w/ this 
                time_Start = time_End; // update start time
                currentState = RISINGEDGE; // state swap
                break;
            }
            break; // extra break to catch but else case should catch all before it hits here
        default:
            printf("\n Hello weary traveler, you look lost. \n How did you end up here? \n");

    }
}


// NOTE : this moving average code is modified from https://gist.github.com/bmccormack/d12f4bf0c96423d03f82 with the help of chatGPT
// THIS IS NOT MY OWN ORGINAL WORK, it is my own modification upon moving average code 

int MovingAvgCalc(int val, int buffer_len, int *average, int *buffer_arr, int *counter) {
    buffer_arr[(*counter) % buffer_len] = val / buffer_len;
    *average = *average + (val / buffer_len) - buffer_arr[((*counter) + 1) % buffer_len];
    *counter = *counter + 1;
}



















