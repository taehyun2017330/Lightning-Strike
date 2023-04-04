`timescale 1ns / 1ps

// ctrlSig parameters
`define ALUOP    0
`define MEMTOREG 1
`define MEMWRITE 2
`define MEMREAD  3
`define BRANCH   4
`define ALUSRC   5
`define REGWRITE 6
`define REGDST   7

module PROC(
	 input [7:0] inst,
    input clr,
    input clk,
    output reg [7:0] pc,
    output [6:0] num1,
    output [6:0] num2
	 );

    wire [1:0] op;
    wire [1:0] rs1;
    wire [1:0] rs2;
    wire [1:0] rd;
    wire [1:0] imm;
	 wire [7:0] immExt;
	 
	 wire [7:0] ctrlSig;
	 
    wire [7:0] rVal1;
    wire [7:0] rVal2;
	 
    wire [7:0] aluRes;
	 
    wire [7:0] nextPC;
	 
    wire [7:0] memRD;
	 
	 wire [7:0] regWD;
	 wire [1:0] regWI;
	 
	 
	 wire [3:0] bcd1;
	 wire [3:0] bcd2;
	 
	 wire clk1Hz;
	 
	 freq_divider fd(.clk(clk), .clr(clr), .clkout(clk1Hz));
	 
	 // instruction fetch & decode
    assign op = inst[7:6];
    assign rs1 = inst[5:4];
    assign rs2 = inst[3:2];
    assign rd = inst[1:0];
	 assign imm = inst[1:0];
	 assign immExt = {imm[1], imm[1], imm[1], imm[1], imm[1], imm[1], imm};
	 
	 //control
    Control ctr(.op(op), .ctrl(ctrlSig));
	 
	 //register file
	 assign regWI = ctrlSig[`REGDST]?rd:rs2;
	 assign regWD = ctrlSig[`MEMTOREG]?memRD:aluRes;
	
	 
	 RFILE rf(
		.regRI1(rs1),
		.regRD1(rVal1),
		.regRI2(rs2),
		.regRD2(rVal2),
		.regWI(regWI),
		.regWD(regWD),
		.regWE(ctrlSig[`REGWRITE]),
		.clk(clk1Hz),
		.clr(clr),
		.num1(bcd1), 
		.num2(bcd2)
	);
	 
	 //alu
	 assign aluRes = rVal1 + (ctrlSig[`ALUSRC]?immExt:rVal2);
	
	 //updatePc
	 assign nextPC = ctrlSig[`BRANCH]?(pc + 1 + immExt):(pc + 1);

	 // memory
    DMEM dm(
		.memA(aluRes), 
		.memWD(rVal2), 
		.memRE(ctrlSig[`MEMREAD]), 
		.memWE(ctrlSig[`MEMWRITE]), 
		.memRD(memRD), 
		.clr(clr),
		.clk(clk1Hz)
	);
	 

	 //7segment
	 BCD_to_7seg b1(.bcd(bcd1), .seg(num1));
	 BCD_to_7seg b2(.bcd(bcd2), .seg(num2));
	 
	 initial begin
		pc <= 0;
	 end
	 
    always@(posedge clk1Hz, posedge clr) begin
        if(clr) pc <= 0;
		  else pc <= nextPC;
    end

endmodule

