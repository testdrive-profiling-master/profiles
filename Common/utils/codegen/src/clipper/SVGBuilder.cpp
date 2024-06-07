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
#include "SVGBuilder.h"

static const std::string __svg_xml_start[] = {"<?xml version=\"1.0\" standalone=\"no\"?>\n"
											  "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.0//EN\"\n"
											  "\"http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd\">\n\n"
											  "<svg width=\"",
											  "\" height=\"", "\" viewBox=\"0 0 ",
											  "\" version=\"1.0\" xmlns=\"http://www.w3.org/2000/svg\">\n\n"};
static const std::string __poly_end[]	   = {"\"\n style=\"fill:", "; fill-opacity:", "; fill-rule:", "; stroke:",
											  "; stroke-opacity:",	"; stroke-width:", ";\"/>\n\n"};

static string			 __ColorToHtml(unsigned clr)
{
	stringstream ss;
	ss << '#' << hex << std::setfill('0') << setw(6) << (clr & 0xFFFFFF);
	return ss.str();
}

static float __GetAlphaAsFrac(unsigned clr)
{
	return ((float)(clr >> 24) / 255);
}

SVGBuilder::StyleInfo::StyleInfo(void)
{
	pft		   = pftNonZero;
	brushClr   = 0xFFFFFFCC;
	penClr	   = 0xFF000000;
	penWidth   = 0.8;
	showCoords = false;
	offset.x   = 0;
	offset.y   = 0;
}

SVGBuilder::PolyInfo::PolyInfo(Paths paths, StyleInfo style)
{
	this->paths = paths;
	this->si	= style;
}

void SVGBuilder::AddPaths(Paths &poly, StyleInfo *pStyle)
{
	if (poly.size() == 0)
		return;

	if (pStyle) {
		polyInfos.push_back(PolyInfo(poly, *pStyle));
	} else {
		polyInfos.push_back(PolyInfo(poly, default_style));
	}
}

bool SVGBuilder::SaveToFile(const string &filename, double scale, int margin)
{
	// calculate the bounding rect ...
	PolyInfoList::size_type i = 0;
	Paths::size_type		j;

	while (i < polyInfos.size()) {
		j = 0;

		while (j < polyInfos[i].paths.size() && polyInfos[i].paths[j].size() == 0) j++;

		if (j < polyInfos[i].paths.size())
			break;

		i++;
	}

	if (i == polyInfos.size())
		return false;

	IntRect rec;
	rec.left   = polyInfos[i].paths[j][0].X;
	rec.right  = rec.left;
	rec.top	   = polyInfos[i].paths[j][0].Y;
	rec.bottom = rec.top;

	for (; i < polyInfos.size(); ++i) {
		StyleInfo &si = polyInfos[i].si;
		for (Paths::size_type j = 0; j < polyInfos[i].paths.size(); ++j) {
			for (Path::size_type k = 0; k < polyInfos[i].paths[j].size(); ++k) {
				IntPoint &ip = polyInfos[i].paths[j][k];
				double	  x	 = ip.X + si.offset.x;
				double	  y	 = ip.Y + si.offset.y;

				if (x < rec.left)
					rec.left = x;
				else if (x > rec.right)
					rec.right = x;

				if (y < rec.top)
					rec.top = y;
				else if (y > rec.bottom)
					rec.bottom = y;
			}
		}
	}

	if (scale == 0)
		scale = 1.0;

	if (margin < 0)
		margin = 0;

	rec.left		 = (cInt)((double)rec.left * scale);
	rec.top			 = (cInt)((double)rec.top * scale);
	rec.right		 = (cInt)((double)rec.right * scale);
	rec.bottom		 = (cInt)((double)rec.bottom * scale);
	cInt	 offsetX = -rec.left + margin;
	cInt	 offsetY = -rec.top + margin;
	ofstream file;
	file.open(filename);

	if (!file.is_open())
		return false;

	file.setf(ios::fixed);
	file.precision(0);
	file << __svg_xml_start[0] << ((rec.right - rec.left) + margin * 2) << "px" << __svg_xml_start[1]
		 << ((rec.bottom - rec.top) + margin * 2) << "px" << __svg_xml_start[2] << ((rec.right - rec.left) + margin * 2)
		 << " " << ((rec.bottom - rec.top) + margin * 2) << __svg_xml_start[3];
	setlocale(LC_NUMERIC, "C");
	file.precision(2);

	for (PolyInfoList::size_type i = 0; i < polyInfos.size(); ++i) {
		StyleInfo &si = polyInfos[i].si;
		file << " <path d=\"";
		for (Paths::size_type j = 0; j < polyInfos[i].paths.size(); ++j) {
			if (polyInfos[i].paths[j].size() < 3)
				continue;

			file << " M " << ((double)polyInfos[i].paths[j][0].X * scale + offsetX) << " "
				 << ((double)polyInfos[i].paths[j][0].Y * scale + offsetY);

			for (Path::size_type k = 1; k < polyInfos[i].paths[j].size(); ++k) {
				IntPoint &ip = polyInfos[i].paths[j][k];
				double	  x	 = (double)(ip.X + si.offset.x) * scale;
				double	  y	 = (double)(ip.Y + si.offset.y) * scale;
				file << " L " << (x + offsetX) << " " << (y + offsetY);
			}

			file << " z";
		}

		file << __poly_end[0] << __ColorToHtml(si.brushClr) << __poly_end[1] << __GetAlphaAsFrac(si.brushClr)
			 << __poly_end[2] << (si.pft == pftEvenOdd ? "evenodd" : "nonzero") << __poly_end[3]
			 << __ColorToHtml(si.penClr) << __poly_end[4] << __GetAlphaAsFrac(si.penClr) << __poly_end[5] << si.penWidth
			 << __poly_end[6];

		if (polyInfos[i].si.showCoords) {
			file << "<g font-family=\"Verdana\" font-size=\"11\" fill=\"black\">\n\n";

			for (Paths::size_type j = 0; j < polyInfos[i].paths.size(); ++j) {
				if (polyInfos[i].paths[j].size() < 3)
					continue;

				for (Path::size_type k = 0; k < polyInfos[i].paths[j].size(); ++k) {
					IntPoint ip = polyInfos[i].paths[j][k];
					file << "<text x=\"" << (int)((ip.X + si.offset.y) * scale + offsetX) << "\" y=\""
						 << (int)((ip.Y + si.offset.x) * scale + offsetY) << "\">" << (ip.X + si.offset.x) << ","
						 << (ip.Y + si.offset.y) << "</text>\n";
					file << "\n";
				}
			}

			file << "</g>\n";
		}
	}

	file << "</svg>\n";
	file.close();
	setlocale(LC_NUMERIC, "");
	return true;
}
