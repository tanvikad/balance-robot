`timescale 10ns/1ns
/////////////////////////////////////////////
// testbench_aes_spi
// Tests AES with cases from FIPS-197 appendix
// Simulates full system with SPI load
/////////////////////////////////////////////

module testbench_spi();
    logic clk, load, done, sck, sdi, sdo;
    logic [7:0] motor1, motor2, motor1_in, motor2_in;
    logic [8:0] i;
	logic [15:0] comb;

    // Added delay
    logic delay;
    
    // device under test
    spi dut(sck, sdi, sdo, motor1, motor2);

    
    // test case
    initial begin   
// Test case from FIPS-197 Appendix A.1, B
		motor1_in 	 <= 8'b10101010;
		motor2_in    <= 8'b01010101;

// Alternate test case from Appendix C.1
//      key       <= 128'h000102030405060708090A0B0C0D0E0F;
//      plaintext <= 128'h00112233445566778899AABBCCDDEEFF;
//      expected  <= 128'h69C4E0D86A7B0430D8CDB78070B4C55A;
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
      // set delay to true
      delay = 1;
    end

    
	assign comb = {motor1_in, motor2_in};
    // shift in test vectors, wait until done, and shift out result
    always @(posedge clk) 
		begin
		  if (i == 16) load = 1'b0;
		  if (i<16) 
			begin
				#1; sdi = comb[15-i];
				#1; sck = 1; #5; sck = 0;
				i = i + 1;
			end
		  else 
			begin 
			  if (motor1 == motor1_in) 
				  begin
					$display("Testbench ran successfully");
				  end
			  else 
				  begin 
					$stop();
				  end
			end
		end
	
    
endmodule