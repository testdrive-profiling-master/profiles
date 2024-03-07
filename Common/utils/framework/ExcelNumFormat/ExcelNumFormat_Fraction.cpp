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

bool FractionSection::TryParse(list<string> &tokens)
{
	list<string> numeratorParts;
	list<string> denominatorParts;

	{
		bool bDenominator = false;
		for (auto &i : tokens) {
			if (i == "/") {
				bDenominator = true;
				continue;
			}

			if (bDenominator)
				denominatorParts.push_back(i);
			else
				numeratorParts.push_back(i);
		}
	}

	if (numeratorParts.empty()) {
		return false;
	}

	GetNumerator(numeratorParts);

	if (!TryGetDenominator(denominatorParts)) {
		return false;
	}

	return true;
}

void FractionSection::GetNumerator(list<string> &tokens)
{
	bool hasPlaceholder = false;
	bool hasSpace		= false;
	bool hasIntegerPart = false;
	int	 numeratorIndex = -1;
	int	 index			= tokens.size() - 1;

	for (auto i = tokens.rbegin(); i != tokens.rend(); i++) {
		string token = *i;
		if (IsPlaceholder(token)) {
			hasPlaceholder = true;

			if (hasSpace) {
				hasIntegerPart = true;
				break;
			}
		} else {
			if (hasPlaceholder && !hasSpace) {
				// First time we get here marks the end of the integer part
				hasSpace	   = true;
				numeratorIndex = index + 1;
			}
		}
		index--;
	}

	if (hasIntegerPart) {
		index = 0;
		for (auto &i : tokens) {
			if (index < numeratorIndex) {
				IntegerPart.push_back(i);
			} else {
				Numerator.push_back(i);
			}
			index++;
		}
	} else {
		IntegerPart.clear();
		Numerator = tokens;
	}
}

bool FractionSection::TryGetDenominator(list<string> &tokens)
{

	int	   index		  = 0;
	bool   hasPlaceholder = false;
	bool   hasConstant	  = false;

	string constant;

	// Read literals until the first number placeholder or digit
	for (auto &i : tokens) {
		if (IsPlaceholder(i)) {
			hasPlaceholder = true;
			break;
		} else if (IsDigit19(i)) {
			hasConstant = true;
			break;
		}
		index++;
	}

	if (!hasPlaceholder && !hasConstant) {
		DenominatorConstant = 0;
		return false;
	}

	// The denominator starts here, keep the index
	int denominatorIndex = index;

	// Read placeholders or digits in sequence
	{
		int i_pos = 0;
		for (auto &i : tokens) {
			if (i_pos >= index) {
				if (hasPlaceholder && IsPlaceholder(i)) {
					; // OK
				} else if (hasConstant && (IsDigit09(i))) {
					constant += i;
				} else {
					break;
				}
				index++;
			}
			i_pos++;
		}
	}

	// 'index' is now at the first token after the denominator placeholders.
	// The remaining, if anything, is to be treated in one or two parts:
	// Any ultimately terminating literals are considered the "Fraction suffix".
	// Anything between the denominator and the fraction suffix is the "Denominator suffix".
	// Placeholders in the denominator suffix are treated as insignificant zeros.

	// Scan backwards to determine the fraction suffix
	int fractionSuffixIndex = tokens.size();
	for (auto i = tokens.rbegin(); i != tokens.rend(); i++) {
		if ((fractionSuffixIndex <= index) || IsPlaceholder(*i))
			break;

		fractionSuffixIndex--;
	}

	// Finally extract the detected token ranges

	if (denominatorIndex > 0) {
		int i_pos = 0;
		for (auto &i : tokens) {
			if (i_pos >= denominatorIndex)
				break;
			DenominatorPrefix.push_back(i);
			i_pos++;
		}
	} else {
		DenominatorPrefix.clear();
	}

	if (hasConstant) {
		DenominatorConstant = atoi(constant.c_str());
	} else {
		DenominatorConstant = 0;
	}

	{
		int i_pos = 0;
		for (auto &i : tokens) {
			if (i_pos >= index - denominatorIndex)
				break;
			if (i_pos >= denominatorIndex)
				Denominator.push_back(i);
			i_pos++;
		}
	}

	if (index < fractionSuffixIndex) {
		int i_pos = 0;
		for (auto &i : tokens) {
			if (i_pos >= fractionSuffixIndex - index)
				break;
			if (i_pos >= index)
				DenominatorSuffix.push_back(i);
			i_pos++;
		}
	} else {
		DenominatorSuffix.clear();
	}

	if (fractionSuffixIndex < tokens.size()) {
		int i_pos = 0;
		for (auto &i : tokens) {
			if (i_pos >= tokens.size() - fractionSuffixIndex)
				break;
			if (i_pos >= fractionSuffixIndex)
				FractionSuffix.push_back(i);
			i_pos++;
		}
	} else {
		FractionSuffix.clear();
	}

	return true;
}
