module top(
	input logic[7:0] motor1_count,
	input logic motor1_sign,
	input logic[7:0] motor2_count,
	input logic motor2_sign,
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
	top(clk,  motor1_count,  motor1_sign,  motor2_count, motor2_sign, enable12, enable34, a1, a2, a3, a4);
endmodule 