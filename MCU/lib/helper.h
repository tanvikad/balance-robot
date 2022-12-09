/**
Contains structures that are used throughout the program
    @file helper.h
    @author Eric Chen, Tanvika Dasari
    @version 1.0 12/08/2022
*/
#ifndef HELPER_H
#define HELPER_H

//A structure that holds IMU data
struct imu_values {
  float x_acc; //x linear acceleration
  float y_acc; //y linear acceleration
  float z_acc; //z linear acceleration
  float x_rot; //x rotational acceleration
  float y_rot; //y rotation acceleration
  float z_rot; // z rotation acceleratoin
};

#endif