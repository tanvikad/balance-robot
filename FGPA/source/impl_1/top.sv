module top_wrapper(
	input logic reset,
	output logic enable12,
	output logic enable34,
	output logic a1,
	output logic a2,
	output logic a3,
	output logic a4
);
	
	top dut(reset, 1'd0, 7'd100, 1'd1, 7'd50, enable12, enable34, a1, a2, a3, a4);
endmodule




module top(
	input logic reset,
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

	// Internal high-speed oscillator
	//(Clock divider selection. 0b00 = 48 MHz, 0b01 = 24 MHz, 0b10 = 12MHz, 0b11 = 6 MHz
	HSOSC #(.CLKHF_DIV(2'b11))
		hf_osc (.CLKHFPU(1'b1), .CLKHFEN(1'b1), .CLKHF(int_osc));
	
	logic clk;
	logic[5:0] counter;
	parameter[5:0] limit = 6'd48;
	
	always_ff@(posedge int_osc)
	begin
		if(counter <= limit) counter <= counter+1;
		else counter <= 0;
		clk <= counter[5];
	end
	motor_controller dut(reset, clk,  motor1_sign, motor1_period, motor2_sign, motor2_period, enable12, enable34, a1, a2, a3, a4);
endmodule 



module motor_controller_tb();
	logic motor1_sign;
	logic[6:0] motor1_period;
	logic motor2_sign;
	logic[6:0] motor2_period;
	logic enable12;
	logic enable34;
	logic a1;
	logic a2;
	logic a3;
	logic a4;
	logic clk;
	logic reset;
	 
	always begin
		  clk = 1'b0; #5;
		  clk = 1'b1; #5;
	 end
	 initial begin
		motor1_sign <= 1'b1;
		motor2_sign <= 1'b0;
		motor1_period <= 7'd30;
		motor2_period <= 7'd100;
	 end
	 
	 initial begin
		  
		  #10 
		  reset <= 1'b1;
		  // Check outputs
		  #10;
		  reset <= 1'b0;
	  end
	 
	  motor_controller dut(reset, clk, motor1_sign, motor1_period, motor2_sign, motor2_period, enable12, enable34, a1, a2, a3, a4);
	
endmodule