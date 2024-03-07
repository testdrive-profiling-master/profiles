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
#include <chrono>
using namespace excel_number_format;

void		Excel_DateToYMD(int iDate, int &iYear, int &iMonth, int &iDay);

static void FormatPlaceholder(string token, char c, bool significant, string &result)
{
	if (token == "0") {
		if (significant)
			result += c;
		else
			result += "0";
	} else if (token == "#") {
		if (significant)
			result += c;
	} else if (token == "?") {
		if (significant)
			result += c;
		else
			result += " ";
	}
}

static char GetLeftAlignedValueDigit(string token, string valueString, int startIndex, bool significant,
									 int &valueIndex)
{

	char c;
	valueIndex = startIndex;

	if (valueIndex < valueString.length()) {
		c = valueString[valueIndex];
		valueIndex++;

		if (c != '0')
			significant = true;

		if (token == "?" && !significant) {
			// Eat insignificant zeros to left align denominator
			while (valueIndex < valueString.length()) {
				c = valueString[valueIndex];
				valueIndex++;

				if (c != '0') {
					significant = true;
					break;
				}
			}
		}
	} else {
		c			= '0';
		significant = false;
	}

	return c;
}

static void FormatLiteral(string token, string &result)
{
	string literal;
	if (!token.empty()) {
		if (token == ",") {
			; // skip commas
		} else if (token.length() == 2 && (token[0] == '*' || token[0] == '\\')) {
			// TODO: * = repeat to fill cell
			literal = token[1];
		} else if (token.length() == 2 && token[0] == '_') {
			literal = " ";
		} else if (token.at(0) == '\"') {
			literal = token.substr(1, token.length() - 2);
		} else {
			literal = token;
		}
	}
	result += literal;
}

// Adapted from ssf.js 'frac()' helper
void GetFraction(double x, int D, int &nom, int &den)
{

	int	   sgn = x < 0 ? -1 : 1;
	double B   = x * sgn;
	double P_2 = 0.0;
	double P_1 = 1.0;
	double P   = 0.0;
	double Q_2 = 1.0;
	double Q_1 = 0.0;
	double Q   = 0.0;
	double A   = floor(B);
	while (Q_1 < D) {
		A = floor(B);
		P = A * P_1 + P_2;
		Q = A * Q_1 + Q_2;
		if ((B - A) < 0.00000005)
			break;
		B	= 1 / (B - A);
		P_2 = P_1;
		P_1 = P;
		Q_2 = Q_1;
		Q_1 = Q;
	}
	if (Q > D) {
		if (Q_1 > D) {
			Q = Q_2;
			P = P_2;
		} else {
			Q = Q_1;
			P = P_1;
		}
	}
	nom = (int)(sgn * P);
	den = (int)Q;
}

static void FormatDenominator(string valueString, list<string> &tokens, string &result)
{

	int formatDigits = GetDigitCount(tokens);
	// padding '0' to left
	if (formatDigits > valueString.length()) {
		for (int i = 0; i < formatDigits - valueString.length(); i++) {
			valueString.insert(0, "0");
		}
	}

	bool significant = false;
	int	 valueIndex	 = 0;

	for (auto &i : tokens) {
		char c;
		if (valueIndex < valueString.length()) {
			c = GetLeftAlignedValueDigit(i, valueString, valueIndex, significant, valueIndex);

			if (c != '0')
				significant = true;
		} else {
			c			= '0';
			significant = false;
		}

		FormatPlaceholder(i, c, significant, result);
	}
}

static void FormatThousandSeparator(string valueString, int digit, string &result)
{
	int positionInTens = valueString.length() - 1 - digit;
	if (positionInTens > 0 && (positionInTens % 3) == 0) {
		result += ",";
	}
}

