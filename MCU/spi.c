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
#include "lib/helper.h"
#include <float.h>
#define RCC_BASE_ADR (0x40021000UL)
#define RCC_APB1ENR1 ((uint32_t*)(RCC_BASE_ADR + 0x58))


////////////////////////////////////////////////
// Main
////////////////////////////////////////////////

int main(void) {

  init();
  loop();
}



void print_float(float f)
{
  int before_decimal_place = (int)(f);
  int after_decimal_place = (f - before_decimal_place) * 1000;
  printf("%d.%d", before_decimal_place, after_decimal_place);
}


void waiting(int i, struct imu_values * values)
{
  if(i % 2 == 0)
    digitalWrite(DEBUG_LED_PIN_1, 1);
  else
    digitalWrite(DEBUG_LED_PIN_1, 0);

  char z_high = read_imu((char) OUTZ_H_A);
  char z_low = read_imu((char) OUTZ_L_A);
  float z = scale_accel(twosComplement_to_int(z_high, z_low));
  values->z_acc = z;

  char x_high = read_imu((char) OUTX_H_A);
  char x_low = read_imu((char) OUTX_L_A);
  float x = scale_accel(twosComplement_to_int(x_high, x_low));
  //printf("IMU x: %d\n", x_int);
  values->x_acc = x;

  char y_high = read_imu((char) OUTY_H_A);
  char y_low = read_imu((char) OUTY_L_A);
  float y = scale_accel(twosComplement_to_int(y_high, y_low));
  //printf("IMU y: %d\n", y_int);
  values->y_acc = y;

  char rot_z_high = read_imu((char) OUTZ_H_G);
  char rot_z_low = read_imu((char) OUTZ_L_G);
  float rot_z = get_angle(twosComplement_to_int(rot_z_high, rot_z_low));
  values->z_rot = rot_z;

  char rot_x_high = read_imu((char) OUTX_H_G);
  char rot_x_low = read_imu((char) OUTX_L_G);
  float rot_x = get_angle(twosComplement_to_int(rot_x_high, rot_x_low));
  //printf("IMU x angle: %d\n", rot_x_int);
  values->x_rot = rot_x;

  char rot_y_high = read_imu((char) OUTY_H_G);
  char rot_y_low = read_imu((char) OUTY_L_G);
  float rot_y = get_angle(twosComplement_to_int(rot_y_high, rot_y_low));
  //printf("IMU y angle: %d\n", rot_y_int);
  values->y_rot = rot_y;



}

void after_waiting(struct imu_values * values, struct controller* c)
{
  printf("\n after waiting z-acceleration value ");
  print_float(values->z_acc);
  printf("\n");


  printf("\n after waiting x-acceleration value ");
  print_float(values->x_acc);
  printf("\n");


  bool falling_forward = (values->x_acc < 2.0);
  printf("Falling forward %d \n ", falling_forward);

  float error =   10.0 - values->z_acc;
  if(error < 0) error = error*-1;

  printf("The error is ");
  print_float(error);
  printf("\n");

  if(falling_forward) error = error * -1;

  float ce = pid_update(c, (float)(error));

  printf("\n The CONTROL EFFORT IS %d \n", (int)(ce));
}





