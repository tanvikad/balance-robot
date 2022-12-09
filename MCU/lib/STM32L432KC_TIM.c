/**
Contains functions for MCU timers

    @file TIM.h
    @author Tanvika Dasari, Josh Brake
    @version 1.0 12/08/2022
*/

#include "STM32L432KC_TIM.h"
#include "STM32L432KC_RCC.h"

void initTIM(TIM_TypeDef * TIMx){
  // Set prescaler to give 1 ms time base
  uint32_t psc_div = (uint32_t) ((SystemCoreClock/1e3));

  // Set prescaler division factor
  TIMx->PSC = (psc_div - 1);
  // Generate an update event to update prescaler value
  TIMx->EGR |= 1;
  // Enable counter
  TIMx->CR1 |= 1; // Set CEN = 1
}

void delay_millis(TIM_TypeDef * TIMx, uint32_t ms){
  TIMx->ARR = ms;// Set timer max count
  TIMx->EGR |= 1;     // Force update
  TIMx->SR &= ~(0x1); // Clear UIF
  TIMx->CNT = 0;      // Reset count

  while(!(TIMx->SR & 1)); // Wait for UIF to go high
  
}

void tim_loop(TIM_TypeDef * TIMx, uint32_t ms, void (*event_during_waiting)(int, struct imu_values*),  void (*event_after_waiting)(struct imu_values*, struct controller*))
{
  TIMx->ARR = ms;// Set timer max count
  TIMx->EGR |= 1;     // Force update
  TIMx->SR &= ~(0x1); // Clear UIF
  TIMx->CNT = 0;      // Reset count

  int i = 0; //A counter that is used to toggle a debug LED
  printf("The ARR is currently: %d", TIMx->ARR);
  struct imu_values values;
  struct controller pid_controller;

  //initial the PID controller 
  pid_init(&pid_controller);
  
  while(1)
  {
    while(!(TIMx->SR & 1)) // Wait for UIF to go high
    {
      event_during_waiting(i, &values);
    }
    
    //reseting the event 
    TIMx->SR &= ~(0x1);

    i++;
    event_after_waiting(&values, &pid_controller);
  }
  
}