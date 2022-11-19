/**
    Main: Contains main function for AES SPI communication with FPGA.
    Sends the plaintext and key over SPI to the FPGA and then receives back
    the cyphertext. The cyphertext is then compared against the solution
    listed in Appendix A of the AES FIPS 197 standard.
    @file lab7.c
    @author Josh Brake
    @version 1.0 7/13/2021
*/
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

////////////////////////////////////////////////
// Main
////////////////////////////////////////////////

int main(void) {

  int bin = decimalToBinary(20);
  int dec = binaryToDecimal(bin);
  char cyphertext[16];
  char m1_val;
  char m2_val;
  char imu_wai;
  char imu_yaw_h;
  char imu_yaw_l;
  char temp;
  char z_high;
  char z_low;
  char x_high;
  char x_low;
  // Configure flash latency and set clock to run at 84 MHz

  // Enable GPIOA clock
  RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN);

  // "clock divide" = master clock frequency / desired baud rate
  // the phase for the SPI clock is 1 and the polarity is 0
  initSPI(1, 1, 1);


  // Load and done pins
  pinMode(PA5, GPIO_OUTPUT);  // LOAD
  pinMode(PA6, GPIO_OUTPUT);  // Reset on FPGA
  pinMode(PA9, GPIO_OUTPUT);
  pinMode(PA11, GPIO_OUTPUT);
  pinMode(PB6, GPIO_OUTPUT);
  digitalWrite(PA5, 1);       // set the chip select high when idle.
  digitalWrite(PB6, 1);
  digitalWrite(PA11, 1);
  digitalWrite(PA9, 1);

  // debugging LEDs
  pinMode(PA9, GPIO_OUTPUT);
  pinMode(PA10, GPIO_OUTPUT);
  //digitalWrite(PA9, 0);
  digitalWrite(PA10, 0);
  
  // imu
  // Read from who_am_I, expected value = 01101011
  while(1){
    imu_wai = read_imu((char)0b00001111);
    printf("imu returned %d \n", imu_wai);
    
    write_imu((char) CTRL1_XL, (char)0b01010000);
    
    z_high = read_imu((char) OUTZ_H_A);
    printf("IMU z high returned %d \n", z_high);
    z_low = read_imu((char) OUTZ_L_A);
    printf("IMU z low returned %d \n", z_low);

    x_high = read_imu((char) OUTX_H_A);
    printf("IMU x high returned %d \n", x_high);
    x_low = read_imu((char) OUTX_L_A);
    printf("IMU x low returned %d \n", x_low);
    
    //write_imu((char)(0b00011000), (char)(0b11100010));  
    //temp = read_imu((char)(0b00011000));

    //printf("configuration 9 %d \n", temp);

    //write_imu((char) FIFO_CTRL3, (char)0b00010001);  

    //imu_yaw_h = read_imu((char)OUTZ_H_G);
    //printf("imu yaw h returned %d \n", imu_yaw_h);
    
    
    //imu_yaw_l = read_imu((char)OUTZ_L_G);
    //printf("imu yaw l returned %d \n", imu_yaw_l);



  }


  // // configure motor
  // m1_val = set_val(-40);
  // printf("m1 val is %d", m1_val);
  // m2_val = set_val(40);
  // spin_motor(m1_val, m2_val);
  

  // for (int j = 0; j < 20; j++)
  // {
  //   digitalWrite(PA9, j%2);
  //   //force_reset();
  //   spin_motor(m1_val, m2_val);
  //   for (int i = 0; i < 200000; i++) 
  //     ;
  //   //force_reset();
  //   spin_motor(m2_val, m1_val);
  //   for (int i = 0; i < 200000; i++) 
  //     ;
  // }
  // char final_val = set_val(100);
  // spin_motor(final_val, final_val);
}

////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////
char read_imu(char address) {
  char imu_response;
  address |= 0b10000000;
  printf("trying to read from %d \n", address);
  //digitalWrite(PB6, 0);
  digitalWrite(PA5, 0);
  digitalWrite(PA11, 0);
  digitalWrite(PA9, 0);
  digitalWrite(PB6, 0);
  imu_response = spiSendReceiveTwoChar(address, 0b00000000);
  while(SPI1->SR & SPI_SR_BSY);
  //digitalWrite(PB6, 1);
  digitalWrite(PA11, 1);
  digitalWrite(PA5, 1);
  digitalWrite(PA9, 1);
  digitalWrite(PB6, 1);
  return imu_response;
}

void write_imu(char address, char write) {
  char imu_response;
  printf("trying to write to %d with write block %d \n", address, write);
  digitalWrite(PA5, 0);
  digitalWrite(PA11, 0);
  digitalWrite(PA9, 0);
  digitalWrite(PB6, 0);
  imu_response = spiSendReceiveTwoChar(address, write);
  while(SPI1->SR & SPI_SR_BSY);
  //digitalWrite(PB6, 1);
  digitalWrite(PA11, 1);
  digitalWrite(PA5, 1);
  digitalWrite(PA9, 1);
  digitalWrite(PB6, 1);
  //return imu_response;
}

void spin_motor(char m1_val, char m2_val) {
  int i; 
  digitalWrite(PA5, 0);
  spiSendReceiveTwoChar(m1_val, m2_val);
  while(SPI1->SR & SPI_SR_BSY);
  digitalWrite(PA5, 1);
}

int binaryToDecimal(int n)
{
    int num = n;
    int dec_value = 0;
 
    // Initializing base value to 1, i.e 2^0
    int base = 1;
 
    int temp = num;
    while (temp) {
        int last_digit = temp % 10;
        temp = temp / 10;
 
        dec_value += last_digit * base;
        base = base * 2;
    }
    printf("the decimal number is %d\n", dec_value);
    return dec_value;
}

int decimalToBinary(int n)
{
    // array to store binary number
    int binaryNum = 0;
  
    // counter for binary array
    int base = 1;
    int i = 0;
    while (n > 0) {
        // storing remainder in binary array
        binaryNum += (n % 2) * base;
        n = n / 2;
        base = base * 10;
        i++;
        
        if(i >= 7) break;
    }
    // printing binary array in reverse order
    printf("the binary number is %d \n", binaryNum);
    return binaryNum;
}

char set_val(int value){
  if (value < 0){
    return set_val_helper(1, value*-1);
  }
  else{
    return set_val_helper(0, value);
  }
}

char set_val_helper(bool reverse, int value){
  char motor_output = (char) value;
  motor_output |= reverse << 7;
  return motor_output;
}

void force_reset(){
  digitalWrite(PA6, 1);
  //delaying between the pin input
  for(int i = 0; i < 200000; i++);
  digitalWrite(PA6, 0);
}
