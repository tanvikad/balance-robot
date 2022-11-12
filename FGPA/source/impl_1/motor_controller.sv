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

	logic [7:0] counter;
	
	
	always_ff @(posedge clk)
		begin
			//Here we are changing the inputs based on the direction of the motor1 and motor2 input
			if(reset == 1'b1)
				begin
					counter <= 8'd0;
					a3 <= 1'b1;
					enable12 <= 1'b0;
					enable34 <= 1'b0;
				end
			else
				begin
					a3 <= 1'b0;
					a1<= (motor1_sign);
					a2<= (!motor1_sign);
					if(counter < motor1_period) counter <= counter + 1;
					else 
						begin
							enable12 <= !enable12;
							counter <= 0;
						end
				end
		end
endmodule

