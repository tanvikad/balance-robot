---
layout: page
title: Documentation
permalink: /doc/
---

# Schematics
<!-- Include images of the schematics for your system. They should follow best practices for schematic drawings with all parts and pins clearly labeled. You may draw your schematics either with a software tool or neatly by hand. -->


<div style="text-align: left">
  <img src="https://tanvikad.github.io/balance-robot/assets/schematics/Schematics.jpg" alt="results" width="1000" />
  <div style="text-align:center">hiii</div>
</div>


# Source Code Overview
<!-- This section should include information to describe the organization of the code base and highlight how the code connects. -->
Source code for the self balancing robot can be found with documentation [here](https://github.com/echen4628/balance-robot-code). The MCU and FPGA code is located in their respective folders. 

The MCU code is intended for the STM32L432KC. Within the MCU folder, there are several files with helper functions in the $\texttt{/lib}$ folder for using [GPIO pins](https://github.com/echen4628/balance-robot-code/blob/main/MCU/lib/STM32L432KC_GPIO.h), [timers](https://github.com/echen4628/balance-robot-code/blob/main/MCU/lib/STM32L432KC_TIM.h), and [SPI communication](https://github.com/echen4628/balance-robot-code/blob/main/MCU/lib/STM32L432KC_SPI.h). The main function is located in [spi.c](https://github.com/echen4628/balance-robot-code/blob/main/MCU/spi.c) and the PID control loop critical for self balancing is located in the [pid.c](https://github.com/echen4628/balance-robot-code/blob/main/MCU/pid.c). 

The FPGA code is written for the iCE40 UP5K. The top module is located in [top.sv](https://github.com/echen4628/balance-robot-code/blob/main/FPGA/source/top.sv). [motor_controller.sv](https://github.com/echen4628/balance-robot-code/blob/main/FPGA/source/motor_controller.sv) contains code for generating a PWM signal to control the motors, and the [spi.sv](https://github.com/echen4628/balance-robot-code/blob/main/FPGA/source/spi.sv) contains the module for recieving messages from our microcontroller. Files ending in $\texttt{tb}$ are testbench files.

For more information on our methods, check out our [project report](https://tanvikad.github.io/balance-robot/report/).



# Bill of Materials
<!-- The bill of materials should include all the parts used in your project along with the prices and links.  -->

| Item | Part Number | Quantity | Unit Price | Link |
| ---- | ----------- | ----- | ---- | ---- |
| 4xAA Battery  |  N.A | 1 | $7.00 |  N.A |
| Adafruit 6 DoF IMU  |  4502 | 1 | $19.95 |  [link](https://www.adafruit.com/ product/4502 description) |
| Battery Holder |  3859 | 1 | $2.95 |  [link](https://www.adafruit.com/product/3859) |
| Adafruit Robot Kit  |  3244 | 1 | $24.95 |  [link](https://www.adafruit.com/product/3244) |
| H-Bridge  |  STM L293D | 1  | $8.52 | [link](https://www.mouser.com/ProductDetail/STMicroelectronics/L293D)|

**Total cost: $56.37**
