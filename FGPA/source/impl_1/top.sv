module top_wrapper(
	input logic reset,
	input logic sck,
	input logic sdi,
	input logic load,
	output logic sdo,
	output logic enable12,
	output logic enable34,
	output logic a1,
	output logic a2,
	output logic a3,
	output logic a4,
	output logic debug_light
);
	logic [7:0] motor1, motor2;
	pwm_spi spi(sck, sdi, sdo, motor1, motor2);
	// top dut(reset, 1'd1, 7'd25, 1'd1, 7'd50, enable12, enable34, a1, a2, a3, a4, debug_light);
	top dut(reset, load, motor1[7], motor1[6:0], motor2[7], motor2[6:0], enable12, enable34, a1, a2, a3, a4, debug_light);

endmodule




module top(
	input logic reset,
	input logic load,
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

	// Internal high-speed oscillator
	//(Clock divider selection. 0b00 = 48 MHz, 0b01 = 24 MHz, 0b10 = 12MHz, 0b11 = 6 MHz
	HSOSC #(.CLKHF_DIV(2'b11))
		hf_osc (.CLKHFPU(1'b1), .CLKHFEN(1'b1), .CLKHF(int_osc));
	
	logic clk;
	logic[5:0] counter;
	parameter[5:0] limit = 6'd50;
	
	always_ff@(posedge int_osc)
	begin
		if(counter <= limit) counter <= counter+1;
		else 
			begin
				clk <= !clk;
				counter <= 0;
			end
	end
	motor_controller dut(reset, load, clk,  motor1_sign, motor1_upperlimit, motor2_sign, motor2_upperlimit, enable12, enable34, a1, a2, a3, a4, debug_light);
endmodule 

