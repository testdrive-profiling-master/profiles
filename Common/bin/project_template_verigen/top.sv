// Testbench top
`include "testdrive_system.vh"

/* HIDDEN */
module top (
	input						nRST,
	input						MCLK,
	output						INTR,
	output						BUSY
);

// definition & assignment ---------------------------------------------------
initial
	SetSystemDescription("PROJECT");

assign	BUSY		= `FALSE;
// implementation ------------------------------------------------------------


endmodule
