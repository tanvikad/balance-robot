module pwm(input logic sck,
			input logic sdi,
			output logic sdo,
			input logic load);
		
		logic [7:0] motor1, motor2;
		
		pwm_spi spi(sck, sdi, sdo, motor1, motor2);
endmodule 

module pwm_spi(input logic sck,
       input logic sdi,
	   output logic sdo,
	   output logic [7:0] motor1,
	   output logic [7:0] motor2);
	   
	always_ff @(posedge sck) 
	begin
	{motor1, motor2} = {motor1[6:0], motor2, sdi};
	end
	
	assign sdo = 0;
	  
endmodule