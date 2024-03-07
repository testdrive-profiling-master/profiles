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

bool DecimalSection::TryParse(list<string> &tokens)
{
	if (ParseNumberTokens(tokens, 0, BeforeDecimal, DecimalSeparator, AfterDecimal) == tokens.size()) {
		ThousandDivisor	  = GetTrailingCommasDivisor(tokens, ThousandSeparator);
		PercentMultiplier = GetPercentMultiplier(tokens);

		return true;
	}

	return false;
}

double DecimalSection::GetPercentMultiplier(list<string> &tokens)
{
	// If there is a percentage literal in the part list, multiply the result by 100
	for (auto &i : tokens) {
		if (i == "%")
			return 100;
	}

	return 1;
}

double DecimalSection::GetTrailingCommasDivisor(list<string> &tokens, bool &thousandSeparator)
{

	// This parses all comma literals in the part list:
	// Each comma after the last digit placeholder divides the result by 1000.
	// If there are any other commas, display the result with thousand separators.
	bool   hasLastPlaceholder = false;
	double divisor			  = 1.0;

	int	   tokenIndex = tokens.size();
	for (auto j = tokens.rbegin(); j != tokens.rend(); j++) {
		string token = *j;

		if (!hasLastPlaceholder) {
			if (IsPlaceholder(token)) {
				// Each trailing comma multiplies the divisor by 1000
				{
					int i_pos = 0;

					for (auto &k : tokens) {
						if (i_pos >= tokenIndex) {
							if (k == ",")
								divisor *= 1000.0;
							else
								break;
						}
						i_pos++;
					}
				}

				// Continue scanning backwards from the last digit placeholder,
				// but now look for a thousand separator comma
				hasLastPlaceholder = true;
			}
		} else {
			if (token == ",") {
				thousandSeparator = true;
				return divisor;
			}
		}
		tokenIndex--;
	}

	thousandSeparator = false;
	return divisor;
}
