module motor_controller(
	input logic reset,
	input logic clk,
	input logic motor1_sign,
	input logic[6:0] motor1_count,
	input logic motor2_sign,
	input logic[6:0] motor2_count,
	output logic enable12,
	output logic enable34,
	output logic a1,
	output logic a2,
	output logic a3,
	output logic a4
);

	parameter [11:0] period = 12'd2500;
	logic [11:0] counter;
	
	logic[7:0] motor1_upper_limit;
	logic[7:0] motor2_upper_limit;
	
	always_ff @(posedge clk)
		begin
			//Here we are changing the inputs based on the direction of the motor1 and motor2 input
			if(reset)
			begin
				counter <= 12'd0;
				motor1_upper_limit <= motor1_count + 6'd125;
				motor2_upper_limit <= motor2_count + 6'd125;
			end
			else
				begin
					if(motor1_sign == 1'b1)
						begin
							a1 <= 1;
							a2 <= 0;
						end
					else
						begin
							a1 <= 0;
							a2 <= 1;	
						end
					
					if(motor2_sign == 1'b1)
						begin
							a3 <= 1;
							a4 <= 0;
						end
					else
						begin
							
							a3 <= 0;
							a4 <= 1;
						end
					
					//incrementing the counter to be 50Hz
					if(counter > period) counter <= 0;
					else counter <= counter + 1;
				end
			//output logic, setting the enable high when we are in the right period
			enable12 <= (counter < motor1_upper_limit);
			enable34 <= (counter < motor2_upper_limit);
	end
endmodule

