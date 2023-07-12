module top(
	input	[31:0]		A,
	input	[31:0]		B,
	output				Exception,
	output	[31:0]		O		              	//Outputs in the format of IEEE-754 Representation.
);

FPU_F32_Adder fpu (
	.A					(A),
	.B					(B),
	.Exception			(Exception),
	.O					(O)
);

endmodule
