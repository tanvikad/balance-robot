/**
    Main: Contains main function for AES SPI communication with FPGA.
    Sends the plaintext and key over SPI to the FPGA and then receives back
    the cyphertext. The cyphertext is then compared against the solution
    listed in Appendix A of the AES FIPS 197 standard.
    @file lab7.c
    @author Josh Brake
    @version 1.0 7/13/2021
*/

#include "spi.h"




////////////////////////////////////////////////
// Main
////////////////////////////////////////////////

int main(void) {

  init();
  loop();
}

void loop(){
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
  while(1){
    imu_wai = read_imu((char)WHO_AM_I);
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
  }
}
////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////

void init() {
  // Configure flash latency and set clock to run at 84 MHz

  // Enable GPIOA clock
  RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN);

  // "clock divide" = master clock frequency / desired baud rate
  // the phase for the SPI clock is 1 and the polarity is 0
  initSPI(1, 1, 1);


  // Load and done pins
  pinMode(FPGA_LOAD_PIN, GPIO_OUTPUT);  // LOAD
  pinMode(FPGA_RESET_PIN, GPIO_OUTPUT);  // Reset on FPGA
  pinMode(PB6, GPIO_OUTPUT);
  

  // debugging LEDs
  pinMode(DEBUG_LED_PIN_1, GPIO_OUTPUT);
  pinMode(DEBUG_LED_PIN_2, GPIO_OUTPUT);
  pinMode(DEBUG_LED_PIN_3, GPIO_OUTPUT);

    
  digitalWrite(FPGA_LOAD_PIN, 0);       // set the chip select high when idle.
  digitalWrite(IMU_LOAD_PIN, 1);
  
  digitalWrite(DEBUG_LED_PIN_1, 1);
  digitalWrite(DEBUG_LED_PIN_2, 0);
  digitalWrite(DEBUG_LED_PIN_3, 1);


}
void spin_motor(char m1_val, char m2_val) {
  int i; 
  digitalWrite(FPGA_LOAD_PIN, 0);
  spiSendReceiveTwoChar(m1_val, m2_val);
  while(SPI1->SR & SPI_SR_BSY);
  digitalWrite(FPGA_LOAD_PIN, 1);
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
  digitalWrite(FPGA_RESET_PIN, 1);
  //delaying between the pin input
  for(int i = 0; i < 200000; i++)
    ;
  digitalWrite(FPGA_RESET_PIN, 0);
}

void write_imu(char address, char write) {
  char imu_response;
  printf("trying to write to %d with write block %d \n", address, write);
  digitalWrite(PA5, 0);
  digitalWrite(PA11, 0);
  digitalWrite(PA9, 0);
  digitalWrite(IMU_LOAD_PIN, 0);
  imu_response = spiSendReceiveTwoChar(address, write);
  while(SPI1->SR & SPI_SR_BSY);
  //digitalWrite(PB6, 1);
  digitalWrite(PA11, 1);
  digitalWrite(PA5, 1);
  digitalWrite(PA9, 1);
  digitalWrite(IMU_LOAD_PIN, 1);
  //return imu_response;
}

char read_imu(char address) {
  char imu_response;
  address |= IMU_READ_ADDRESS;
  printf("trying to read from %d \n", address);
  //digitalWrite(PB6, 0);
  digitalWrite(PA5, 0);
  digitalWrite(PA11, 0);
  digitalWrite(PA9, 0);
  digitalWrite(IMU_LOAD_PIN, 0);
  imu_response = spiSendReceiveTwoChar(address, 0b00000000);
  while(SPI1->SR & SPI_SR_BSY);
  //digitalWrite(PB6, 1);
  digitalWrite(PA11, 1);
  digitalWrite(PA5, 1);
  digitalWrite(PA9, 1);
  digitalWrite(IMU_LOAD_PIN, 1);
  return imu_response;
}
