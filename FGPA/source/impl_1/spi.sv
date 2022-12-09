/////////////////////////////////////////////
// spi
// Faciliates spi communication with mcu
// Added by Eric Chen
// erchen@hmc.edu
//////////////////////////////////////////////////////////////////////////////////////////
module spi(input logic sck,
	   input logic load,
       input logic sdi,
	   output logic sdo,
	   output logic [7:0] motor1,
	   output logic [7:0] motor2);
	   
	always_ff @(posedge sck) 
	begin
		// when load is low, the mcu is talking to the spi, so start shifting in bytes
		if (load == 1'b0)
		begin
				{motor1, motor2} = {motor1[6:0], motor2, sdi};
		end
	end
	
	// no need to send anything to mcu
	assign sdo = 0;
	  
endmodule