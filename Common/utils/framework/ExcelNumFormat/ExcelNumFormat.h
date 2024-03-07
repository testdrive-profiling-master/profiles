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
#ifndef __EXCEL_NUM_FORMAT_H__
#define __EXCEL_NUM_FORMAT_H__
#include "UtilFramework.h"

namespace excel_number_format
{
typedef enum {
	SECTION_TYPE_GENERAL,
	SECTION_TYPE_NUMBER,
	SECTION_TYPE_FRACTION,
	SECTION_TYPE_EXPONENTIAL,
	SECTION_TYPE_DATE,
	SECTION_TYPE_DURATION,
	SECTION_TYPE_TEXT
} SectionType;

class Tokenizer
{
public:
	cstring formatString;
	int		formatStringPosition;

public:
	Tokenizer(const char *fmt)
	{
		formatString		 = fmt;
		formatStringPosition = 0;
	}

	inline int Position(void)
	{
		return formatStringPosition;
	}

	inline int Length(void)
	{
		return formatString.Length();
	}

	string Substring(int startIndex, int length)
	{
		return formatString.c_string().substr(startIndex, length);
	}

	int Peek(int offset = 0)
	{
		if (formatStringPosition + offset >= Length())
			return -1;
		return formatString[formatStringPosition + offset];
	}

	int PeekUntil(int startOffset, int until)
	{
		int offset = startOffset;
		while (true) {
			int c = Peek(offset++);
			if (c == -1)
				break;
			if (c == until)
				return offset - startOffset;
		}
		return 0;
	}

	bool PeekOneOf(int offset, string s)
	{
		for (char c : s) {
			if (Peek(offset) == c) {
				return true;
			}
		}
		return false;
	}

	void Advance(int characters = 1)
	{
		formatStringPosition += characters;
		if (formatStringPosition > Length())
			formatStringPosition = Length();
	}

	bool ReadOneOrMore(int c)
	{
		if (Peek() != c)
			return false;

		while (Peek() == c) Advance();

		return true;
	}

	bool ReadOneOf(string s)
	{
		if (PeekOneOf(0, s)) {
			Advance();
			return true;
		}
		return false;
	}

	bool ReadString(string s, bool ignoreCase = false)
	{
		if (formatStringPosition + s.length() > Length())
			return false;

		for (int i = 0; i < s.length(); i++) {
			char c1 = s[i];
			char c2 = (char)Peek(i);
			if (ignoreCase) {
				if (tolower(c1) != tolower(c2))
					return false;
			} else {
				if (c1 != c2)
					return false;
			}
		}

		Advance(s.length());
		return true;
	}

	bool ReadEnclosed(char open, char close)
	{
		if (Peek() == open) {
			int length = PeekUntil(1, close);
			if (length > 0) {
				Advance(1 + length);
				return true;
			}
		}

		return false;
	}
};

class ExponentialSection
{
public:
	list<string> BeforeDecimal;
	bool		 DecimalSeparator;
	list<string> AfterDecimal;
	string		 ExponentialToken;
	list<string> Power;
	bool		 TryParse(list<string> &tokens);
};

class FractionSection
{
public:
	list<string> IntegerPart;
	list<string> Numerator;
	list<string> DenominatorPrefix;
	list<string> Denominator;
	int			 DenominatorConstant;
	list<string> DenominatorSuffix;
	list<string> FractionSuffix;

	bool		 TryParse(list<string> &tokens);
	void		 GetNumerator(list<string> &tokens);
	bool		 TryGetDenominator(list<string> &tokens);
};

class DecimalSection
{
public:
	bool		 ThousandSeparator;
	double		 ThousandDivisor;
	double		 PercentMultiplier;
	list<string> BeforeDecimal;
	bool		 DecimalSeparator;
	list<string> AfterDecimal;

	bool		 TryParse(list<string> &tokens);
	double		 GetPercentMultiplier(list<string> &tokens);
	double		 GetTrailingCommasDivisor(list<string> &tokens, bool &thousandSeparator);
};

typedef enum {
	CONDITION_NONE,
	CONDITION_LT, // less than "<"
	CONDITION_LE, // less or equal "<="
	CONDITION_GT, // greater than ">"
	CONDITION_GE, // greater or equal ">="
	CONDITION_NE, // not equal "<>"
	CONDITION_EQ, // equal "="
} CONDITION;

class Condition
{
public:
	Condition(void)
	{
		Operator = CONDITION_NONE;
		Value	 = 0;
	}

	CONDITION Operator;
	double	  Value;
	bool	  Evaluate(double lhs)
	{
		switch (Operator) {
		case CONDITION_LT:
			return lhs < Value;
		case CONDITION_LE:
			return lhs <= Value;
		case CONDITION_GT:
			return lhs > Value;
		case CONDITION_GE:
			return lhs >= Value;
		case CONDITION_NE:
			return lhs != Value;
		case CONDITION_EQ:
			return lhs == Value;
		default:
			break;
		}

		return false;
	}
	inline bool IsValid(void)
	{
		return Operator != CONDITION_NONE;
	}
};

class Section
{
public:
	Section(void)
	{
		section_index = -1;
		type		  = SECTION_TYPE_GENERAL;
	}
	~Section(void) {}
	int				   section_index;
	SectionType		   type;
	string			   color;
	Condition		   condition;
	ExponentialSection Exponential;
	FractionSection	   Fraction;
	DecimalSection	   Number;
	list<string>	   GeneralTextDateDurationParts;
};

class ExcelNumFormat
{
public:
	ExcelNumFormat(const char *sFormat);
	~ExcelNumFormat(void);

	void		  Release(void);

	string		  Format(double fValue, const char *sValue = NULL);
	inline string Color(void)
	{
		return m_sColor;
	}
	inline bool IsValid(void)
	{
		return !m_Sections.empty();
	}

protected:
	Section *GetSection(double fValue, const char *sValue);
	bool	 ParseSections(const char *sFormat);
	bool	 ParseSection(Section *pSection, Tokenizer &reader);

private:
	cstring			m_sFormat;
	string			m_sColor;
	bool			IsDateTimeFormat;
	bool			IsTimeSpanFormat;
	list<Section *> m_Sections;
};

bool IsLiteral(string token);
bool IsNumberLiteral(string token);
bool IsExponent(cstring token);
bool IsPlaceholder(string token);
bool IsGeneral(cstring token);
bool IsDatePart(cstring token);
bool IsDurationPart(cstring token);
bool IsDigit19(string token);
bool IsDigit09(string token);
int	 GetDigitCount(list<string> &tokens);
int	 GetZeroCount(list<string> &tokens);
bool IsContainsAmPm(list<string> &tokens);

int	 ParseNumberTokens(list<string> &tokens, int startPosition, list<string> &beforeDecimal, bool &decimalSeparator,
					   list<string> &afterDecimal);
}; // namespace excel_number_format

#endif //__EXCEL_NUM_FORMAT_H__
