/**
Contains the main and functions to communicate with the IMU and FPGA

    @file spi.c
    @author Eric Chen, Tanvika Dasari
    @version 1.0 12/08/2022
*/

#include "spi.h"
#include "lib/helper.h"
#include <float.h>
#include <math.h>
#define RCC_BASE_ADR (0x40021000UL)
#define RCC_APB1ENR1 ((uint32_t*)(RCC_BASE_ADR + 0x58))


float control_cap(float control_effort){
  if(control_effort < -100) return -100;
  if(control_effort > 100) return 100;
  return control_effort;
}

void print_float(float f)
{
  int before_decimal_place = (int)(f);
  int after_decimal_place = (f - before_decimal_place) * 1000;
  printf("%d.%d", before_decimal_place, after_decimal_place);
}

void waiting(int i, struct imu_values * values)
{
  //debug led is switched on and off based on if the timer is continued to count
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

  printf("\n after waiting y-acceleration value ");
  print_float(values->y_acc);
  printf("\n");

  printf("\n after waiting x-acceleration value ");
  print_float(values->x_acc);
  printf("\n");

  //we check that the robot is falling based on if the y_acceleration is less than 0 
  bool falling_forward = (values->y_acc < 0.0);
  printf("Falling forward %d \n ", falling_forward);

  //we estimate gravity to be 10.0
  //since upright is when z_acc = 10.0 our error on magnitude is the difference
  float error =   10.0 - values->z_acc;
  
  //if the gravity is showing up as more than 10, we will just assume that it is balanced
  if(error < 0) error = 0;

  printf("The error is ");
  print_float(error);
  printf("\n");

  //we want to use the direction to change the error going into the control effort
  if(falling_forward) error = error * -1;

  //we update the controller with our error
  float ce = pid_update(c, (float)(error));

  printf("\n The CONTROL EFFORT IS %d \n", (int)(ce));

  //we make sure our control is capped so that we can create a PWM signal
  int capped_control = (int) (control_cap(ce));
  printf("Try to spin motor with %d", capped_control);

  //convert the integer capped control into our 8 bits of control based our FPGA specification
  char motor_output = generate_FPGA_message(capped_control);

  //send the control for both motors through SPI
  spin_motor(motor_output, motor_output);
}

void init() {

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

  // set the chip select high when idle.
  digitalWrite(FPGA_LOAD_PIN, 1);
  digitalWrite(IMU_LOAD_PIN, 1);
  
  digitalWrite(DEBUG_LED_PIN_1, 1);
  digitalWrite(DEBUG_LED_PIN_2, 0);
  digitalWrite(DEBUG_LED_PIN_3, 1);

  //initializing a timer
  //RCC_APB1ENR1 |= (RCC_APB1ENR1_TIM6EN);
  RCC->AHB1ENR |= (RCC_APB1ENR1_TIM6EN);
  *RCC_APB1ENR1 |= (1<<4);
  *RCC_APB1ENR1 |= (1<<5);

  //initializes the timer
  initTIM(TIM6);

  //checking if the IMU is working 
  char imu_wai = read_imu((char)0b00001111);
  if(imu_wai == (char)(170))
    printf("IMU returned the correct WhoAmI register");
  else
    printf("IMU returned the wrong WhoAmI register");

  
  //set accelerometer frequency to 6.66 kHz and range from -2g to 2g
  write_imu((char) CTRL1_XL, (char)0b10100000);
  
  //set gyroscope frequency to 6.66 kHz and range from -125 to 125 dps
  write_imu((char) CTRL2_G,  (char)0b10100000);

  // enable high pass filter for gyroscope
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


float scale_accel(int16_t raw_acceleration) {
 return (float) (raw_acceleration * ACCEL_SCALE_2G * 9.807/1000.0);
}


float get_angle(int16_t raw_angle) {
 return (float) (raw_angle * GYRO_SCALE_250 * 0.017453293/1000.0);
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

char generate_FPGA_message(int control_effort){
  // encode control effort based on sign
  if (control_effort < 0){
    return generate_FPGA_message_helper(1, control_effort*-1);
  }
  else{
    return generate_FPGA_message_helper(0, control_effort);
  }
}

char generate_FPGA_message_helper(bool reverse, int value){
  char motor_output = (char) value;
  // The msb of the message indicate direction of motor
  // the rest of the message encodes a binary number
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
  printf("Writing to %d with write block %d \n", address, write);
  digitalWrite(IMU_LOAD_PIN, 0);
  imu_response = spiSendReceiveTwoChar(address, write);
  while(SPI1->SR & SPI_SR_BSY);
  digitalWrite(IMU_LOAD_PIN, 1);
}

char read_imu(char address) {
  char imu_response;
  address |= IMU_READ_ADDRESS;
  digitalWrite(IMU_LOAD_PIN, 0);
  // We send the address and the send an empty byte to get the information at the address
  imu_response = spiSendReceiveTwoChar(address, 0b00000000);
  while(SPI1->SR & SPI_SR_BSY);
  digitalWrite(IMU_LOAD_PIN, 1);
  return imu_response;
}

int main(void) {
  init();
  tim_loop(TIM6, 20, waiting, after_waiting);
}

