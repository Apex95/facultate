`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    10:03:05 04/21/2016 
// Design Name: 
// Module Name:    booth_inmultire 
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
module booth_inmultire(
    input wire[3:0] m,
	 input wire[3:0] r,
	 output wire[7:0] p);

wire[8:0] A;
assign A = m << 5;

wire[8:0] S;
// (-m) ???
assign S = ((~m)+1) << 5;
wire[7:0] P;
assign P = r << 1;

wire[2:0] rez;
reg[3:0] i;
reg[8:0] P1;
reg sign;

always @(*)
begin
	P1 = P;
	for (i = 0; i < 4; i = i + 1)
	begin
		
		 if (P1[1:0] == 2'b01)
			P1 = P1 + A;
		else if (P1[1:0] == 2'b10)
			P1 = P1 + S;
		
		sign = P1[8];
		P1 = P1 >> 1;
		P1[8] = sign;
	end
end

assign p = P1[8:1];


endmodule
