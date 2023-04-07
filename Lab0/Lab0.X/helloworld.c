/* 
 * File:   helloworld.c
 * Author: 97min
 *
 * Created on April 6, 2023, 3:26 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include "BOARD.h"
#include "SERIAL.h"
#include "AD.h"

/*
 * 
 */

int pot;

int main(void) {
    BOARD_Init();
    printf("Hello World!\n");

    AD_Init();
    
    while(1){
    AD_AddPins(AD_A0);
    pot = AD_ReadADPin(AD_A0);
    printf("%d \n", pot);
    }


    BOARD_End();
    while (1);
}

