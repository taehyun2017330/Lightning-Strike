`timescale 1ns / 1ps

module BCD_to_7seg(
    input [3:0] bcd,
    output reg [6:0] seg
    );
	 
	 always@(bcd) begin
		case(bcd)
			4'd0: 	seg <= 7'b0111111;
			4'd1: 	seg <= 7'b0000110;
			4'd2: 	seg <= 7'b1011011;
			4'd3: 	seg <= 7'b1001111;
			4'd4: 	seg <= 7'b1100110;
			4'd5: 	seg <= 7'b1101101;
			4'd6: 	seg <= 7'b1111101;
			4'd7: 	seg <= 7'b0000111;
			4'd8: 	seg <= 7'b1111111;
			4'd9: 	seg <= 7'b1101111;
			4'd10: 	seg <= 7'b1110111; // A
			4'd11: 	seg <= 7'b0011111; // B (b)
			4'd12: 	seg <= 7'b1001110; // C
			4'd13:	seg <= 7'b0111101; // D (d)
			4'd14: 	seg <= 7'b1001111; // E
			4'd15: 	seg <= 7'b1000111; //F
			default:	seg <= 7'b0000000;
		endcase
	 end

endmodule
