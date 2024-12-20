#include <UtilFrameWork.h>
#include ".PROJECT_LOWER_version.inl"
#include "lua.hpp"
#include "LuaBridge/LuaBridge.h"

using namespace luabridge;

// Lua base class example
class BaseClass {
public:
	BaseClass(const char* sName) {
		m_sName		= sName;
		m_iValue	= 0;
		printf("Create %s class\n", m_sName.c_str());
	}
	virtual ~BaseClass(void) {
		printf("Destroy %s class\n", m_sName.c_str());
	}

	void SetValue(int iV) {
		m_iValue	= iV;
	}

	int GetValue(void) {
		return m_iValue;
	}

	const char* Name(void) {
		return m_sName;
	}

	cstring		m_sName;
	int			m_iValue;
};

// Lua derived class example #1
class DerivedClass : public BaseClass {
public:
	DerivedClass(const char* sName) : BaseClass(sName) {}
	virtual ~DerivedClass(void) {}

	void Add(BaseClass* pSrc) {
		m_iValue	+= pSrc->m_iValue;
	}

	void Reset(void) {
		m_iValue	= 0;
	}
};

// named function sample
string named_function(int a, int b) {
	cstring s;
	s.Format("Result : %d", a + b);
	return s.c_string();
}

// global function sample with LuaRef. (more flexible)
string global_Function(LuaRef v1, LuaRef v2) {
	cstring s;
	if(v1.isNumber()) {
		s.Format("It's %d", (int)v1);
	} else if(v1.isString()) {
		s.Format("It's %s", (const char*)v1);
	}

	if(v2.isNumber()) {
		s.AppendFormat(" and %d", (int)v2);
	} else if(v2.isString()) {
		s.AppendFormat(" and %s", (const char*)v2);
	}

	return s.c_string();
}

int main(int argc, const char *argv[])
{
	int		iRet	= 0;
	cstring sScriptFileName;

	// managing command line
	{
		// setup argument table
		cstring sTitle;
		sTitle.Format("PROJECT Program. v%d.%d (build #%d date : " PROJECT_UPPER_VERSION_DATE ")",
			PROJECT_UPPER_VERSION_MAJOR,
			PROJECT_UPPER_VERSION_MINOR,
			PROJECT_UPPER_VERSION_BUILD
		);
		ArgTable arg_table(sTitle);

		arg_table.AddOptionFile("script_file", NULL, NULL, NULL, "script_file", "source Lua script file name.");

		if (!arg_table.DoParse(argc, argv))
			return 0;

		// do argument parsing
		sScriptFileName	 = arg_table.GetOptionFile("script_file");
	}

	// Do test Lua
	lua_State*		pLua = luaL_newstate();

	if(pLua) {
		// register default system functions
		luaL_openlibs(pLua);

		// register extra function/class
		getGlobalNamespace(pLua)
			// base class
			.beginClass<BaseClass>("BaseClass")
			.addFunction("Set", &BaseClass::SetValue)
			.addFunction("Get", &BaseClass::GetValue)
			.addFunction("Name", &BaseClass::Name)
			.addProperty("v", &BaseClass::m_iValue)
			.endClass()
			// derived class
			.deriveClass<DerivedClass, BaseClass>("ClassData")
			.addConstructor<void (*)(const char*)>()
			.addFunction("Add", &DerivedClass::Add)
			.addFunction("Reset", &DerivedClass::Reset)
			.endClass()
			 // name space functions
			.beginNamespace("test")
			.addFunction("named_function", &named_function)
			.endNamespace()
			// global functions
			.addFunction("global_Function", &global_Function);

		// run script
		if(luaL_dofile(pLua, sScriptFileName)) {
			// got error on script
			const char *sError = luaL_checkstring(pLua, -1);
			LOGE("Error on running script : %s", sError);
			lua_pop(pLua, 1); // pop out error message
			iRet	= 1;
		}

		// release
		lua_close(pLua);
	}

	return iRet;
}
