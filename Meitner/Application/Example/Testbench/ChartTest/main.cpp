//================================================================================
// Copyright (c) 2013 ~ 2024. HyungKi Jeong(clonextop@gmail.com)
// Freely available under the terms of the 3-Clause BSD License
// (https://opensource.org/licenses/BSD-3-Clause)
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
// Title : Testbench
// Rev.  : 8/7/2024 Wed (clonextop@gmail.com)
//================================================================================
#include "Chart.h"
#include "TestDriver.inl"
#include <math.h>

int main(int argc, const char *argv[])
{
	Chart chart;

	if (!chart.Initialize()) { // initialization of chart
		printf("*W: To view a chart, first you must activate the chart document(System chart).\n");
		return 1;
	}

	chart.SetForeground(); // set chart document to foreground
	{
		chart.RemoveSerie();									  // remove all series
		chart.CreateSerie(0, "A");								  // Add Serie 'A'
		chart.CreateSerie(1, "B");								  // Add Serie 'B'
		chart.CreateSerie(2, "C", CHART_STYLE_LINE, FALSE, TRUE); // Add Serie 'C' (%)
		chart.SetWidth(1, 2);

		for (int i = 0; i <= 100; i++) {
			chart.AddPoint(0, i, i * 3);								 // "A"
			chart.AddPoint(1, i, 150 + 150 * cosf(3.141592f * i / 100)); // "B"
			chart.AddPoint(2, i, sinf(3.141592f * i / 20) * 30 + 50);	 // "C"
		}

		chart.SetLabel(0, 80, "serie 'A' : linear function!");
		chart.SetLabel(1, 80, "Just Bold 'B' serie");
		chart.SetLabel(2, 50, "serie 'C' : Sine wave~");
	}
	return 0;
}
