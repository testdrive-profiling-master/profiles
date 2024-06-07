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
// Title : Common profiles
// Rev.  : 6/7/2024 Fri (clonextop@gmail.com)
//================================================================================
#ifndef __SVG_BUILDER_H__
#define __SVG_BUILDER_H__
#include <cmath>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "clipper.hpp"

using namespace std;
using namespace ClipperLib;

//---------------------------------------------------------------------------
// SVGBuilder class
// a very simple class that creates an SVG image file
//---------------------------------------------------------------------------
class SVGBuilder
{
public:
	class StyleInfo
	{
	public:
		PolyFillType pft;
		unsigned	 brushClr;
		unsigned	 penClr;
		double		 penWidth;
		bool		 showCoords;
		struct {
			double x, y;
		} offset;

		StyleInfo();
	};

	class PolyInfo
	{
	public:
		Paths	  paths;
		StyleInfo si;

		PolyInfo(Paths paths, StyleInfo style);
	};

private:
	typedef std::vector<PolyInfo> PolyInfoList;

	StyleInfo					  default_style;
	PolyInfoList				  polyInfos;

public:
	void AddPaths(Paths &poly, StyleInfo *pStyle = NULL);
	bool SaveToFile(const string &filename, double scale = 1.0, int margin = 10);
};

#endif //__SVG_BUILDER_H__
