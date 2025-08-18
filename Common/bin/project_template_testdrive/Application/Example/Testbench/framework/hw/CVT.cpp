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
// Title : Testbench
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#include "CVT.h"
#include <math.h>

CVT::CVT(void)
{
	memset(&m_Mode, 0, sizeof(m_Mode));
}

CVT::~CVT(void)
{
}

CVT_MODE* CVT::Refresh(int iWidth, int iHeight, int iRefreshFreq, BOOL bReducedBlank)
{
	double h_period_est;
	double v_sync_bp;
	double h_blank;
	double cur_duty_cycle;
	double vbi_lines, act_vbi_lines, rb_min_vbi;
	m_Mode.h.active		= iWidth & (~7);		// horizontal active pixels
	m_Mode.v.active		= iHeight & (~7);		// vertical active lines
	m_Mode.v.front		= 3;					// vertical front porch
	// vertical sync lines
	{
		typedef struct {
			double	screen_ratio;
			int		vsync_count;
		} VSYNC_DATA;
		const VSYNC_DATA	__sync_list[] = {
			{4.0 / 3.0,		4},		// 4:3
			{16.0 / 9.0,	5},		// 16:9
			{16.0 / 10.0,	6},		// 16:10
			{5.0 / 4.0,		7},		// 5:4
			{15.0 / 9.0,	7},		// 15:9
			{0,				10},	// custom (unknown ratio)
		};

		for(int i = 0;; i++) {
			bool	bMatched	= (!__sync_list[i].screen_ratio) || (m_Mode.h.active == (int)(m_Mode.v.active * __sync_list[i].screen_ratio));

			if(bMatched) {
				m_Mode.v.sync	= __sync_list[i].vsync_count;
				break;
			}
		}
	}

	if(bReducedBlank) {	// reduced blank mode
		h_blank 			= 160;			// minimum horizontal blanking pixels
		m_Mode.h.sync		= 32;			// minimum horizontal sync
		m_Mode.v.back		= 6;			// minimum vertical back porch
		h_period_est		= (1000000.0 / iRefreshFreq - 460.0) / iHeight;		// estimate horizontal period (min 460us)
		vbi_lines			= floor(460.0 / h_period_est) + 1;					// vertical blanking lines
		rb_min_vbi			= m_Mode.v.front + m_Mode.v.sync + m_Mode.v.back;	// minimum VBI lines
		act_vbi_lines		= (vbi_lines < rb_min_vbi) ? rb_min_vbi : vbi_lines;
		m_Mode.v.total		= iHeight + act_vbi_lines;						// vertical total lines
		m_Mode.v.back		= act_vbi_lines - m_Mode.v.sync - m_Mode.v.front;
	} else {    // Normal blanking mode
		h_period_est		= ((1.0 / iRefreshFreq) - 550.0 / 1000000.0) / (iHeight + m_Mode.v.front) * 1000000.0;	// estimate horizontal period (min 550us)
		v_sync_bp			= floor(550.0 / h_period_est) + 1;
		v_sync_bp			= (v_sync_bp < m_Mode.v.sync + 3) ? (m_Mode.v.sync + 3) : v_sync_bp;
		m_Mode.v.back		= (int)(v_sync_bp - m_Mode.v.sync);				// vertical back porch
		m_Mode.v.total		= iHeight + v_sync_bp + m_Mode.v.front;			// vertical total lines
		cur_duty_cycle		= 30.0 - (300.0 * h_period_est / 1000.0);		// ideal blank duty cycle(%)

		if(cur_duty_cycle < 20) cur_duty_cycle = 20;		// minimum 20% higher

		h_blank				= floor((m_Mode.h.active * cur_duty_cycle / (100.0 - cur_duty_cycle) / (16.0))) * (16.0);	// total horizontal blank
		m_Mode.h.sync		= ((int)floor(0.08 * (m_Mode.h.active + h_blank))) & (~7);	// horizontal sync. normal(8% of total width)
	}

	// total horizental pixels
	m_Mode.h.total		= m_Mode.h.active + h_blank;
	// Horizontal back porch
	m_Mode.h.back		= h_blank / 2;
	// Horizontal front porch
	m_Mode.h.front		= (int)(h_blank - m_Mode.h.back - m_Mode.h.sync);
	// pixel clocks(MHz)
	m_Mode.pixel_clk	= (int)((iRefreshFreq * m_Mode.v.total * m_Mode.h.total) / 1000000.0);
	return &m_Mode;
}
