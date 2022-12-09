/**
Contains function headers for PID controller

    @file pid.h
    @author Eric Chen, Tanvika Dasari
    @version 1.0 12/08/2022
*/

#ifndef PID_H
#define PID_H

#include <stdio.h>

// A struct that holds all the information relating to the PID controller
struct controller {
  float integral_total_error; //a running error sum for I-control
  float previous_error; //an error holder that is updated every pid_update for D-control
  float current_error; //an error that holds current error for P-control
  float dt; //the time step between PID updates
  float k_p; //constant for P-control
  float k_i; //constant for I-control
  float k_d; //constant for D-control
  float integration_cap; //A limit to control the amount of I-control that can be used
  float previous_control_effort[5]; //The previous 5 control effort to filter
  int array_pointer; //array pointer for the array previous_control_effort
};

/**
* Updates the controller with the current error
* @param c Pointer to the PID controller that is being updated
* @param current_error Error in angle to update the controller
* @return control effort for this update
*/
float pid_update(struct controller * c, float current_error);

/**
* Initializes the controller
* @param c Intializes the controller based on values in pid_init
*/
void pid_init(struct controller * c);




#endif