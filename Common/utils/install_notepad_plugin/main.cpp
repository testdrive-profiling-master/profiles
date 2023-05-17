#include "UtilFramework.h"
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>


using namespace rapidjson;

static string __exec(const char* cmd)
{
	cstring	sCmd(cmd);
	char buffer[128];
	string result = "";
	sCmd	+= " 2>&1";		// redirect catch stderr
	FILE* pipe = popen(sCmd.c_str(), "r");

	if(!pipe) throw runtime_error("popen() failed!");

	try {
		while(fgets(buffer, sizeof(buffer), pipe) != NULL) {
			result += buffer;
		}
	} catch(...) {
		pclose(pipe);
		throw;
	}

	pclose(pipe);
	return result;
}

const Value* GetPlugin(Value& npp_plugins, const char* sName) {
	for (Value::ConstValueIterator p = npp_plugins.Begin(); p != npp_plugins.End(); ++p) {
		cstring	sPluginName	= (*p)["folder-name"].GetString();
		if(sPluginName == sName) {
			return p;
		}
	}
	return nullptr;
}

int main(int argc, const char* argv[]) {
	Document document;

	ArgTable	main_arg_table("Notepad++ plugins installer");

	main_arg_table.AddOption("list", "l", "list", "list up all available plug-ins");
	main_arg_table.AddOptionString("name", "", NULL, NULL, "name", "plug-in name");

	if(!main_arg_table.DoParse(argc, argv))
		return 1;

	if (document.Parse(__exec("curl -s https://raw.githubusercontent.com/notepad-plus-plus/nppPluginList/master/src/pl.x64.json").c_str()).HasParseError()) {
		LOGE("Can't read notepad++ plugin list.");
		return 1;
	}

	cstring sPluginName	= main_arg_table.GetOptionString("name");
	cstring sToolPath;
	sToolPath.GetEnvironment("TESTDRIVE_DIR");

	if(!document.HasMember("npp-plugins")) {
		LOGE("No 'npp-plugins' member.");
		return 1;
	}

	Value& npp_plugins = document["npp-plugins"];

	// list up all available plug-ins
	if (main_arg_table.GetOption("list")) {
		printf("** available plug-ins\n");
		for (Value::ConstValueIterator p = npp_plugins.Begin(); p != npp_plugins.End(); ++p) {
			cstring	sName	= (*p)["folder-name"].GetString();
			printf("\t%-32s %s\n", sName.c_str(), (*p)["version"].GetString());
		}
	} else if(sPluginName.Length() != 0){

		auto p = GetPlugin(npp_plugins, sPluginName);

		if(p == nullptr) {
			LOGE("No npp-plugin member : %s", sPluginName.c_str());
			return 1;
		}

		cstring sCmd;

		LOGI("Installing '%s' (%s)", sPluginName.c_str(), (*p)["version"].GetString());

		sCmd.Format("wget -c -O %s.zip %s", sPluginName.c_str(), (*p)["repository"].GetString());
		__exec(sCmd);
		sCmd.Format("mkdir %sbin/notepad/plugins/%s", sToolPath.c_str(), sPluginName.c_str());
		sCmd.Replace("/", "\\", true);
		__exec(sCmd);
		sCmd.Format("%sbin/unzip.exe -o %s.zip -d %sbin/notepad/plugins/%s", sToolPath.c_str(), sPluginName.c_str(), sToolPath.c_str(), sPluginName.c_str());
		__exec(sCmd);
		sCmd.Format("rm -f %s.zip", sPluginName.c_str());
		system(sCmd);
		LOGI("Done!");
	}

	return 0;
}
