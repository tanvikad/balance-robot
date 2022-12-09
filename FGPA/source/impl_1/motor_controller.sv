/////////////////////////////////////////////
// motor_controller
// module to send PWM signals to 2 motors according to motorx_sign and motorx_upperlimit
// Added by Tanvika Dasari
// tdasari@hmc.edu
//////////////////////////////////////////////////////////////////////////////////////////
module motor_controller(
	input logic reset,
	input logic load,
	input logic clk,
	input logic motor1_sign,
	input logic[6:0] motor1_upperlimit,
	input logic motor2_sign,
	input logic[6:0] motor2_upperlimit,
	output logic enable12,
	output logic enable34,
	output logic a1,
	output logic a2,
	output logic a3,
	output logic a4,
	output logic debug_light
);
	//assign a1 = reset;

	logic [6:0] counter;
	//Counter counts to the upper limit 100
	parameter [6:0] upper_limit = 7'd100;
	
	logic [6:0] old_motor1_upperlimit;
	logic [6:0] old_motor2_upperlimit;
	always_ff @(posedge clk)
		begin
			if(reset == 1'b1)
				begin
					enable12 <= 1'b0;
					enable34 <= 1'b0;
				end
			else
				begin
					//When load is pulled high (or when information is not being sent) we send the new upper limit through a flip flop
						begin
							old_motor1_upperlimit <= motor1_upperlimit;
							old_motor2_upperlimit <= motor2_upperlimit;
						end
					debug_light <= 1'b0;
					
					//we are updating a1 - a4 based on the motor sign
					a1<=(motor1_sign);
					a2<=(!motor1_sign);
					a3<=(motor2_sign);
					a4<= (!motor2_sign);
					
					//incrementing the counter every rising clock edge unless it overflows past the limit, then we set it to 0
					if(counter < upper_limit) counter <= counter + 1;
					else counter <= 0;
					
					
					//set the enables with the PWM signal
					enable12 <= (counter <= old_motor1_upperlimit);
					enable34 <= (counter <= old_motor2_upperlimit);
						
				end
		end
endmodule

