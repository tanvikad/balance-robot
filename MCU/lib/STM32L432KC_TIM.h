/**
Contains function headers for timer functions

    @file TIM.h
    @author Tanvika Dasari, Josh Brake
    @version 1.0 12/08/2022
*/
#ifndef STM32L4_TIM_H
#define STM32L4_TIM_H

#include <stdint.h> // Include stdint header
#include "STM32L432KC_GPIO.h"
#include <stdio.h>
#include "helper.h"
#include "PID.h"

///////////////////////////////////////////////////////////////////////////////
// Function prototypes
///////////////////////////////////////////////////////////////////////////////

void initTIM(TIM_TypeDef * TIMx);
void delay_millis(TIM_TypeDef * TIMx, uint32_t ms);
void delay_micros(TIM_TypeDef * TIMx, uint32_t us);


/**
* The main timer loop that counts up to the specified ms 
* @param TIMx the timer to configure on the MCU
* @param ms The milliseconds the timer will count to
* @param event_during_waiting The function that is being called while the timer counts to ARR 
* @param event_after_waiting The function that is being called after timer counts to ARR
*/
void tim_loop(TIM_TypeDef * TIMx, uint32_t ms, void (*event_during_waiting)(int, struct imu_values*),  void (*event_after_waiting)(struct imu_values*, struct controller*));

#endif

