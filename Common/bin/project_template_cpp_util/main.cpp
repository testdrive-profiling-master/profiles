#include ".PROJECT_LOWER_version.inl"
#include <UtilFrameWork.h>

int main(int argc, const char *argv[])
{
	// setup argument table
	cstring sTitle;
	sTitle.Format("PROJECT Program. v%d.%d (build #%d date : " __DATE__ ")",
		PROJECT_UPPER_VERSION_MAJOR,
		PROJECT_UPPER_VERSION_MINOR,
		PROJECT_UPPER_VERSION_BUILD
	);
	ArgTable arg_table(sTitle);

	arg_table.AddOption("special", "s", NULL, "Special option.");
	arg_table.AddOptionFile("out_file", "default.txt", "o", NULL, "output_file", "Output file name.");
	arg_table.AddOptionFile("in_file", NULL, NULL, NULL, "input_file", "Input file name.");

	if (!arg_table.DoParse(argc, argv))
		return 0;

	// do argument parsing
	cstring sInFileName	 = arg_table.GetOptionFile("in_file");
	cstring sOutFileName = arg_table.GetOptionFile("out_file");
	LOGI("input file  : %s", sInFileName.c_str());
	LOGI("output file : %s", sOutFileName.c_str());

	if (arg_table.GetOption("special")) {
		LOGI("It's special!");
	}
	
	//TODO...

	return 0;
}
