#include <stdio.h>
#include <stm32l432xx.h>
#include "lib/STM32L432KC.h"
#include <stdbool.h>
#include <math.h>
#include <time.h>

////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////
#define MCK_FREQ 100000



////////////////////////////////////////////////
// Constants for IMU
////////////////////////////////////////////////
#define WHO_AM_I 0b00001111
#define OUTX_L_G 0b00100010
#define OUTX_H_G 0b00100011
#define OUTY_L_G 0b00100100
#define OUTY_H_G 0b00100101
#define OUTZ_L_G 0b00100110
#define OUTZ_H_G 0b00100111
#define OUTX_L_A 0b00101000
#define OUTX_H_A 0b00101001
#define OUTY_L_A 0b00101010
#define OUTY_H_A 0b00101011
#define OUTZ_L_A 0b00101100
#define OUTZ_H_A 0b00101101
#define FIFO_CTRL3 0b00001001


////////////////////////////////////////////////
// Function Prototypes
////////////////////////////////////////////////

void spin_motor(char, char);
int binaryToDecimal(int);
int decimalToBinary(int);
char set_val(int);
char set_val_helper(bool, int);
void force_reset();

char read_imu(char);
void write_imu(char, char);