static void FormatThousands(string valueString, bool thousandSeparator, bool significantZero, list<string> &tokens,
							string &result)
{

	bool significant  = false;
	int	 formatDigits = GetDigitCount(tokens);

	// 최소 place holder 개수만큼 자릿수 확장.
	for (int i = valueString.length(); i < formatDigits; i++) {
		valueString.insert(0, "0");
	}

	// Print literals occurring before any placeholders
	auto it_token = tokens.begin();

	for (; it_token != tokens.end(); it_token++) {
		string &token = *it_token;
		if (IsPlaceholder(token)) {
			break;
		} else {
			FormatLiteral(token, result);
		}
	}

	// placeholder 개수 전까지 적용.
	int digitIndex = 0;
	for (; digitIndex < (int)(valueString.length() - formatDigits); digitIndex++) {
		significant = true;
		result += valueString[digitIndex];

		if (thousandSeparator)
			FormatThousandSeparator(valueString, digitIndex, result);
	}

	// Print remaining value digits and format literals
	for (; it_token != tokens.end(); it_token++) {
		string &token = *it_token;
		if (IsPlaceholder(token)) {
			char c = valueString[digitIndex];
			if (c != '0' || (significantZero && digitIndex == (int)(valueString.length() - 1)))
				significant = true;

			FormatPlaceholder(token, c, significant, result);

			if (thousandSeparator && (significant || token == "0"))
				FormatThousandSeparator(valueString, digitIndex, result);

			digitIndex++;
		} else {
			FormatLiteral(token, result);
		}
	}
}

static void FormatDecimals(string valueString, list<string> &tokens, string &result)
{
	bool significant	= true;
	int	 unpaddedDigits = valueString.length();
	int	 formatDigits	= GetDigitCount(tokens);

	for (int i = 0; i < formatDigits; i++) valueString += '0';

	// Print all format digits
	int valueIndex = 0;
	for (auto &token : tokens) {
		if (IsPlaceholder(token)) {
			char c		= valueString[valueIndex];
			significant = valueIndex < unpaddedDigits;

			FormatPlaceholder(token, c, significant, result);
			valueIndex++;
		} else {
			FormatLiteral(token, result);
		}
	}
}

static void FormatNumber(double value, list<string> &beforeDecimal, bool decimalSeparator, list<string> &afterDecimal,
						 bool thousandSeparator, cstring &result)
{

	int signitificantDigits = 0;
	if (!afterDecimal.empty())
		signitificantDigits = GetDigitCount(afterDecimal);
	cstring valueString;
	valueString.Format("%.*f", signitificantDigits, abs(value));
	cstring thousandsString(valueString);
	cstring decimalString(valueString);
	thousandsString.CutBack(".");
	decimalString.CutFront(".");
	decimalString.TrimRight("0");

	if (value < 0) {
		result.Append("-");
	}

	if (!beforeDecimal.empty()) {
		FormatThousands(thousandsString.c_string(), thousandSeparator, false, beforeDecimal, result.c_string());
	}

	if (decimalSeparator) {
		result.Append(".");
	}

	if (!afterDecimal.empty()) {
		FormatDecimals(decimalString.c_string(), afterDecimal, result.c_string());
	}
}

static bool LookAheadDatePart(list<string> &tokens, int fromIndex, cstring startsWith)
{
	startsWith.MakeLower(); // ignore case

	for (auto &token : tokens) {
		if (fromIndex)
			fromIndex--;
		else {
			cstring sTok(token);
			sTok.MakeLower();
			if (sTok.CompareFront(startsWith))
				return true;
			if (IsDatePart(sTok))
				return false;
		}
	}

	return false;
}

static bool LookBackDatePart(list<string> &tokens, int fromIndex, cstring startsWith)
{
	startsWith.MakeLower(); // ignore case

	// reverse traversal search
	fromIndex = tokens.size() - fromIndex - 1;

	for (auto i = tokens.rbegin(); i != tokens.rend(); i++) {
		if (fromIndex)
			fromIndex--;
		else {
			cstring sTok(*i);
			sTok.MakeLower();
			if (sTok.CompareFront(startsWith))
				return true;
			if (IsDatePart(sTok))
				return false;
		}
	}

	return false;
}

static string FormatNumber(double value, DecimalSection &format)
{
	bool thousandSeparator = format.ThousandSeparator;
	value				   = value / format.ThousandDivisor;
	value				   = value * format.PercentMultiplier;

	cstring result;
	FormatNumber(value, format.BeforeDecimal, format.DecimalSeparator, format.AfterDecimal, thousandSeparator, result);
	return result.c_string();
}

