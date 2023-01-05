//=======================================================================
// CloneX developments. Copyright 2013~2016. ALL RIGHTS RESERVED
// Title : Processor
// Rev.  : 7/18/2016 Mon (clonextop@gmail.com)
//=======================================================================
module MSI_Generator (
	input					CLK,
	input					nRST,
	input	[7:0]			INTR,
	input					MSI_EN,
	output	reg 			MSI_REQ,
	output	reg [4:0]		MSI_NUM,
	input					MSI_GRANT
);
// definition & assignment ---------------------------------------------------
genvar i;
wire	[7:0]				reqs;
reg		[7:0]				clear;
reg							busy;

// implementation ------------------------------------------------------------
generate
for(i=0;i<8;i=i+1) begin : intr_gen
	MSI_interrupt_pipe	intr_pipe (
		.CLK			(CLK),
		.nRST			(nRST),
		.INTR			(INTR[i]),
		.CLEAR			(clear[i] & busy & MSI_GRANT),
		.REQ			(reqs[i])
	);
end
endgenerate

// toss to PCIe MSI interface
always@(posedge CLK, negedge nRST) begin
	if(!nRST) begin
		MSI_REQ		<= 1'b0;
		MSI_NUM		<= 5'd0;
		busy		<= 1'b0;
		clear		<= 8'd0;
	end
	else begin
		if(!busy) begin
			if(MSI_EN && (|reqs)) begin
				MSI_REQ		<= 1'b1;
				MSI_NUM		<= (
					reqs[0] ? 'd0 :
					reqs[1] ? 'd1 :
					reqs[2] ? 'd2 :
					reqs[3] ? 'd3 :
					reqs[4] ? 'd4 :
					reqs[5] ? 'd5 :
					reqs[6] ? 'd6 :
					/* ~[7]*/ 'd7
				);
				clear		<= (
					reqs[0] ? 8'b00000001 :
					reqs[1] ? 8'b00000010 :
					reqs[2] ? 8'b00000100 :
					reqs[3] ? 8'b00001000 :
					reqs[4] ? 8'b00010000 :
					reqs[5] ? 8'b00100000 :
					reqs[6] ? 8'b01000000 :
					/* ~[7]*/ 8'b10000000
				);
				busy		<= 1'b1;
			end
		end
		else begin
			MSI_REQ		<= 1'b0;
			if(MSI_GRANT) begin
				busy		<= 1'b0;
			end
		end
	end
end

endmodule
