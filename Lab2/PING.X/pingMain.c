/*
 John Minnick
 * 4/24/23
 
 */


#include <BOARD.h>
#include <AD.h>
#include <serial.h>
#include <timers.h>
#include <stdio.h>
#include <PING.h>
#include <ToneGeneration.h>

unsigned int printer;

int main(void) {
    BOARD_Init();
    PING_Init();
    ToneGeneration_Init();
    ToneGeneration_ToneOn();
    printf("\n BEGIN \n");
    while (1) {
        for (int i = 0; i < 183000; i++) {}
        printer = PING_GetTimeofFlight();
        printf("\n ADC Reading: %d \n", printer);
        printer = PING_GetDistance();
        printf("\n Distance Reading (cm): %d \n", printer);
        if(printer > 1000){
            printer = 1000;
        } else if (printer < 0){
            printer = 0;
        }
        ToneGeneration_SetFrequency(printer);
    }


    BOARD_End();
}