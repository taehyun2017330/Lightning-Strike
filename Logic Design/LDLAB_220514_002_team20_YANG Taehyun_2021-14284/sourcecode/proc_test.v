`timescale 1ns / 1ps

module PROC_test;
	// Inputs
	wire [7:0] inst;
	reg clk = 0;
	reg clr = 0;

	// Outputs
	wire [7:0] pc;
	wire [6:0] num1;
	wire [6:0] num2;

	// Instantiate the Unit Under Test (UUT)
	PROC uut (
		.inst(inst),
		.clr(clr),
		.clk(clk), 
		.pc(pc), 
		.num1(num1), 
		.num2(num2)
	);
	
	IMEM imem (
		.instruction(inst),
		.Read_Address(pc)
	);
	
	always #10 clk = ~clk;

	initial begin
		// Initialize Inputs

		// Wait 100 ns for global reset to finish
		#100;
        
		// Add stimulus here

	end
	
	
      
endmodule

