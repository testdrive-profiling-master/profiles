#include "ExcelNumFormat.h"
using namespace excel_number_format;

namespace excel_number_format {

static bool ReadLiteral(Tokenizer& reader)
{
	if (reader.Peek() == '\\' || reader.Peek() == '*' || reader.Peek() == '_')
	{
		reader.Advance(2);
		return true;
	}
	else if (reader.ReadEnclosed('"', '"'))
	{
		return true;
	}

	return false;
}

static void ParseMilliseconds(list<string>& tokens, list<string>& result)
{
	// if tokens form .0 through .000.., combine to single subsecond token

	for (auto i = tokens.begin();i != tokens.end();i++)
	{
		string token = *i;
		if (token == ".")
		{
			int zeros = 0;
			auto i_next = i;i_next++;
			while ((i_next != tokens.end()) && *i_next == "0")
			{
				i++;
				i_next++;
				zeros++;
			}

			if (zeros > 0)
				result.push_back("." + string('0', zeros));
			else
				result.push_back(".");
		}
		else
		{
			result.push_back(token);
		}
	}
}

static string ReadToken(Tokenizer& reader, bool& syntaxError)
{
	int offset = reader.Position();
	if (
		ReadLiteral(reader) ||
		reader.ReadEnclosed('[', ']') ||

		// Symbols
		reader.ReadOneOf("#?,!&%+-$€£0123456789{}():;/.@ ") ||
		reader.ReadString("e+", true) ||
		reader.ReadString("e-", true) ||
		reader.ReadString("General", true) ||

		// Date
		reader.ReadString("am/pm", true) ||
		reader.ReadString("a/p", true) ||
		reader.ReadOneOrMore('y') ||
		reader.ReadOneOrMore('Y') ||
		reader.ReadOneOrMore('m') ||
		reader.ReadOneOrMore('M') ||
		reader.ReadOneOrMore('d') ||
		reader.ReadOneOrMore('D') ||
		reader.ReadOneOrMore('h') ||
		reader.ReadOneOrMore('H') ||
		reader.ReadOneOrMore('s') ||
		reader.ReadOneOrMore('S') ||
		reader.ReadOneOrMore('g') ||
		reader.ReadOneOrMore('G'))
	{
		syntaxError = false;
		int length = reader.Position() - offset;

		return reader.Substring(offset, length);
	}

	syntaxError = reader.Position() < reader.Length();
	return "";
}

static bool ReadConditionValue(Tokenizer& tokenizer)
{
	// NFPartCondNum = [ASCII-HYPHEN-MINUS] NFPartIntNum [INTL-CHAR-DECIMAL-SEP NFPartIntNum] [NFPartExponential NFPartIntNum]
	tokenizer.ReadString("-");
	while (tokenizer.ReadOneOf("0123456789"))
	{
	}

	if (tokenizer.ReadString("."))
	{
		while (tokenizer.ReadOneOf("0123456789"))
		{
		}
	}

	if (tokenizer.ReadString("e+", true) || tokenizer.ReadString("e-", true))
	{
		if (tokenizer.ReadOneOf("0123456789"))
		{
			while (tokenizer.ReadOneOf("0123456789"))
			{
			}
		}
		else
		{
			return false;
		}
	}

	return true;
}

static bool TryParseCondition(string token, Condition& result)
{
	Tokenizer tokenizer(token.c_str());

	if (tokenizer.ReadString("<=") ||
		tokenizer.ReadString("<>") ||
		tokenizer.ReadString("<") ||
		tokenizer.ReadString(">=") ||
		tokenizer.ReadString(">") ||
		tokenizer.ReadString("="))
	{
		int conditionPosition = tokenizer.Position();
		string op = tokenizer.Substring(0, conditionPosition);

		if (ReadConditionValue(tokenizer))
		{
			string valueString = tokenizer.Substring(conditionPosition, tokenizer.Position() - conditionPosition);

			if(op == "<") result.Operator	= CONDITION_LT;
			else if(op == "<=") result.Operator	= CONDITION_LE;
			else if(op == ">") result.Operator	= CONDITION_GT;
			else if(op == ">=") result.Operator	= CONDITION_GE;
			else if(op == "<>") result.Operator	= CONDITION_NE;
			else result.Operator	= CONDITION_EQ;

			result.Value	= atof(valueString.c_str());
			return true;
		}
	}

	return false;
}



static bool TryParseColor(string token, string& color)
{
	Tokenizer	tokenizer(token.c_str());
	if (
		tokenizer.ReadString("black", true) ||
		tokenizer.ReadString("blue", true) ||
		tokenizer.ReadString("cyan", true) ||
		tokenizer.ReadString("green", true) ||
		tokenizer.ReadString("magenta", true) ||
		tokenizer.ReadString("red", true) ||
		tokenizer.ReadString("white", true) ||
		tokenizer.ReadString("yellow", true))
	{
		color = tokenizer.Substring(0, tokenizer.Position());
		return true;
	}

	color.clear();
	return false;
}

static bool TryParseCurrencySymbol(string token, string& currencySymbol)
{

	if (token.empty() || token.at(0) != '$')
	{
		currencySymbol.clear();
		return false;
	}

	if (token.find('-') >= 0)
		currencySymbol = token.substr(1, token.find('-') - 1);
	else
		currencySymbol = token.substr(1);

	return true;
}


int ParseNumberTokens(list<string>& tokens, int startPosition, list<string>& beforeDecimal, bool& decimalSeparator, list<string>& afterDecimal)
{
    beforeDecimal.clear();
    afterDecimal.clear();
    decimalSeparator = false;

    list<string> remainder;
    int index = 0;
    for(auto& i : tokens) {
        if (i == "." && beforeDecimal.empty()) {
            decimalSeparator = true;
            {	// TODO: why not remainder? has only valid tokens...
            	int i_pos = 0;
				for(auto& t : tokens) {
					if(i_pos >= index) break;
					beforeDecimal.push_back(t);
					i_pos++;
				}
            }

            remainder.clear();
        } else if (IsNumberLiteral(i))
        {
            remainder.push_back(i);
        } else if (i.at(0) == '[')
        {
            // ignore
        } else break;

    	index++;
    }


    if (remainder.size() > 0)
    {
        if (!beforeDecimal.empty())
        {
            afterDecimal = remainder;
        } else {
            beforeDecimal = remainder;
        }
    }

    return index;
}
};

