`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   10:27:21 04/21/2016
// Design Name:   booth_inmultire
// Module Name:   C:/Users/Student 2/booth/test_booth.v
// Project Name:  booth
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: booth_inmultire
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module test_booth;

	// Inputs
	reg [3:0] m;
	reg [3:0] r;

	// Outputs
	wire [7:0] p;

	// Instantiate the Unit Under Test (UUT)
	booth_inmultire uut (
		.m(m), 
		.r(r), 
		.p(p)
	);

	initial begin
		// Initialize Inputs
		m = 4;
		r = 2;

		// Wait 100 ns for global reset to finish
		#100;
        
		// Add stimulus here

	end
      
endmodule

