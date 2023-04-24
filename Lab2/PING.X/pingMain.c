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

unsigned int printer;


int main(void) {
    BOARD_Init();
    PING_Init();
    printf("\n BEGIN \n");
    while(1){
        for(int i = 0; i < 183000; i++){
          
        }
          printer = PING_GetTimeofFlight();
          printf("\n %d \n", printer);
    }
    
    
    BOARD_End();
}