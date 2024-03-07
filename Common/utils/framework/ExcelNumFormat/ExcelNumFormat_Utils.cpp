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

namespace excel_number_format
{
bool IsLiteral(string token)
{
	return (token.at(0) == '_') || (token.at(0) == '\\') || (token.at(0) == '\"') || (token.at(0) == '*') ||
		   token == "," || token == "!" || token == "&" || token == "%" || token == "+" || token == "-" ||
		   token == "$" || token == "€" || token == "£" || token == "1" || token == "2" || token == "3" ||
		   token == "4" || token == "5" || token == "6" || token == "7" || token == "8" || token == "9" ||
		   token == "{" || token == "}" || token == "(" || token == ")" || token == " ";
}

bool IsNumberLiteral(string token)
{
	return IsPlaceholder(token) || IsLiteral(token) || token == ".";
}

bool IsExponent(cstring token)
{
	token.MakeLower();
	return token == "e+" || token == "e-";
}

bool IsPlaceholder(string token)
{
	return token == "0" || token == "#" || token == "?";
}

bool IsGeneral(cstring token)
{
	token.MakeLower();
	return token == "general";
}

bool IsDatePart(cstring token)
{
	if (token.IsEmpty())
		return false;
	token.MakeLower();
	return (token[0] == 'y') || (token[0] == 'm') || (token[0] == 'd') || (token[0] == 's') || (token[0] == 'h') ||
		   ((token[0] == 'g') && !IsGeneral(token)) || (token.find("am/pm") == 0) || (token.find("a/p") == 0) ||
		   IsDurationPart(token);
}

bool IsDurationPart(cstring token)
{
	token.MakeLower();
	return (token.find("[h") == 0) || (token.find("[m") == 0) || (token.find("[s") == 0);
}

bool IsDigit19(string token)
{
	if (token.length() == 1) {
		return (token[0] >= '1') && (token[0] <= '9');
		/*switch (token.at(0)) {
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			return true;
		default:
			return false;
		}*/
	}
	return false;
}

bool IsDigit09(string token)
{
	return token == "0" || IsDigit19(token);
}

int GetDigitCount(list<string> &tokens)
{
	int counter = 0;

	for (auto &i : tokens) {
		if (IsPlaceholder(i)) {
			counter++;
		}
	}
	return counter;
}

int GetZeroCount(list<string> &tokens)
{
	int counter = 0;

	for (auto &i : tokens) {
		if (i == "0")
			counter++;
	}

	return counter;
}

bool IsContainsAmPm(list<string> &tokens)
{
	for (auto &token : tokens) {
		cstring stok(token);
		stok.MakeLower();
		if (stok == "am/pm" || stok == "a/p")
			return true;
	}

	return false;
}
}; // namespace excel_number_format
