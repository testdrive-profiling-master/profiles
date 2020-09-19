//================================================================================
// Copyright (c) 2013 ~ 2020. HyungKi Jeong(clonextop@gmail.com)
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
// Title : Testbench
// Rev.  : 9/19/2020 Sat (clonextop@gmail.com)
//================================================================================
#include "JuliaSet.h"

JuliaSet::JuliaSet(void)
{
	SetMaxIteration(300);
	SetConstant(0.27015, -0.7);
	SetMatrix(640, 480);
	SetZoom(1.0);
}

JuliaSet::~JuliaSet(void)
{
}

void JuliaSet::SetMaxIteration(int iIterations)
{
	m_iMaxIterations	= iIterations;
}

void JuliaSet::SetZoom(double zoom)
{
	m_Zoom	= zoom;
}

void JuliaSet::SetPosition(double x, double y)
{
	m_Position.x	= x;
	m_Position.y	= y;
}

void JuliaSet::SetConstant(double Imaginary, double Real)
{
	m_Constant.Imaginary	= Imaginary;
	m_Constant.Real			= Real;
}

void JuliaSet::SetMatrix(int iWidth, int iHeight)
{
	m_iWidth	= iWidth;
	m_iHeight	= iHeight;
}

static DWORD HSVtoRGB(int h, int s, int v)
{
	// h : 0 ~ 360
	DWORD r = 0, g = 0, b = 0;
	int	t1	= v;
	int	t2	= (255 - s) * v / 255;
	int	t3	= (t1 - t2) * (h % 60) / 60;

	if(h < 60) {
		r	= t1;
		g	= t2;
		b	= t2 + t3;
	} else if(h < 120) {
		g	= t1;
		b	= t2;
		r	= t1 - t3;
	} else if(h < 180) {
		g	= t1;
		r	= t2;
		b	= t2 + t3;
	} else if(h < 240) {
		b	= t1;
		r	= t2;
		g	= t1 - t3;
	} else if(h < 300) {
		b	= t1;
		g	= t2;
		r	= t2 + t3;
	} else if(h < 360) {
		r	= t1;
		g	= t2;
		b	= t1 - t3;
	}

	return ((b & 0xFF) << 16) | ((g & 0xFF) << 8) | (r & 0xFF);
}

DWORD JuliaSet::RunJuliaSet(int x, int y)
{
	double	Real		= 1.5 * ((double)x - (double)(m_iWidth / 2)) / (0.5 * m_Zoom * m_iWidth) + m_Position.x;
	double Imaginary	= ((double)y - (double)(m_iHeight / 2)) / (0.5 * m_Zoom * m_iHeight) + m_Position.y;
	double R, I;
	DWORD	color, i;

	for(i = 0; i < m_iMaxIterations; i++) {
		R			= Real;
		I			= Imaginary;
		Real		= R * R - I * I + m_Constant.Real;
		Imaginary	= 2 * R * I + m_Constant.Imaginary;

		if((Real * Real + Imaginary * Imaginary) > 4) break;
	}

	color	= HSVtoRGB(i % 360, 255, 255 * (i < m_iMaxIterations));
	return color;
}

void JuliaSet::Present(void)
{
	for(int y = 0; y < m_iHeight; y++) {
		for(int x = 0; x < m_iWidth; x++) {
			OnPresent(x, y, RunJuliaSet(x, y));
		}
	}
}
