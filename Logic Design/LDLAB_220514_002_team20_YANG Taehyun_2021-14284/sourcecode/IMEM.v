`timescale  1ns / 1ps

module IMEM (instruction, Read_Address);
    output  [7:0] instruction;
    input   [7:0] Read_Address;
    wire    [7:0] MemByte[4:0];

    assign MemByte[0] = {2'b01, 2'b00, 2'b10, 2'b01};

    assign MemByte[1] = {2'b11, 2'b00, 2'b00, 2'b01};
	 
	 assign MemByte[2] = {2'b00, 2'b01, 2'b10, 2'b00};
	 
	 assign MemByte[3] = {2'b10, 2'b10, 2'b10, 2'b01};
	 
	 assign MemByte[4] = {2'b01, 2'b00, 2'b11, 2'b01};

    assign instruction = MemByte[Read_Address];

endmodule