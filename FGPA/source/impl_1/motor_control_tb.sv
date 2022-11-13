module motor_controller_tb();
	logic motor1_sign;
	logic[6:0] motor1_upperlimit;
	logic motor2_sign;
	logic[6:0] motor2_upperlimit;
	logic enable12;
	logic enable34;
	logic a1;
	logic a2;
	logic a3;
	logic a4;
	logic clk;
	logic reset;
	logic debug_light; 
	logic load;
	 
	always begin
		  clk = 1'b0; #5;
		  clk = 1'b1; #5;
	 end
	 initial begin
		motor1_sign <= 1'b1;
		motor2_sign <= 1'b0;
		motor1_upperlimit <= 7'd30;
		motor2_upperlimit <= 7'd100;
	 end
	 
	 // please as load
	 initial begin
		  
		  #10 
		  reset <= 1'b1;
		  // Check outputs
		  #10;
		  reset <= 1'b0;
	  end
	 
	  motor_controller dut(reset, load, clk, motor1_sign, motor1_upperlimit, motor2_sign, motor2_upperlimit, enable12, enable34, a1, a2, a3, a4, debug_light);
	
endmodule