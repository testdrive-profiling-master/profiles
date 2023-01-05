module MSI_Generator_v1_0 #(
	parameter integer	INTERRUPT_SIZE    = 4
) (
	input				CLK,
	input				nRST,
	input				INTR_0,
	input				INTR_1,
	input				INTR_2,
	input				INTR_3,
	input				INTR_4,
	input				INTR_5,
	input				INTR_6,
	input				INTR_7,
	input				MSI_EN,
	output				MSI_REQ,
	output	[4:0]		MSI_NUM,
	input				MSI_GRANT
);
// definition & assignment ---------------------------------------------------
wire	[7:0]	intr	= {
	INTR_7,
	INTR_6,
	INTR_5,
	INTR_4,
	INTR_3,
	INTR_2,
	INTR_1,
	INTR_0
};

// implementation ------------------------------------------------------------

MSI_Generator  msi_gen (
	.CLK			(CLK),
	.nRST			(nRST),
	.INTR			(intr),
	.MSI_EN			(MSI_EN),
	.MSI_REQ		(MSI_REQ),
	.MSI_NUM		(MSI_NUM),
	.MSI_GRANT		(MSI_GRANT)
);


endmodule
