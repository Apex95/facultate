`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    17:49:10 01/05/2017 
// Design Name: 
// Module Name:    round 
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
module round(text, key, clk, encryptedText, swap);
	input [0:63] text;
	input [0:127] key;
	input clk;
	output reg [0:63] encryptedText;
	input swap;
	
	reg[0:15] A,B,C,D,E,F, aux,
				 key1, key2, key3, key4, key5, key6;
	
	always @(*)
	begin
		//$display("%h %h\n", text, key[0:95]);
	
		// impartire text in 4 block-uri
		A[0:15] = text[0:15];
		B[0:15] = text[16:31];
		C[0:15] = text[32:47];
		D[0:15] = text[48:63];
		
		// generare 6 sub-chei
		key1[0:15] = key[0:15];
		key2[0:15] = key[16:31];
		key3[0:15] = key[32:47];
		key4[0:15] = key[48:63];
		key5[0:15] = key[64:79];
		key6[0:15] = key[80:95];


		$display("%h %h %h %h %h %h\n", key1, key2, key3, key4, key5, key6);

	
		A = A * key1 % 65537;
		B = (B + key2) % 65536;
		C = (C + key3) % 65536;
		D = D * key4 % 65537;
		
		
		E = A ^ C;
		F = B ^ D; //6
		
		E = E * key5 % 65537; //7
		F = (F + E) % 65536; //8
		
		F = F * key6 % 65537; //9
		E = (E + F) % 65536; //10
		
		A = A ^ F;
		C = C ^ F;
		B = B ^ E;
		D = D ^ E;
		
		if (swap)
			begin
			aux = B;
			B = C;
			C = aux;
			end

	end
	
	always @(posedge clk)
	begin
		encryptedText = {A, B, C, D};
	end


endmodule
