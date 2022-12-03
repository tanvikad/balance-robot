#include "lib/PID.h"

float pid_update(struct controller * c, float current_angle){
  float control_effort;
  float derivative_error;

  c->previous_error = c->current_error;
  c->current_error = current_angle;
  
  control_effort = c->k_p * (c->current_error);
  c->integral_total_error += c->current_error * c->dt;

  //we are capping the integration effort between -integration cap and +integration cap
  float integration_effort = c->k_i * (c->integral_total_error);
  if (integration_effort > c->integration_cap) integration_effort = c->integration_cap;
  else if(integration_effort < -1 * c->integration_cap) integration_effort = -1 * c->integration_cap;

  control_effort += integration_effort; 

  derivative_error = (c->current_error - c->previous_error) / c->dt;
  control_effort += c->k_d * (derivative_error);

  return control_effort;
}

void pid_init(struct controller * c)
{
  c->current_error = 0;
  c->integral_total_error = 0;
  c->previous_error = 0;
  c->k_p = 60;
  c->k_i = 10;
  c->k_d = 1;
  c->dt = 20;
  c->integration_cap = 10;

  printf("\n initalized the pid \n");
}


//class PID():
//    def __init__(self, k_p = 2, k_i = 10, k_d = 20, dt):
//        self.k_p = k_p
//        self.k_i = k_i
//        self.k_d = k_d
//        self.integral_total_error = 0
//        self.dt = dt
//        self.previous_error = 0
//        self.current_error = 0

//    def update(self, current_angle):
//        self.previous_error = self.current_error
//        self.current_error = 0 - current_angle

//        control_effort = self.k_p * self.current_error

//        integral_total_error +=  self.current_error * self.dt
//        control_effort += self.k_i * integral_total_error
        
//        #we redo the derivative error everytime, we might want to keep some of the old value
//        derivative_error =  ( self.current_error - self.previous_error ) / self.dt
//        control_effort += self.k_d * derivative_error

//        return control_effort
        