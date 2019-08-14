`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    23:53:20 05/23/2016 
// Design Name: 
// Module Name:    pipeDecomp 
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
module pipeDecomp(clk, rst, x, y, out_xs, out_xe, out_xm, out_ys, out_ye, out_ym);
	input clk, rst;
	input [0:31]x, y;
	output reg out_xs, out_ys;
	output reg [0:7]out_xe, out_ye;
	output reg [0:22]out_xm, out_ym;
	
	reg xs, ys;
	reg [0:7]xe, ye;
	reg [0:22]xm, ym;
	
	always @(posedge clk or posedge rst) begin
		if(rst) begin
			out_xs = 0;
			out_ys = 0;
			out_xe = 0;
			out_ye = 0;
			out_xm = 0;
			out_ym = 0;
		end
		
		else begin
			out_xs = xs;
			out_ys = ys;
			out_xe = xe;
			out_ye = ye;
			out_xm = xm;
			out_ym = ym;
		end
	end
	
	always @(*) begin
		if(x[0] == 1 && y[0] == 0) begin
			xs = 0;
			xe = y[1:8];
			xm = y[9:31];
			ys = 1;
			ye = x[1:8];
			ym = x[9:31];
		end
		
		else begin
			xs = x[0];
			xe = x[1:8];
			xm = x[9:31];
			ys = y[0];
			ye = y[1:8];
			ym = y[9:31];
		end
	end

endmodule