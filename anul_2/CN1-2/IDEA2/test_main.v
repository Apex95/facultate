`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   18:17:09 01/05/2017
// Design Name:   main
// Module Name:   D:/IDEA2/test_main.v
// Project Name:  IDEA2
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: main
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module test_main;

	// Inputs
	reg [0:63] text;
	reg [0:127] key;
	reg action;
	reg clk;

	// Outputs
	wire [0:63] encryptedText;

	// Instantiate the Unit Under Test (UUT)
	main uut (
		.text(text), 
		.key(key), 
		.action(action), 
		.clk(clk), 
		.encryptedText(encryptedText)
	);

	initial begin
		// Initialize Inputs
		
		// pt criptare
		
		text = 64'hf5db1ac45e5ef9f9;
		key = 128'h9d4075c103bc322afb03e7be6ab30006;
		
		
		
		
		action = 1;
		clk = 0;

		
        
		// Add stimulus here

	end
   always #(5) clk = ~clk;
	
	always #(500)
		begin
		
		// pt decriptare
		text = 64'h53bff7278ac667db;
		action = 0;
		end
endmodule

