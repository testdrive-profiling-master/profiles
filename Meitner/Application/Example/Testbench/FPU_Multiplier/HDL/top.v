module top(
	input	[31:0]		A,
	input	[31:0]		B,
	output				Exception,
	output				Overflow,
	output				Underflow,
	output	[31:0]		O		              	//Outputs in the format of IEEE-754 Representation.
);

FPU_F32_Multiplier fpu (
	.A					(A),
	.B					(B),
	.Exception			(Exception),
	.Overflow			(Overflow),
	.Underflow			(Underflow),
	.O					(O)
);

endmodule
