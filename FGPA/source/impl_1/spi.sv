module pwm(input logic sck

/////////////////////////////////////////////
// aes
//   Top level module with SPI interface and SPI core
/////////////////////////////////////////////

module aes(input  logic clk,
           input  logic sck, 
           input  logic sdi,
           output logic sdo,
           input  logic load,
           output logic done);
                    
    logic [127:0] key, plaintext, cyphertext;
            
    aes_spi spi(sck, sdi, sdo, done, key, plaintext, cyphertext);   
    aes_core core(clk, loa

module spi(input logic sck,
       input logic sdi,
	   output logic sdo,
	   output logic [7:0] motor1,
	   output logic [7:0] motor2);
	   
	always_ff @(posedge sck) 
	begin
	{motor1, motor2} = {motor1[6:0], motor2, sdi};
	end
	   

endmodule