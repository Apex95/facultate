`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    01:37:42 01/06/2017 
// Design Name: 
// Module Name:    keyshifterinv 
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
module keyshifterinv(keysList, keysListInv, clk);
	input clk;
	input [0:895] keysList;
	output reg [0:895] keysListInv;

	reg [0:5] i;
	reg [0:15] aux;

	function [0:15] findInverse;
		input [0:15] key;
		reg [0:16] modulo, g0, g1, g2, v0, v1, v2, y;
		
		begin
			if (key <= 1)
				findInverse = key;
			else
				begin
				modulo = 17'h10001; // 2^16 + 1
				
				g0 = modulo; v0= 0;
				g1[0:16] = {1'b0,key[0:15]}; v1 = 1;
				g2 = 0; v2 = 0; y  = 0;
				
				
				while (g1 != 0)
					begin
					y = g0 / g1;
					g2 = g0 - y * g1;
					v2 = v0 - y * v1;
					
					//$display("%h %h %h\n", y, g2, v2);
					
					g0 = g1;
					g1 = g2;
					v0 = v1;
					v1 = v2;
					
					//$display("%h\n", v0);
					end
				
				// overflow
				if (v0[0] == 1)
					v0 = v0 + modulo;


				findInverse[0:15] = v0[1:16];
				end
		end
	endfunction

	always @(posedge clk)
		begin
		//  primele 4 subchei de decriptare
		keysListInv[0:15] = findInverse(keysList[768:783]); // 49
		keysListInv[16:31] = -keysList[784:799]; //50
		keysListInv[32:47] = -keysList[800:815]; // 51 
		keysListInv[48:63] = findInverse(keysList[816:831]); // 52

		
		
		
		for (i = 4; i < 47; i = i + 6)
			begin
			keysListInv[i*16 +: 16] = keysList[(52-i-2)*16 +: 16];
			keysListInv[(i+1)*16 +: 16] = keysList[(52-i-1)*16 +: 16];
			
			keysListInv[(i+2)*16 +: 16] = findInverse(keysList[(52-i-6)*16 +: 16]);
			keysListInv[(i+3)*16 +: 16] = -keysList[(52-i-4)*16 +: 16];
			keysListInv[(i+4)*16 +: 16] = -keysList[(52-i-5)*16 +: 16];
			keysListInv[(i+5)*16 +: 16] = findInverse(keysList[(52-i-3)*16 +: 16]);
			end
		
		keysListInv[832:895] = 0;
		
		// inversare la ultima linie de sub-chei
		aux = keysListInv[784+:16];
		keysListInv[784+:16] = keysListInv[800+:16];
		keysListInv[800+:16] = aux;
		
		end

endmodule
