module MSI_interrupt_pipe (
	// system
	input 					CLK,			// clock
	input 					nRST,			// reset (active low)
	// control
	input 					INTR,			// interrupt input
	input 					CLEAR,			// clear
	output reg				REQ				// equest enable

);

// definition & assignment ---------------------------------------------------
reg		[1:0]				intr_pipe;

// implementation ------------------------------------------------------------
// interrupt pipe & request control
always@(posedge CLK, negedge nRST) begin
	if(!nRST) begin
		intr_pipe	<= 2'd0;
		REQ			<= 1'b0;
	end else begin
		intr_pipe	<= {intr_pipe[0], INTR};
		REQ			<= (intr_pipe == 2'b01) | (REQ & (~CLEAR));	// high edge check
	end
end

endmodule
