`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    00:06:16 05/24/2016 
// Design Name: 
// Module Name:    pipeFin 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module pipeFin(clk, rst, outs, oute, outm, out_out);
	input clk, rst, outs;
	input [0:7]oute;
	input [0:22]outm;
	output reg [0:31]out_out;
	
	reg [0:31]out;
	
	always @(posedge clk or posedge rst) begin
		if(rst)
			out_out = 0;
		else
			out_out = out;
	end

	always @(*) begin
		out[0] = outs;
		out[1:8] = oute;
		out[9:31] = outm;
	
		$display("res = %b", out);
	end
endmodule
