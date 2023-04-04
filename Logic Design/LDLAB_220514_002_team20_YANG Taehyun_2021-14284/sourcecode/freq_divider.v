`timescale 1ns / 1ps

module freq_divider(
	input clk,
	input clr,
	output reg clkout = 0
    );
	 
	 reg [31:0] cnt = 0;
	 always@ (posedge clk, posedge clr) begin
		if(clr) begin
			cnt <= 0;
			clkout <= 0;
		end
		else if(cnt == 32'd25000000) begin
			cnt <= 0;
			clkout <= ~clkout;
		end
		else begin
			cnt <= cnt+1;
		end
	end

endmodule
