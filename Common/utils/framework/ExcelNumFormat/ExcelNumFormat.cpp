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
// Rev.  : 3/7/2024 Thu (clonextop@gmail.com)
//================================================================================
#include "ExcelNumFormat.h"
using namespace excel_number_format;

ExcelNumFormat::ExcelNumFormat(const char *sFormat)
{
	if (!ParseSections(sFormat)) {
		Release();
	}
}

ExcelNumFormat::~ExcelNumFormat(void)
{
	Release();
}

void ExcelNumFormat::Release(void)
{
	for (auto &i : m_Sections) delete i;
	m_Sections.clear();
}

Section *ExcelNumFormat::GetSection(double fValue, const char *sValue)
{
	bool bText = (sValue != NULL);
	if (bText) { // get text section
		if (m_Sections.size() >= 4) {
			auto it = m_Sections.begin();
			it++;
			it++;
			it++; // step to #4

			return (*it);
		} else if (m_Sections.size()) {
			Section *pSection = *(m_Sections.begin());

			if (pSection->type == SECTION_TYPE_TEXT) {
				return pSection;
			}
		}

		return NULL;
	}

	// get numeric section
	{
		// First section applies if
		// - Has a condition:
		// - There is 1 section, or
		// - There are 2 sections, and the value is 0 or positive, or
		// - There are >2 sections, and the value is positive
		if (!m_Sections.size())
			return NULL;

		auto	 it		  = m_Sections.begin();
		Section *section0 = *it;

		if (section0->condition.IsValid()) {
			if (section0->condition.Evaluate(fValue)) {
				return section0;
			}
		} else if (m_Sections.size() == 1 || (m_Sections.size() == 2 && fValue >= 0) ||
				   (m_Sections.size() >= 2 && fValue > 0)) {
			return section0;
		}

		if (m_Sections.size() < 2) {
			return NULL;
		}

		Section *section1 = *(++it);

		// First condition didnt match, or was a negative number. Second condition applies if:
		// - Has a condition, or
		// - Value is negative, or
		// - There are two sections, and the first section had a non-matching condition
		if (section1->condition.IsValid()) {
			if (section1->condition.Evaluate(fValue)) {
				return section1;
			}
		} else if (fValue < 0 || (m_Sections.size() == 2 && section0->condition.IsValid())) {
			return section1;
		}

		// Second condition didnt match, or was positive. The following
		// sections cannot have conditions, always fall back to the third
		// section (for zero formatting) if specified.
		if (m_Sections.size() < 3) {
			return NULL;
		}

		return (*(++it));
	}
}
