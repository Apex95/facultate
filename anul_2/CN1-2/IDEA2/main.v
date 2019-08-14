`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    17:48:59 01/05/2017 
// Design Name: 
// Module Name:    main 
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
module main(text, key, action, clk, encryptedText);
	input[0:63] text;
	input[0:127] key;
	input action;
	input clk;
	output[0:63] encryptedText;
	
	wire [0:895] keysList, keysListInv, keys;
	wire [0:63] encryptedText1, encryptedText2, encryptedText3, encryptedText4, encryptedText5, encryptedText6, encryptedText7, encryptedText8;
	
	keyshifter ks1(key, keysList, clk);
	keyshifterinv ksi1(keysList, keysListInv, clk);
	
	assign keys = action == 1 ? keysList : keysListInv;
	
	// action == 1 => criptare; altfel decriptare
	round r1(text, keys[0:127], clk, encryptedText1, 1);
	round r2(encryptedText1, keys[127-32+1:127-32+128], clk, encryptedText2, 1);
	round r3(encryptedText2, keys[127-32+96+1:127-32+128+96], clk, encryptedText3, 1);
	round r4(encryptedText3, keys[127-32+96*2+1:127-32+128+96*2], clk, encryptedText4, 1);
	round r5(encryptedText4, keys[127-32+96*3+1:127-32+128+96*3], clk, encryptedText5, 1);
	round r6(encryptedText5, keys[127-32+96*4+1:127-32+128+96*4], clk, encryptedText6, 1);
	round r7(encryptedText6, keys[127-32+96*5+1:127-32+128+96*5], clk, encryptedText7, 1);
	round r8(encryptedText7, keys[127-32+96*6+1:127-32+128+96*6], clk, encryptedText8, 0);
	round rfinal(encryptedText8, keys[127-32+96*7+1:127-32+128+96*7], clk, encryptedText, 0);
	
endmodule
