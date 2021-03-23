`include "system_defines.vh"

/* HIDDEN */
module top (
	input						nRST,
	input						MCLK,
	output						INTR,
	output						BUSY
);

// definition & assignment ---------------------------------------------------
assign	BUSY		= `FALSE;

reg		[31:0]		reg_A, reg_Temp;

`DPI_FUNCTION void Test_ALU(input bit [31:0] A, output bit [31:0] O);
// implementation ------------------------------------------------------------

always@(posedge MCLK, negedge nRST) begin
	if(!nRST) begin
		reg_A		<= 0;
	end
	else begin
		Test_ALU(reg_A, reg_Temp);
		reg_A		<= reg_Temp;
	end
end

endmodule
