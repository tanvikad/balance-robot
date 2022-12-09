/**
Contains functions to initialize and update PID controller
    @file pid.c
    @author Eric Chen, Tanvika Dasari
    @version 1.0 12/08/2022
*/

#include "lib/PID.h"

float pid_update(struct controller * c, float current_error){
  float control_effort;
  float derivative_error;

  c->previous_error = c->current_error;
  c->current_error = current_error;
  
  control_effort = c->k_p * (c->current_error);
  c->integral_total_error += c->current_error * c->dt;

  //we are capping the integration effort between -integration cap and +integration cap
  float integration_effort = c->k_i * (c->integral_total_error);
  if (integration_effort > c->integration_cap) integration_effort = c->integration_cap;
  else if(integration_effort < -1 * c->integration_cap) integration_effort = -1 * c->integration_cap;

  control_effort += integration_effort; 

  derivative_error = (c->current_error - c->previous_error) / c->dt;
  control_effort += c->k_d * (derivative_error);

  c->previous_control_effort[c->array_pointer] = control_effort;
  c->array_pointer += 1;
  c->array_pointer = c->array_pointer % 5;

  return control_effort;
}

void pid_init(struct controller * c)
{
  c->current_error = 0;
  c->integral_total_error = 0;
  c->previous_error = 0;
  c->k_p = 500;
  c->k_i = 2;
  c->k_d = 0.5;
  c->dt = 20;
  c->integration_cap = 10;
  for(int i = 0; i < 5; i++)
  {
    c->previous_control_effort[i] =0;
  }
  c->array_pointer = 0;

  printf("\n initalized the pid \n");
}
