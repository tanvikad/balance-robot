#ifndef SPI_H
#define SPI_H

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
#define CTRL1_XL 0b00010000
#define CTRL2_G 0b00010001
#define CTRL7_G 00010110





////////////////////////////////////////////////
// Pin Names
////////////////////////////////////////////////
#define FPGA_LOAD_PIN PA5
#define FPGA_RESET_PIN PA6

#define IMU_LOAD_PIN PB6
#define IMU_READ_ADDRESS 0b10000000

#define DEBUG_LED_PIN_1 PA9
#define DEBUG_LED_PIN_2 PA10
#define DEBUG_LED_PIN_3 PA11    


////////////////////////////////////////////////
// IMU Values
////////////////////////////////////////////////

#define ACCEL_SCALE_2G 0.061
#define ACCEL_SCALE_4G 0.122
#define GYRO_SCALE_125 4.375
#define GYRO_SCALE_250 8.75







////////////////////////////////////////////////
// Function Prototypes
////////////////////////////////////////////////

void init();
void loop();


void spin_motor(char, char);
int binaryToDecimal(int);
int decimalToBinary(int);
char set_val(int);
char set_val_helper(bool, int);
void force_reset();

char read_imu(char);
void write_imu(char, char);

int16_t twosComplement_to_int(char, char);
float scale_accel(int16_t);
float get_angle(int16_t);



void waiting(int i, struct imu_values* values);

void after_waiting(struct imu_values * values, struct controller* c);

void print_float(float f);

float sigmoid(float x);



#endif