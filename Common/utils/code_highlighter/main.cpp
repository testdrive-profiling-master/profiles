#include "ArgTable.h"
#include "TextFile.h"
#include "cstring.h"
#include "source-highlight/sourcehighlight.h"
#include "source-highlight/langmap.h"

using namespace std;
using namespace srchilite;

int main(int argc, const char* argv[])
{
	ArgTable	main_arg_table("TestDrive code high-lighter. (build date : " __DATE__ ")");
	main_arg_table.AddOptionString("input_lang", "cpp", NULL, "ilang", "input_lang", "input language identifier string");
	main_arg_table.AddRemark(NULL, "default : \"cpp\"");
	main_arg_table.AddOptionString("out_lang", "html", NULL, "olang", "out_lang", "output language identifier string");
	main_arg_table.AddRemark(NULL, "default : \"html\"");
	main_arg_table.AddOption("list", "l", NULL, "list up possible input code format");
	main_arg_table.AddOption("line_number", "n", NULL, "Enable line number");
	main_arg_table.AddOptionString("style", "", "s", "style", "style", "style");
	main_arg_table.AddOptionString("input", "", NULL, NULL, "input_file", "input file");
	main_arg_table.AddRemark(NULL, "default : stdin");
	main_arg_table.AddOptionString("output", "", NULL, NULL, "output_file", "output file");
	main_arg_table.AddRemark(NULL, "default : stdout");

	if(!main_arg_table.DoParse(argc, argv))
		return 1;

	// get base environment path
	cstring	sBasePath	= GetCommonToolPath();
	sBasePath.Append("bin/source-highlight-lang");

	// get output language format
	cstring	sOutLang	= main_arg_table.GetOptionString("out_lang");
	if(!sOutLang.CompareBack(".outlang")) {	// correct file extension
		sOutLang.Append(".outlang");
	}
	if(access(sOutLang.c_str(), F_OK)) {	// can't search on file path
		sOutLang.insert(0, "/");
		sOutLang.insert(0, sBasePath.c_str());
		if(access(sOutLang.c_str(), F_OK)) {	// can't search on file path
			LOGE("Can't find out-lang file '%s'.", main_arg_table.GetOptionString("out_lang"));
			return 1;
		}
	}

	// get input/output file name
	cstring		sInput		= main_arg_table.GetOptionString("input");
	cstring		sOutput		= main_arg_table.GetOptionString("output");
	int			iLineCount	= 0;
	if(sInput == "stdin") sInput.clear();
	if(sOutput == "stdout") sOutput.clear();

	if(sInput.Length()) {	// get line count
		TextFile	f;
		if(!f.Open(sInput)) {	// can't search on file path
			LOGE("Can't open input file. : '%s'", sInput.c_str());
			return 1;
		}
		cstring sLine;
		while(f.GetLine(sLine));
		iLineCount	= f.LineNumber();
	}

	// get input language format
	srchilite::LangMap langMap(sBasePath.c_str(), "lang.map");
	string sInputLang	= langMap.getMappedFileNameFromFileName(main_arg_table.GetOptionString("input_lang"));
	if(sInputLang.empty()) {
		LOGE("Can't find input-lang : '%s'", main_arg_table.GetOptionString("input_lang"));
		return 1;
	}

	if(main_arg_table.GetOption("list")) {
		int iCount = 0;
		printf("*** Available current input code format(%s/lang.map) list\n", sBasePath.c_str());
		for(auto& i : langMap) {
			printf(!(iCount%10) ? "\n\t" : (iCount ? ", " : ""));
			printf("%s", i.first.c_str());
			iCount++;
		}
		printf("\n");
		return 0;
	}

	cstring		sStyle		= main_arg_table.GetOptionString("style");

	{	// do right thing...
		SourceHighlight sourceHighlight(sOutLang.c_str());
		sourceHighlight.setDataDir(sBasePath.c_str());
		sourceHighlight.setTabSpaces(4);
		if(main_arg_table.GetOption("line_number")) {
			sourceHighlight.setGenerateLineNumbers();
			sourceHighlight.setLineNumberPad(' ');
			sourceHighlight.setLineNumberDigits(
					iLineCount < 10 ? 1 :
					iLineCount < 100 ? 2 :
					iLineCount < 1000 ? 3 : 4
					);
		}
		if(sStyle.Length()) sourceHighlight.setStyleFile(sStyle.c_str());
		sourceHighlight.highlight(sInput.c_str(), sOutput.c_str(), sInputLang);
	}
	return 0;
}
