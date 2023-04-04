`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   21:55:30 05/24/2022
// Design Name:   computer
// Module Name:   /csehome/ktwin01/logic design/final/Microproc/computer_test.v
// Project Name:  Microproc
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: computer
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module computer_test;

	// Inputs
	reg clk = 0;
	reg clr;

	// Outputs
	wire [6:0] num1;
	wire [6:0] num2;

	// Instantiate the Unit Under Test (UUT)
	computer uut (
		.clk(clk), 
		.clr(clr), 
		.num1(num1), 
		.num2(num2)
	);
	
	always #10 clk = ~clk;
	
	initial begin
		// Initialize Inputs
		clr = 0;

		// Wait 100 ns for global reset to finish
		#100;
        
		// Add stimulus here

	end
      
endmodule

