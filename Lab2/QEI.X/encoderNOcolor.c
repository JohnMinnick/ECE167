/*
 *John Minnick
 * ECE167 Lab 2
 * 4/21/2023
 * Quadrature Encoder stuffs
 */

// including and colluding, remind you of anyone else we know?

#include "stdlib.h"
#include "stdio.h"


#include <BOARD.h>
#include <AD.h>
#include <serial.h>
//#include <timers.h>
#include <stdio.h>
#include <QEI.h>
//#include <proc/p32mx340f512h.h>
int temp = 0;

int main(void) {
    BOARD_Init();
    QEI_Init();
    printf("\n \n Starting QEI Main \n \n");

    while (1) {
        for(int i =0; i < 183000; i++){
            
        }
        temp = QEI_GetPosition();
        printf("\n %d\n", temp);
    }

    BOARD_End(); // safety catch 
}




