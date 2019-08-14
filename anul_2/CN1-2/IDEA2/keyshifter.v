`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    18:28:27 01/05/2017 
// Design Name: 
// Module Name:    keyshifter 
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
module keyshifter(inputKey, keysList, clk);
	input [0:127] inputKey;
	input clk;
	output reg [0:895] keysList;

	reg [0:5] i;
	
	always @(posedge clk)
		begin
		// shiftez 25 si adaug primii 25
		keysList[0:127] = inputKey[0:127];
		
		for (i = 1; i < 7; i = i + 1)
			begin
			keysList[128 * i +: 128] = (keysList[128*(i-1)+:128] << 25) | (keysList[128*(i-1)+:128] >> 103);
			end
		
		keysList[832:895] = 0; // doar 52 de sub-chei
		
		end


endmodule
