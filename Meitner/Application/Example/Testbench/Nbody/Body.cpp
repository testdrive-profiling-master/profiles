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
#include "Body.h"

double		Body::m_G			= GRAPVITY_CONSTANT;   // gravitational constant
double		Body::m_SolarMass	= SOLAR_MASS;

Body::Body(void)
{
	Initialize(0, 0, 0, 0, 1, 0);
}

void Body::Initialize(double x, double y, double vx, double vy, double mass, DWORD color)
{
	m_Pos.x			= x;
	m_Pos.y			= y;
	m_Velocity.x	= vx;
	m_Velocity.y	= vy;
	m_Mass			= mass;
	m_Color			= color;
	ResetForce();
}

Body::~Body(void)
{
}

void Body::Update(double ElapsedTime)
{
	m_Velocity.x	+= ElapsedTime * m_Force.x / m_Mass;
	m_Velocity.y	+= ElapsedTime * m_Force.y / m_Mass;
	m_Pos.x			+= ElapsedTime * m_Velocity.x;
	m_Pos.y			+= ElapsedTime * m_Velocity.y;
}

double Body::distanceTo(Body* body)
{
	double	dx = m_Pos.x - body->m_Pos.x;
	double	dy = m_Pos.y - body->m_Pos.y;
	return sqrt(dx * dx + dy * dy);
}

void Body::ResetForce(void)
{
	m_Force.x	= 0;
	m_Force.y	= 0;
}

void Body::AddForce(Body* body)
{
	double	EPS		= 1E-40;	// softening parameter (just to avoid infinities)
	double	dx		= body->m_Pos.x - m_Pos.x;
	double	dy		= body->m_Pos.y - m_Pos.y;
	double	dist	= sqrt(dx * dx + dy * dy);
	double	F		= (m_G * body->m_Mass * m_Mass) / (dist * dist + EPS * EPS);
	m_Force.x		+= F * dx / dist;
	m_Force.y		+= F * dy / dist;
}
