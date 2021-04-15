`include "system_defines.vh"
`include "top/testdrive_clock_bfm.sv"
`include "top/testdrive_busy_bfm.sv"
`include "top/testdrive_interrupt_bfm.sv"

module top ();

// definition & assignment ---------------------------------------------------
wire	MCLK, nRST;
wire	BUSY		= 1'b0;
wire	INTR		= 1'b0;

// implementation ------------------------------------------------------------
// Clock instance
testdrive_clock_bfm #(
	.C_CLOCK_PERIOD_PS		(10000),	// 10ns clock  period, 100MHz
	.C_RESET_POLARITY		(0)			// active low reset
) clk_inst (
	.CLK					(MCLK),
	.RST					(nRST)
);

// Busy instance
testdrive_busy_bfm busy_inst (			// if BUSY is asserted, simulation is go on continuously.
	.BUSY					(BUSY)
);

// interrupt instance
testdrive_interrupt_bfm #(
	.C_EDGE_DETECT			(1),		// edge detection
	.C_ACTIVE				(1)			// active high
) intr_inst (
	.CLK					(MCLK),
	.nRST					(nRST),
	.EN						(1'b1),
	.INTR					(INTR)
);

// Simple DPI
`DPI_FUNCTION void HelloWorld();

initial begin
	HelloWorld();
end

endmodule
