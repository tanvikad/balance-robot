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
	parameter [6:0] upper_limit = 7'd100;

	logic [6:0] old_motor1_upperlimit;
	logic [6:0] old_motor2_upperlimit;
	always_ff @(posedge clk)
		begin
			//Here we are changing the inputs based on the direction of the motor1 and motor2 input
			if(reset == 1'b1)
				begin
					counter <= 8'd0;
					debug_light <= 1'b1;
					enable12 <= 1'b0;
					enable34 <= 1'b0;
					//old_motor1_upperlimit <= 8'd50;
					//old_motor2_upperlimit <= 8'd50;
				end
			else
				begin
					if(load == 1'b0)
						begin
							old_motor1_upperlimit <= motor1_upperlimit;
							old_motor2_upperlimit <= motor2_upperlimit;
						end
					debug_light <= 1'b0;
					a1<=(motor1_sign);
					a2<=(!motor1_sign);
					a3<=(motor2_sign);
					a4<= (!motor2_sign);
					if(counter < upper_limit) counter <= counter + 1;
					else counter <= 0;
					
					enable12 <= (counter <= old_motor1_upperlimit);
					enable34 <= (counter <= old_motor2_upperlimit);
						
				end
		end
endmodule

