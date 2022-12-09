/////////////////////////////////////////////
// pwm
// 
// Added by Eric Chen
// erchen@hmc.edu
//////////////////////////////////////////////////////////////////////////////////////////
module pwm(input logic sck,
			input logic sdi,
			output logic sdo,
			input logic load);
		
		logic [7:0] motor1, motor2;
		
		pwm_spi spi(sck, load, sdi, sdo, motor1, motor2);
endmodule 


/////////////////////////////////////////////
// pwm_spi
// 
// Added by Eric Chen
// erchen@hmc.edu
//////////////////////////////////////////////////////////////////////////////////////////
module pwm_spi(input logic sck,
	   input logic load,
       input logic sdi,
	   output logic sdo,
	   output logic [7:0] motor1,
	   output logic [7:0] motor2);
	   
	always_ff @(posedge sck) 
	begin
		if (load == 0'b0)
		begin
				{motor1, motor2} = {motor1[6:0], motor2, sdi};
		end
	end
	
	assign sdo = 0;
	  
endmodule