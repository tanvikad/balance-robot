`timescale 10ns/1ns
/////////////////////////////////////////////
// spi_tb
// test our top spi module 
// Added by Eric Chen
// erchen@hmc.edu
//////////////////////////////////////////////////////////////////////////////////////////
module testbench_spi_top();
    logic clk, load, done, sck, sdi, sdo, reset,  enable12, enable34, a1, a2, a3, a4, debug_light;
	logic [7:0] motor1_in, motor2_in;
    logic [8:0] i;
	logic [15:0] comb;

    // Added delay
    logic delay;
    
    // device under test
    spi dut(reset, sck, clk, sdi, load, sdo, enable12, enable34, a1, a2, a3, a4, debug_light);
    
    // test case
    initial begin   
      motor1_in 	 <= 8'b10101010;
      motor2_in      <= 8'b01010101;
    end
    
    // generate clock and load signals
    initial 
        forever begin
            clk = 1'b0; #5;
            clk = 1'b1; #5;
        end
        
    initial begin
      i = 0;
      load = 1'b1;
	  reset = 1'b0;
      delay = 1;
    end
    
	assign comb = {motor1_in, motor2_in};
    // shift in test vectors, wait until done, check if the correct motor values are registered
    always @(posedge clk) 
		begin
		  if (i == 16) load = 1'b0;
		  if (i<16) 
			begin
				#1; sdi = comb[15-i];
				#1; sck = 1; #5; sck = 0;	
			end
		  i = i + 1;
		  if(i == 200)
			  begin
				  load = 1'b1;
				  i = 0;
				  motor1_in    <= 8'd100;
				  motor2_in    <= 8'd50;
			  end
		  
		end
	
endmodule