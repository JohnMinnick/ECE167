/* 
 * File:   main_test.c
 * Author: 97min
 *
 * Created on May 2, 2023, 10:22 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <BOARD.h>
#include <I2C.h>
#include <timers.h>

#include "BNO055.h"

#define  HZ_50_MOD 20
int Z_in = 0;
float Z_out = 0;
int Y_in = 0;
float Y_out = 0;
int X_in = 0;
float X_out = 0;

/*
 * 
 */
int main(void) {
    BOARD_Init();
    while (!(BNO055_Init())) {
        printf("\n fail imu init \n");
    }

    while (1) {
        if ((TIMERS_GetMilliSeconds() % HZ_50_MOD) == 0) {
            // Z_in = BNO055_ReadAccelZ();
            Z_in = BNO055_ReadGyroZ() + 12;
            Z_out = (float) Z_in / 131.0f;
            // Z_out = Z_in / 131;
            // Z_out = 0.0010 * Z_in + 0.0109; // Z scale = 0.0010 -> Zbias = 0.0109
            //printf("%f\r\n", Z_out);
            //  X_in = BNO055_ReadAccelX();
            X_in = BNO055_ReadGyroX() - 14;
            X_out = (float) X_in / 131.0f;
            // X_out = X_in / 131;
            // X_out = 0.0010 * X_in + 0.0031; // X scale = 0.0010 -> X bias = 0.0031
            // printf("%f\r\n", X_out);
            //  Y_in = BNO055_ReadAccelY();
            Y_in = BNO055_ReadGyroY() - 19;
            Y_out = (float) Y_in / 131.0f;
            // Y_out = Y_in / 131;
            //Y_out = 0.0010 * Y_in + 0.052; // Y scale = 0.0010 -> Y bias = 0.052
            // printf("%f\r\n", Y_out);
            printf("%f %f %f \r\n", X_out, Y_out, Z_out);
            //printf("%f %f %f \r\n", X_out, Y_out, Z_out);
        }
    }


    BOARD_End();
}

