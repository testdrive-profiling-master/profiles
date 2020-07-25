//================================================================================
// Copyright (c) 2013 ~ 2019. HyungKi Jeong(clonextop@gmail.com)
// All rights reserved.
// 
// The 3-Clause BSD License (https://opensource.org/licenses/BSD-3-Clause)
// 
// Redistribution and use in source and binary forms,
// with or without modification, are permitted provided
// that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
// BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
// OF SUCH DAMAGE.
// 
// Title : Meitner processor v1.1
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
`include "MTSP_Defines.vh"

module MTSP_InstCoordinate(	UINSTx4,
							SRC0A,
							UINSTx4_OUT,
							REF_ADDR_DEST, REF_ADDR_RELATIVE);

// port list -----------------------------------------------------------------
// Instruction
input	[`RANGE_UINSTx4]		UINSTx4;							// input instructions
input	[`RANGE_DWORDx4]		SRC0A;

// Source
output	[`RANGE_UINSTx4]		UINSTx4_OUT;						// output instructions
output	[`RANGE_GPRs]			REF_ADDR_DEST;
output	[`RANGE_INDEX]			REF_ADDR_RELATIVE;

// definition & assignment ---------------------------------------------------
wire	[`RANGE_UINST]				p0_m, p0_s, p1_m, p1_s;			// input unit instruction

wire								addr_nEN, pred_nEN,				// address override enable
p0_disable;
wire	[`RANGE_MASK4D]				pred_mask;
wire	[`RANGE_DWORD]				s0a_x, s0a_y, s0a_z, s0a_w;
assign	{p0_m, p0_s, p1_m, p1_s}	= UINSTx4;						// ��ɾ� �и�
assign	addr_nEN					= p0_m[`RANGE_UINST_nEN] | (p0_m[`RANGE_UINST_MO]!=`MO_ADDR),	// phase #0-Main ��ɾ��� ADDR ����
pred_nEN					= p0_m[`RANGE_UINST_nEN] | (p0_m[`RANGE_UINST_MO]!=`MO_PRED);	// phase #0-Main ��ɾ��� PRED ����
assign	{s0a_x, s0a_y, s0a_z, s0a_w}	= SRC0A;					// phase #0 source A ��ġ

// implementation ------------------------------------------------------------
// etc. : �������� address ��� �ּ� ����
assign	REF_ADDR_DEST				= ((addr_nEN | p0_m[7]) ? 0 : s0a_z[`RANGE_GPRs]),
REF_ADDR_RELATIVE			= ((addr_nEN | p0_m[6]) ? 0 : s0a_w[`RANGE_INDEX]);
// Predicate ���
assign	pred_mask[`SELECT_X]		= ~|(s0a_x[`RANGE_MASK4D]&p0_m[`RANGE_UINST_CMPX]),
pred_mask[`SELECT_Y]		= ~|(s0a_y[`RANGE_MASK4D]&p0_m[`RANGE_UINST_CMPY]),
pred_mask[`SELECT_Z]		= ~|(s0a_z[`RANGE_MASK4D]&p0_m[`RANGE_UINST_CMPZ]),
pred_mask[`SELECT_W]		= ~|(s0a_w[`RANGE_MASK4D]&p0_m[`RANGE_UINST_CMPW]);
//assign	p0_disable					= p1_m[`RANGE_UINST_nALU] & (|pred_mask) & pred_nEN;	// phase #1 ��ɾ ��ȿȭ �Ǵ� ���
assign	p0_disable					= (&pred_mask) & (~pred_nEN);	// phase #1 ��ɾ ��ȿȭ �Ǵ� ��� : pred ��ɾ� �����ϸ鼭 ���� mask�ɶ� phase 1 ��ɾ� ���
// Predicate ����
assign	UINSTx4_OUT					= {p0_m, p0_s,
						  p1_m[`RANGE_UINST_nEN]|p0_disable,					// ALU ��ɾ �ƴҶ� predicate ó��
						  p1_m[30:12],
						  p1_m[`RANGE_UINST_WMASK]|((pred_nEN | p1_m[`RANGE_UINST_nALU]) ? 4'b0 : pred_mask),	// Predication�� WMASK ó��(ALU ��ɾ��� ���)
						  p1_m[7:0],

						  p1_s[`RANGE_UINST_nEN]|p0_disable,					// ALU ��ɾ �ƴҶ� predicate ó��
						  p1_s[30:0]};
endmodule
