`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   10:56:09 04/21/2016
// Design Name:   UAL
// Module Name:   C:/Users/Student 2/booth/UAL_test.v
// Project Name:  booth
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: UAL
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module UAL_test;

	// Inputs
	reg [3:0] a;
	reg [3:0] b;
	reg [3:0]op;

	// Outputs
	wire [7:0] c;

	// Instantiate the Unit Under Test (UUT)
	UAL uut (
		.a(a), 
		.b(b), 
		.op(op), 
		.c(c)
	);

	initial begin
		// Initialize Inputs
		a = 4;
		b = 2;
		op = 0;

		// Wait 100 ns for global reset to finish
		#100;
        
		a = 4;
		b = 2;
		op = 1;
		  
			#100;
        
		a = 4;
		b = 2;
		op = 2;

		#100;
        
		a = 4;
		b = 2;
		op = 3;
		
		#100;
        
		a = 4;
		b = 2;
		op = 4;
		

		#100;
        
		a = 4;
		b = 2;
		op = 5;

		#100;
        
		a = 4;
		b = 2;
		op = 6;
		
		#100;
        
		a = 4;
		b = 2;
		op = 7;
		
		
		// Add stimulus here

	end
      
endmodule

