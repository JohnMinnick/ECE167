/*
 John Minnick
 * 4/24/23
 
 */


#include <BOARD.h>
#include <AD.h>
#include <serial.h>
#include <timers.h>
#include <stdio.h>


unsigned int printer;

int main(void) {
    BOARD_Init();
    AD_Init();
    AD_AddPins(AD_A1);
    while(1){
         for (int i = 0; i < 183000; i++) {}
         printer = AD_ReadADPin(AD_A1);
         //printf("\n %d \n", printer);
         if(printer < 100){
             printf("\n No Touch \n");
         }else if(printer > 100){
             printf("\n Yes Touch \n");
         }
        
    }


    BOARD_End();
}