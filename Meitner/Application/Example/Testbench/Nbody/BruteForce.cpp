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
#include "BruteForce.h"
#include <time.h>
#include <omp.h>

BruteForce::BruteForce(void)
{
	m_pBodies	= NULL;
	srand(time(0));
}

BruteForce::~BruteForce(void)
{
	SAFE_DELETE_ARRAY(m_pBodies);
}

static inline double MathRandom(void)
{
	return (double) rand() / RAND_MAX;
}

double BruteForce::MathSignum(double n)
{
	if(!n) return 0;

	return n > 0 ? 1 : -1;
}

double BruteForce::Exp(double lambda)
{
	return -log(1.0 - MathRandom()) / lambda;
}

double BruteForce::circlev(double rx, double ry)
{
	double solarmass	= 1.98892e30;
	double r2			= sqrt(rx * rx + ry * ry);
	double numerator	= (6.67e-11) * 1e6 * solarmass;
	return sqrt(numerator / r2);
}

void BruteForce::Initialize(int N)
{
	SAFE_DELETE_ARRAY(m_pBodies);
	m_iSize		= N;

	if(!m_iSize) return;

	m_pBodies	= new Body[N];
	double		radius		= 1e18;		// radius of universe
	double		solarmass	= 1.98892e30;

	for(int i = 0; i < N; i++) {
		double px = 1e18 * Exp(-1.8) * (0.5 - MathRandom());
		double py = 1e18 * Exp(-1.8) * (0.5 - MathRandom());
		double magv = circlev(px, py);
		double absangle = atan(abs(py / px));
		double thetav = M_PI / 2 - absangle;
		double phiv = MathRandom() * M_PI;
		double vx   = -1 * MathSignum(py) * cos(thetav) * magv;
		double vy   = MathSignum(px) * sin(thetav) * magv;
		//Orient a random 2D circular orbit

		if(MathRandom() <= 0.5) {
			vx = -vx;
			vy = -vy;
		}

		double mass = MathRandom() * solarmass * 10 + 1e20;
		//Color the masses in green gradients by mass
		int red			= (int) floor(mass * 254 / (solarmass * 10 + 1e20));
		int blue		= (int) floor(mass * 254 / (solarmass * 10 + 1e20));
		int green		= 255;
		DWORD color		= (blue << 16) | (green << 8) | red;
		m_pBodies[i].Initialize(px, py, vx, vy, mass, color);
	}

	//Put the central mass in
	m_pBodies[0].Initialize(0, 0, 0, 0, 1e6 * solarmass, 0xFF0000); //put a heavy body in the center
}

void BruteForce::Update(void)
{
	#pragma omp parallel for

	for(int i = 0; i < m_iSize; i++) {
		Body* pBody	= &m_pBodies[i];
		pBody->ResetForce();

		// Notice-2 loops-->N^2 complexity
		for(int j = 0; j < m_iSize; j++) {
			if(i != j) pBody->AddForce(&m_pBodies[j]);
		}
	}

	#pragma omp parallel for

	for(int i = 0; i < m_iSize; i++) {
		m_pBodies[i].Update(1e11);
	}
}

void BruteForce::Present(void)
{
	for(int i = 0; i < m_iSize; i++) {
		OnPresent(&m_pBodies[i]);
	}

	Update();
}
