`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    00:01:46 05/24/2016 
// Design Name: 
// Module Name:    pipeCalc 
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
module pipeCalc(clk, rst, xs, xe, xm, ys, ye, ym, out_outs, out_oute, out_outm);
	input clk, rst, xs, ys;
	input [0:7]xe, ye;
	input [0:22]xm, ym;
	output reg out_outs;
	output reg [0:7]out_oute;
	output reg [0:22]out_outm;
	
	reg outs;
	reg [0:7]oute;
	reg [0:22]outm;
	reg [0:47]aux;
	reg [0:23]aux_xm, aux_ym;
	
	always @(posedge clk or posedge rst) begin
		if(rst) begin
			out_outs = 0;
			out_oute = 0;
			out_outm = 0;
		end
		
		else begin
			out_outs = outs;
			out_oute = oute;
			out_outm = outm;
		end
	end
	
	always @(*) begin
		//Unul din factori este 0
		if((xs == 0 && xe == 0 && xm == 0) || (ys == 0 && ye == 0 && ym == 0)) begin
			outs = 0;
			oute = 0;
			outm = 0;
		end
		
		//Niciun factor nu este 0
		else begin
			outs = xs ^ ys;
			aux_xm[1:23] = xm;
			aux_xm[0] = 1;
			aux_ym[1:23] = ym;
			aux_ym[0] = 1;
			aux = aux_xm * aux_ym;
			
			while(aux[0] == 0)
				aux = (aux << 1);
				
			outm = aux[1:23];
			oute = xe + ye - 127;
		end
	end
endmodule