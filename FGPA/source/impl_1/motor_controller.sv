module motor_controller(
	input logic reset,
	input logic clk,
	input logic motor1_sign,
	input logic[6:0] motor1_period,
	input logic motor2_sign,
	input logic[6:0] motor2_period,
	output logic enable12,
	output logic enable34,
	output logic a1,
	output logic a2,
	output logic a3,
	output logic a4
);
	//assign a1 = reset;

	logic [11:0] counter;
	
	logic[7:0] motor1_upper_limit;
	logic[7:0] motor2_upper_limit;
	
	always_ff @(posedge clk)
		begin
			//Here we are changing the inputs based on the direction of the motor1 and motor2 input
			if(reset == 1'b1)
				begin
					counter <= 12'd0;
					motor1_upper_limit <= motor1_period>>1;
					motor2_upper_limit <= motor2_period>>1;
					a3 <= 1'b1;
					enable12 <= 1'b0;
					enable34 <= 1'b0;
				end
			else
				begin
					a3 <= 1'b0;
					a1<= 1'b1;
					a2<= 1'b0;
					if(counter < motor1_period) counter <= counter + 1;
					else counter <= 0;
					if(counter < motor1_upper_limit) enable12 <= 1'b1;
					else enable12 <= 1'b0;
				end
			//output logic, setting the enable high when we are in the right period
			//enable12 <= (counter > 0);
			
			
			//enable34 <= (counter < motor2_upper_limit);
		end
endmodule

