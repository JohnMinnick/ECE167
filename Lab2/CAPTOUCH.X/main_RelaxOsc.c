/*
 John Minnick
 * 4/24/23
 
 */


#include <BOARD.h>
#include <AD.h>
#include <serial.h>
#include <timers.h>
#include <stdio.h>

#include "CAPTOUCH.h"


int bool_cap;

int main(void) {
    BOARD_Init();
    CAPTOUCH_Init();

    printf("\n Starting CAPTOUCH Test \n");
    
    while (1) {
        for(int i = 0; i < 10000; i++){
            
        }
        bool_cap = CAPTOUCH_IsTouched(); // rcheck moving avg for touch 
        if(bool_cap == TRUE){
            printf("\n TOUCH \n");
        }else{
            printf("\n NO TOUCH \n");
        }
    }

    BOARD_End();
}