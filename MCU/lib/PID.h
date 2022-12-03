#ifndef PID_H
#define PID_H

#include <stdio.h>

struct controller {
  float integral_total_error;
  float previous_error;
  float current_error;
  float dt;
  float k_p;
  float k_i;
  float k_d;
  float integration_cap;
};


float pid_update(struct controller * c, float current_angle);
void pid_init(struct controller * c);




#endif