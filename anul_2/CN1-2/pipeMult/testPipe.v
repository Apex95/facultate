`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   00:08:30 05/24/2016
// Design Name:   pipeCalc
// Module Name:   /home/alexm/Desktop/pipeAdd/testPipe.v
// Project Name:  pipeAdd
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: pipeCalc
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module testPipe;

	// Inputs
	reg clk, rst;
	reg [0:31]x, y;

	// Outputs
	wire [0:31] out;
	
	//Auxiliary
	wire xs, ys, outs;
	wire [0:7]xe, ye, oute;
	wire [0:22]xm, ym, outm;
	
	parameter PERIOD = 100;

	// Instantiate the Unit Under Test (UUT)
	pipeDecomp decomp(clk, rst, x, y, xs, xe, xm, ys, ye, ym);
	pipeCalc calc(clk, rst, xs, xe, xm, ys, ye, ym, outs, oute, outm);
	pipeFin fin(clk, rst, outs, oute, outm, out);

	initial begin
		// Initialize Inputs
		clk = 1'b0;
		rst = 1'b1;        #(2*PERIOD);
      rst = 1'b0;        #PERIOD;

		// Wait 100 ns for global reset to finish
		x = 32'b00000000000000000000000000000000;
		y = 32'b01000001000110000000000000000000;
		#PERIOD;
		
		x = 32'b11000001100100000000000000000000;
		y = 32'b01000001000110000000000000000000;
		#PERIOD;
		
		x = 32'b01000101000100101001001000000000;
		y = 32'b00111111010000000000000000000000;
		#PERIOD;
		
		x = 32'b00111111100000000000000000000000;
		y = 32'b00111111100000000000000000000000;
		#PERIOD;
		
		x = 32'b10111111100000000000000000000000;
		y = 32'b00111111100000000000000000000000;
		#PERIOD;
        
		// Add stimulus here

	end
	
	// Clock process for clk
	always #(PERIOD*0.5) clk = ~clk;
      
endmodule