bool ExcelNumFormat::ParseSections(const char* sFormat)
{
	bool		bRet	= true;
	Tokenizer	tokenizer(sFormat);
	Release();

	while(1) {
		Section*	pSection	= new Section;
		pSection->section_index = -1;

        if (!ParseSection(pSection, tokenizer)) {
        	delete pSection;
        	return false;
        }

        if (pSection->section_index < 0) {
        	delete pSection;
            break;
        }

        m_Sections.push_back(pSection);
	}

	return true;
}

bool ExcelNumFormat::ParseSection(Section* pSection, Tokenizer& reader)
{
    bool hasDateParts = false;
    bool hasDurationParts = false;
    bool hasGeneralPart = false;
    bool hasTextPart = false;
    bool hasPlaceholders = false;
    Condition		condition;
    string			color;
    string			token;
    list<string>	tokens;
    bool syntaxError	= false;

    while (!(token = ReadToken(reader, syntaxError)).empty())
	{
		if (token == ";")
			break;

		hasPlaceholders |= IsPlaceholder(token);

		if (IsDatePart(token))
		{
			hasDateParts |= true;
			hasDurationParts |= IsDurationPart(token);
			tokens.push_back(token);
		}
		else if (IsGeneral(token))
		{
			hasGeneralPart |= true;
			tokens.push_back(token);
		}
		else if (token == "@")
		{
			hasTextPart |= true;
			tokens.push_back(token);
		}
		else if (token.at(0) == '[')
		{
			// Does not add to tokens. Absolute/elapsed time tokens
			// also start with '[', but handled as date part above
			string expression = token.substr(1, token.length() - 2);
			Condition		parseCondition;
			string			parseColor;
			string			parseCurrencySymbol;
			if (TryParseCondition(expression, parseCondition))
				condition = parseCondition;
			else if (TryParseColor(expression, color)) {
			} else if (TryParseCurrencySymbol(expression, parseCurrencySymbol))
				tokens.push_back("\"" + parseCurrencySymbol + "\"");
		}
		else
		{
			tokens.push_back(token);
		}
	}

    if(syntaxError) return false;
	if(!tokens.size()) return true;

	if (
		(hasDateParts && (hasGeneralPart || hasTextPart)) ||
		(hasGeneralPart && (hasDateParts || hasTextPart)) ||
		(hasTextPart && (hasGeneralPart || hasDateParts)))
	{
		// Cannot mix date, general and/or text parts
		syntaxError = true;
		return false;
	}

	SectionType type;
	FractionSection fraction;
	ExponentialSection exponential;
	DecimalSection number;
	list<string> generalTextDateDuration;

	if (hasDateParts)
	{
		if (hasDurationParts)
		{
			type = SECTION_TYPE_DURATION;
		}
		else
		{
			type = SECTION_TYPE_DATE;
		}

		ParseMilliseconds(tokens, generalTextDateDuration);
	}
	else if (hasGeneralPart)
	{
		type = SECTION_TYPE_GENERAL;
		generalTextDateDuration = tokens;
	}
	else if (hasTextPart || !hasPlaceholders)
	{
		type = SECTION_TYPE_TEXT;
		generalTextDateDuration = tokens;

	}
	else if (fraction.TryParse(tokens))
	{
		type = SECTION_TYPE_FRACTION;
	}
	else if (exponential.TryParse(tokens))
	{
		type = SECTION_TYPE_EXPONENTIAL;

	}
	else if (number.TryParse(tokens))
	{
		type = SECTION_TYPE_NUMBER;
	}
	else
	{
		printf("syntax error:\n");
		// Unable to parse format string
		syntaxError = true;
		return false;
	}

	pSection->type							= type;
	pSection->section_index					= m_Sections.size();
	pSection->color							= color;
	pSection->condition						= condition;
	pSection->Fraction						= fraction;
	pSection->Exponential					= exponential;
	pSection->Number						= number;
	pSection->GeneralTextDateDurationParts	= generalTextDateDuration;

    return true;
}
