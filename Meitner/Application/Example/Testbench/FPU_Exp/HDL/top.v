module top(
	input	[31:0]		A,
	output				Exception,
	output	[31:0]		O		              	//Outputs in the format of IEEE-754 Representation.
);

FPU_F32_Exponential fpu (
	.A					(A),
	.Exception			(Exception),
	.O					(O)
);

endmodule
