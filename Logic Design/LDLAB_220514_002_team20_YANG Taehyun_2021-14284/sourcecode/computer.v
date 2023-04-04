`timescale 1ns / 1ps

module computer(
	input clk,
	input clr,
	output [6:0] num1,
	output [6:0] num2
    );
	 
	wire [7:0] inst;
	wire [7:0] pc;

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
	
	

endmodule
