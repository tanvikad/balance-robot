// STM32F401RE_TIM.h
// Header for TIM functions

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



void tim_main(TIM_TypeDef * TIMx, uint32_t ms, void (*event_during_waiting)(int, struct imu_values*),  void (*event_after_waiting)(struct imu_values*, struct controller*));

#endif