static string FormatDate(double value, list<string> &tokens)
{
	if (value < 0)
		return "####";

	// 날짜 계산
	int iYear, iMonth, iMDay, iWeek;
	{
		int iTotalDay = (int)floor(value);
		iWeek		  = (iTotalDay + 6) % 7; // start from sunday.
		Excel_DateToYMD(iTotalDay, iYear, iMonth, iMDay);
		value -= iTotalDay;
	}
	chrono::duration<double, ratio<60 * 60 * 24>> timeSpan(value); // 1 day.
	bool										  bAfterNoon;
	{
		chrono::hours hour = chrono::duration_cast<chrono::hours>(timeSpan);
		bAfterNoon		   = (hour.count() >= 12);
	}
	bool	bContainsAmPm = IsContainsAmPm(tokens);
	cstring result;
	auto	tok = tokens.begin();
	for (int i = 0; i < (int)tokens.size(); i++, tok++)

	{
		cstring token(*tok);
		cstring token_sym(token);
		token_sym.MakeLower();

		if (token_sym.CompareFront("y")) {
			// year
			int digits = token_sym.Length();
			if (digits < 2)
				digits = 2;
			if (digits == 3)
				digits = 4;

			int year = iYear;
			if (digits == 2)
				year = year % 100;

			result.AppendFormat("%0*d", digits, year);
		} else if (token_sym.CompareFront("m")) {
			// If  "m" or "mm" code is used immediately after the "h" or "hh" code (for hours) or immediately before
			// the "ss" code (for seconds), the application shall display minutes instead of the month.
			if (LookBackDatePart(tokens, i - 1, "h") || LookAheadDatePart(tokens, i + 1, "s")) {
				chrono::minutes mins   = chrono::duration_cast<chrono::minutes>(timeSpan);
				int				digits = token_sym.Length();
				result.AppendFormat("%0*d", digits, (int)(mins.count() % 60));
			} else {
				static const char *__sMonths[]		= {"January",	"February", "March",	"April",
													   "May",		"June",		"July",		"August",
													   "September", "October",	"November", "December"};
				static const char *__sMonthShorts[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
													   "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
				int				   digits			= token_sym.Length();
				if (digits == 3) {
					result.Append(__sMonthShorts[iMonth - 1]);
				} else if (digits == 4) {
					result.Append(__sMonths[iMonth - 1]);
				} else if (digits == 5) {
					result.Append(__sMonths[iMonth - 1][0]);
				} else {
					result.AppendFormat("%0*d", digits, iMonth);
				}
			}
		} else if (token_sym.CompareFront("d")) {
			int digits = token_sym.Length();
			if (digits == 3) {
				static const char *__sWeeks[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
				result.Append(__sWeeks[iWeek]);
			} else if (digits == 4) {
				static const char *__sWeeks[] = {"Sunday",	 "Monday", "Tuesday", "Wednesday",
												 "Thursday", "Friday", "Saturday"};
				result.Append(__sWeeks[iWeek]);
			} else {
				result.AppendFormat("%0*d", digits, iMDay);
			}
		} else if (token_sym.CompareFront("h")) {
			chrono::hours hour	 = chrono::duration_cast<chrono::hours>(timeSpan);
			int			  digits = token_sym.Length();
			if (bContainsAmPm)
				result.AppendFormat("%*d", digits, ((int)(hour.count() + 11) % 12 + 1));
			else
				result.AppendFormat("%0*d", digits, (int)hour.count());
		} else if (token_sym.CompareFront("s")) {
			chrono::seconds sec	   = chrono::duration_cast<chrono::seconds>(timeSpan);
			int				digits = token_sym.Length();
			result.AppendFormat("%0*d", digits, (int)sec.count() % 60);
		} else if (token_sym.CompareFront("g")) {
			// TODO: Should I care this?
			/*
			var era = culture.DateTimeFormat.Calendar.GetEra(date.AdjustedDateTime);
			int digits = token.Length();
			if (digits < 3)
			{
				result.Append(culture.DateTimeFormat.GetAbbreviatedEraName(era));
			}
			else
			{
				result.Append(culture.DateTimeFormat.GetEraName(era));
			}*/
		} else if (token_sym == "am/pm") {
			result.Append(bAfterNoon ? "PM" : "AM");
		} else if (token_sym == "a/p") {
			result.Append(bAfterNoon ? "P" : "A");
		} else if (token_sym.CompareFront(".0")) {
			chrono::milliseconds msec	= chrono::duration_cast<chrono::milliseconds>(timeSpan);
			int					 digits = token_sym.Length() - 1;
			result.AppendFormat("%0*d", digits, (int)(msec.count() % 1000));
		} else if (token_sym == "/") {
			result.Append("/");
		} else if (token_sym == ",") { // 한번만.
			for (++i; i < (int)(tokens.size() - 1); i++, tok++) {
				if (*tok != ",") {
					i--;
					tok--;
					break;
				}
			}

			result.Append(",");
		} else {
			FormatLiteral(token.c_string(), result.c_string());
		}
	}

	return result.c_string();
}

static string FormatTimeSpan(double value, list<string> &tokens)
{
	// NOTE/TODO: assumes there is exactly one [hh], [mm] or [ss] using the integer part of TimeSpan.TotalXXX when
	// formatting. The timeSpan input is then truncated to the remainder fraction, which is used to format mm and/or ss.

	chrono::duration<double, ratio<60 * 60 * 24>> timeSpan(value); // 1 day.

	cstring										  result;
	bool										  containsMilliseconds = false;
	for (auto &token : tokens) {
		cstring sTok(token);
		if (sTok.CompareFront(".0")) {
			containsMilliseconds = true;
			break;
		}
	}

	for (auto &token : tokens) {
		cstring sTok(token);
		sTok.MakeLower();

		if (sTok.CompareFront("m")) {
			chrono::minutes minutes = chrono::duration_cast<chrono::minutes>(timeSpan);
			result.AppendFormat("%0*lld", sTok.Length(), minutes.count());
		} else if (sTok.CompareFront("s")) {
			// If format does not include ms, then include ms in seconds and round before printing
			chrono::milliseconds msec = chrono::duration_cast<chrono::milliseconds>(timeSpan);
			int64_t				 sec  = msec.count();
			if (containsMilliseconds) {
				sec -= sec % 1000;
			}
			sec = (int64_t)round(sec / 1000.0);
			result.AppendFormat("%0*lld", sTok.Length(), sec);
		} else if (sTok.CompareFront("[h")) {
			chrono::hours hour = chrono::duration_cast<chrono::hours>(timeSpan);
			result.AppendFormat("%0*lld", sTok.Length() - 2, hour.count());

			timeSpan -= hour;
		} else if (sTok.CompareFront("[m")) {
			chrono::minutes mins = chrono::duration_cast<chrono::minutes>(timeSpan);
			result.AppendFormat("%0*lld", sTok.Length() - 2, mins.count());
			timeSpan -= mins;
		} else if (sTok.CompareFront("[s")) {
			chrono::seconds sec = chrono::duration_cast<chrono::seconds>(timeSpan);
			result.AppendFormat("%0*lld", sTok.Length() - 2, sec.count());
			timeSpan -= sec;
		} else if (sTok.CompareFront(".0")) {
			chrono::milliseconds msec = chrono::duration_cast<chrono::milliseconds>(timeSpan);
			result.AppendFormat("%0*lld", sTok.Length() - 1, msec.count());
		} else {
			FormatLiteral(sTok.c_string(), result.c_string());
		}
	}

	return result.c_string();
}

static string FormatGeneralText(string text, list<string> &tokens)
{
	cstring result;

	for (auto &i : tokens) {
		IsGeneral(i);
		if (IsGeneral(i) || i == "@") {
			result.Append(text.c_str());
		} else {
			FormatLiteral(i, result.c_string());
		}
	}
	return result.c_str();
}

static string FormatExponential(double value, Section &format)
{

	// The application shall display a number to the right of
	// the "E" symbol that corresponds to the number of places that
	// the decimal point was moved.

	int baseDigits = 0;
	if (!format.Exponential.BeforeDecimal.empty()) {
		baseDigits = GetDigitCount(format.Exponential.BeforeDecimal);
	}

	int	   exponent = (int)floor(log10(abs(value)));
	double mantissa = value / pow(10, exponent);

	int	   shift = abs(exponent) % baseDigits;
	if (shift > 0) {
		if (exponent < 0)
			shift = (baseDigits - shift);

		mantissa *= pow(10, shift);
		exponent -= shift;
	}

	cstring result;
	FormatNumber(mantissa, format.Exponential.BeforeDecimal, format.Exponential.DecimalSeparator,
				 format.Exponential.AfterDecimal, false, result);

	result += format.Exponential.ExponentialToken[0];

	if (format.Exponential.ExponentialToken[1] == '+' && exponent >= 0) {
		result += '+';
	} else if (exponent < 0) {
		result += '-';
	}

	{
		cstring sExponent;
		sExponent.Format("%d", abs(exponent));
		FormatThousands(sExponent.c_string(), false, false, format.Exponential.Power, result.c_string());
	}
	return result.c_string();
}

static string FormatFraction(double value, Section &format)
{
	int	 integral = 0;
	int	 numerator, denominator;

	bool sign = value < 0;

	if (format.Fraction.IntegerPart.size()) // get integer part
	{
		integral = (int)floor(value);
		value	 = abs(value - integral);
	}

	if (format.Fraction.DenominatorConstant != 0) {
		denominator = format.Fraction.DenominatorConstant;
		double rr	= round(value * denominator);
		double b	= floor(rr / denominator);
		numerator	= (int)(rr - b * denominator);
	} else {
		int denominatorDigits = min(GetDigitCount(format.Fraction.Denominator), 7);
		GetFraction(value, (int)pow(10, denominatorDigits) - 1, numerator, denominator);
	}

	// Don't hide fraction if at least one zero in the numerator format
	int		numeratorZeros = GetZeroCount(format.Fraction.Numerator);
	bool	hideFraction   = (format.Fraction.IntegerPart.size() != 0 && numerator == 0 && numeratorZeros == 0);

	cstring result;

	if (sign)
		result.Append("-");

	// Print integer part with significant zero if fraction part is hidden
	if (format.Fraction.IntegerPart.size()) {
		cstring sAbsInt;
		sAbsInt.Format("%d", (int)abs(integral));

		FormatThousands(sAbsInt.c_string(), false, hideFraction, format.Fraction.IntegerPart, result.c_string());
	}
	cstring numeratorString, denominatorString;
	numeratorString.Format("%d", abs(numerator));
	denominatorString.Format("%d", denominator);

	cstring fraction;
	FormatThousands(numeratorString.c_string(), false, true, format.Fraction.Numerator, fraction.c_string());
	fraction.Append("/");
	if (format.Fraction.DenominatorPrefix.size())
		FormatThousands("", false, false, format.Fraction.DenominatorPrefix, fraction.c_string());
	if (format.Fraction.DenominatorConstant)
		fraction.AppendFormat("%d", format.Fraction.DenominatorConstant);
	else
		FormatDenominator(denominatorString.c_string(), format.Fraction.Denominator, fraction.c_string());
	if (format.Fraction.DenominatorSuffix.size())
		FormatThousands("", false, false, format.Fraction.DenominatorSuffix, fraction.c_string());
	if (hideFraction) {
		for (int i = 0; i < fraction.size(); i++) result += ' ';
	} else
		result += fraction;
	if (format.Fraction.FractionSuffix.size())
		FormatThousands("", false, false, format.Fraction.FractionSuffix, result.c_string());
	return result.c_string();
}

string ExcelNumFormat::Format(double fValue, const char *sValue)
{

	Section *section = GetSection(fValue, sValue);

	if (!section) {
		m_sColor.clear();
		return "";
	}

	m_sColor = section->color;

	// Do format!
	switch (section->type) {
	case SECTION_TYPE_NUMBER:
		// Hide sign under certain conditions and section index

		if ((section->section_index == 0 && section->condition.IsValid()) || section->section_index == 1)
			fValue = abs(fValue);

		return FormatNumber(fValue, section->Number);

	case SECTION_TYPE_DATE:
		return FormatDate(fValue, section->GeneralTextDateDurationParts);

	case SECTION_TYPE_DURATION:
		return FormatTimeSpan(fValue, section->GeneralTextDateDurationParts);

	case SECTION_TYPE_GENERAL:
	case SECTION_TYPE_TEXT: {
		cstring sText;
		if (sValue) {
			sText = sValue;
		} else {
			sText.Format("%.9f", fValue);
			sText.TrimRight("0");
			sText.TrimRight(".");
		}
		return FormatGeneralText(sText.c_string(), section->GeneralTextDateDurationParts);
	}
	case SECTION_TYPE_EXPONENTIAL:
		return FormatExponential(fValue, *section);

	case SECTION_TYPE_FRACTION:
		return FormatFraction(fValue, *section);

	default:
		break;
	}

	return "";
}
