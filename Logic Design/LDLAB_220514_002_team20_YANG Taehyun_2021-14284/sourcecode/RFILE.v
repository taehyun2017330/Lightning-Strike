`timescale 1ns/1ps

module RFILE(
	input [1:0] regRI1,
	output [7:0] regRD1,
	input [1:0] regRI2,
	output [7:0] regRD2,
	input [1:0] regWI,
	input [7:0] regWD,
	input regWE,
	input clk,
	input clr,
	output [3:0] num1, 
	output [3:0] num2
	);
	
    reg [7:0] R[3:0];
	 
	 assign regRD1 = R[regRI1];
	 assign regRD2 = R[regRI2];
	 
	 assign num1 = regWD[3:0];
	 assign num2 = regWD[7:4];
	 
	 initial begin
		 R[2'd0] <= 0;
       R[2'd1] <= 0;
       R[2'd2] <= 0;
       R[2'd3] <= 0;
	 end

    always@(posedge clr, posedge clk) begin
		if(clr) begin
        R[2'd0] <= 0;
        R[2'd1] <= 0;
        R[2'd2] <= 0;
        R[2'd3] <= 0;
		 end
		 else begin
			if(regWE == 1) begin
				R[regWI] <= regWD;
			end
		 end
    end


endmodule