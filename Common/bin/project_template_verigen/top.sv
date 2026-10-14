// Testbench top
`include "testdrive_system.svh"

/* HIDDEN */
module top (
	input						nRST,
	input						MCLK,
	output						INTR,
	output						BUSY
);

// definition & assignment ---------------------------------------------------
initial
	SetSystemDescription("__PROJECT__");

assign	BUSY		= `FALSE;
// implementation ------------------------------------------------------------


endmodule
