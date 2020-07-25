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
#ifndef __BODY_H__
#define __BODY_H__
#include "STDInterface.h"
#include <math.h>

#define SOLAR_MASS				1.98892e30
#define GRAPVITY_CONSTANT		6.673e-11

typedef struct{
	double		x,y;
}BODY_POSITION;

class Body{
public:
	Body(void);
	~Body(void);

	void Initialize(double x, double y, double vx, double vy, double mass, DWORD color = 0xFFFFFF);

	void Update(double ElapsedTime);
	double distanceTo(Body* body);

	void ResetForce(void);
	void AddForce(Body* body);

	inline BODY_POSITION&	Position(void)	{return m_Pos;}
	inline BODY_POSITION&	Velocity(void)	{return m_Velocity;}
	inline DWORD			Color(void)	{return m_Color;}

private:
	static double		m_G;		// gravitational constant
	static double		m_SolarMass;
	BODY_POSITION		m_Pos;
	BODY_POSITION		m_Velocity;
	BODY_POSITION		m_Force;
	double				m_Mass;
	DWORD				m_Color;
};

#endif//__BODY_H__
