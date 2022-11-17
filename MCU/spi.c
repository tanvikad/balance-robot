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

  // Configure flash latency and set clock to run at 84 MHz

  // Enable GPIOA clock
  RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN);

  // "clock divide" = master clock frequency / desired baud rate
  // the phase for the SPI clock is 1 and the polarity is 0
  initSPI(1, 1, 1);


  // Load and done pins
  pinMode(PA5, GPIO_OUTPUT);  // LOAD
  pinMode(PA6, GPIO_OUTPUT);  // Reset on FPGA
  pinMode(PB6, GPIO_OUTPUT);
  digitalWrite(PA5, 1);       // set the chip select high when idle.
  digitalWrite(PB6, 1);

  // debugging LEDs
  pinMode(PA9, GPIO_OUTPUT);
  pinMode(PA10, GPIO_OUTPUT);
  digitalWrite(PA9, 0);
  digitalWrite(PA10, 0);
  
  // imu
  // Read from who_am_I, expected value = 01101011
  while(1){
    imu_wai = read_imu((char)0b00001111);
    printf("imu returned %d \n", imu_wai);
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
  imu_response = spiSendReceiveTwoChar(address, 0b00000000);
  while(SPI1->SR & SPI_SR_BSY);
  //digitalWrite(PB6, 1);
  digitalWrite(PA5, 1);
  printf("direct imu reply %d \n", imu_response);
  return imu_response;
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
