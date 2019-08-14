`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    10:46:10 04/21/2016 
// Design Name: 
// Module Name:    UAL 
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
module UAL(
	input wire[3:0] a,
	input wire[3:0] b,
	input wire[3:0] op,
	output reg[7:0] c
    );
	 
	 //initial
	 ///begin
	//	c = 0;
	 //end
	 
	wire[7:0] caux;
	 booth_inmultire inm(a, b, caux);
	 
	 //op = 0 -> inmultire
	 //op = 1 -> adunare
	 //op = 2 -> scadere
	 //op = 3 -> and
	 //op = 4 -> or
	 //op = 5 -> xor
	 //op = 6 -> nand
	 //op = 7 ->nor
	 
	 always @(*)
	 begin
		case (op)
			1 : c = a + b;
			2 : c = a - b;
			3 : c = a & b;
			4 : c = a | b;
			5 : c = a ^ b;
			6 : c = ~(a & b);
			7 : c = ~(a | b);
			default: c =  caux;
		endcase
	 end
	 


endmodule
