`timescale 1ns/1ps

module Control(op, ctrl);
    input [1:0] op;
    output reg [7:0] ctrl;

	 always@(op) begin
		 case(op)
			  2'd0: ctrl = 8'b11000001;
			  2'd1: ctrl = 8'b01101010;
			  2'd2: ctrl = 8'b00100100;
			  2'd3: ctrl = 8'b00010010;
			  default: ctrl = 0;
		 endcase
	 end

endmodule