void loop(){

  tim_main(TIM6, 1000, waiting, after_waiting);

  /*char m1_val;
  char m2_val;
  char imu_wai;
  char imu_yaw_h;
  char imu_yaw_l;
  char temp;
  char z_high;
  char z_low;
  char x_high;
  char x_low;
  char y_high;
  char y_low;
  char rot_z_high;
  char rot_z_low;
  char rot_x_high;
  char rot_x_low;
  char rot_y_high;
  char rot_y_low;

  m1_val = set_val(80);

  while(1){
    spin_motor(m1_val, m1_val);
 
    
     z_high = read_imu((char) OUTZ_H_A);
     z_low = read_imu((char) OUTZ_L_A);
     float z = scale_accel(twosComplement_to_int(z_high, z_low));
     int z_int = (int)(z * 100);
     printf("IMU z: %d\n", z_int);

    // x_high = read_imu((char) OUTX_H_A);
    // x_low = read_imu((char) OUTX_L_A);
    // float x = scale_accel(twosComplement_to_int(x_high, x_low));
    // int x_int = (int)(x * 100);
    // printf("IMU x: %d\n", x_int);

    // y_high = read_imu((char) OUTY_H_A);
    // y_low = read_imu((char) OUTY_L_A);
    // float y = scale_accel(twosComplement_to_int(y_high, y_low));
    // int y_int = (int)(y * 100);
    // printf("IMU y: %d\n", y_int);

    rot_z_high = read_imu((char) OUTZ_H_G);
    rot_z_low = read_imu((char) OUTZ_L_G);
    float rot_z = get_angle(twosComplement_to_int(rot_z_high, rot_z_low));
    int rot_z_int = (int)(rot_z * 100);
    printf("IMU z angle: %d\n", rot_z_int);

    rot_x_high = read_imu((char) OUTX_H_G);
    rot_x_low = read_imu((char) OUTX_L_G);
    float rot_x = get_angle(twosComplement_to_int(rot_x_high, rot_x_low));
    int rot_x_int = (int)(rot_x * 100);
    printf("IMU x angle: %d\n", rot_x_int);

    rot_y_high = read_imu((char) OUTY_H_G);
    rot_y_low = read_imu((char) OUTY_L_G);
    float rot_y = get_angle(twosComplement_to_int(rot_y_high, rot_y_low));
    int rot_y_int = (int)(rot_y * 100);
    printf("IMU y angle: %d\n", rot_y_int);
  }*/
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
  pinMode(FPGA_LOAD_PIN, GPIO_OUTPUT);  // SPI to FPGA
  pinMode(FPGA_RESET_PIN, GPIO_OUTPUT);  // Reset on FPGA
  pinMode(IMU_LOAD_PIN, GPIO_OUTPUT);   // SPI to IMU
  

  // debugging LEDs
  pinMode(DEBUG_LED_PIN_1, GPIO_OUTPUT);
  pinMode(DEBUG_LED_PIN_2, GPIO_OUTPUT);
  pinMode(DEBUG_LED_PIN_3, GPIO_OUTPUT);

    
  digitalWrite(FPGA_LOAD_PIN, 1);       // set the chip select high when idle.
  digitalWrite(IMU_LOAD_PIN, 1);
  
  digitalWrite(DEBUG_LED_PIN_1, 1);
  digitalWrite(DEBUG_LED_PIN_2, 0);
  digitalWrite(DEBUG_LED_PIN_3, 1);

  //initializing a timer
  //RCC_APB1ENR1 |= (RCC_APB1ENR1_TIM6EN);
  RCC->AHB1ENR |= (RCC_APB1ENR1_TIM6EN);
  *RCC_APB1ENR1 |= (1<<4);
  *RCC_APB1ENR1 |= (1<<5);
  initTIM(TIM6);



  //checking if the IMU is working 
  char imu_wai = read_imu((char)0b00001111);
  printf("imu returned %d \n", imu_wai);
    
  //write_imu((char) CTRL1_XL, (char)0b01010000);
  //write_imu((char) CTRL2_G,  (char)0b01010000);


  write_imu((char) CTRL1_XL, (char)0b10100000);
  write_imu((char) CTRL2_G,  (char)0b10100000);

  write_imu((char) CTRL7_G,  (char)0b01000000);
  


}
void spin_motor(char m1_val, char m2_val) {
  digitalWrite(FPGA_LOAD_PIN, 0);
  spiSendReceiveTwoChar(m1_val, m2_val);
  while(SPI1->SR & SPI_SR_BSY);
  digitalWrite(FPGA_LOAD_PIN, 1);
}


int16_t twosComplement_to_int(char higher, char lower) {
 return (int16_t) (higher << 8 | lower);
}

float scale_accel(int16_t raw) {
 return (float) (raw * ACCEL_SCALE_2G * 9.807/1000.0);
}

float get_angle(int16_t raw) {
 return (float) (raw * GYRO_SCALE_250 * 0.017453293/1000.0);
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
  digitalWrite(IMU_LOAD_PIN, 0);
  imu_response = spiSendReceiveTwoChar(address, write);
  while(SPI1->SR & SPI_SR_BSY);
  digitalWrite(IMU_LOAD_PIN, 1);
  //return imu_response;
}

char read_imu(char address) {
  char imu_response;
  //printf("trying to read from %d \n", address);
  address |= IMU_READ_ADDRESS;
  digitalWrite(IMU_LOAD_PIN, 0);
  imu_response = spiSendReceiveTwoChar(address, 0b00000000);
  while(SPI1->SR & SPI_SR_BSY);
  digitalWrite(IMU_LOAD_PIN, 1);
  return imu_response;
}
