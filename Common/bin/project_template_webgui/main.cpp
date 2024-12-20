#include "WebGUI.h"

class WebGUI_imp : public WebGUI
{
public:
	WebGUI_imp(void)
	{
		Bind("PressBtn", [&](const ARGS &args, cstring &result, void *pPrivate) {
			CallJScript("$('#cpp_output').text(\"PressBtn(%d, '%s')\");", args[0].asInt(), args[1].asCString());
		});
		Bind("RadioBtn", [&](const ARGS &args, cstring &result, void *pPrivate) {
			CallJScript("$('#cpp_output').text(\"RadioBtn(%s)\");", args[0].asCString());
		});
		Bind("CheckBtn", [&](const ARGS &args, cstring &result, void *pPrivate) {
			CallJScript("$('#cpp_output').text(\"CheckBtn(%s)\");", args[0].asBool() ? "true" : "false");
		});
		Bind("SetText", [&](const ARGS &args, cstring &result, void *pPrivate) {
			CallJScript("$('#cpp_output').text(\"%s\");", args[0].asCString());
		});
		Bind("OnLoad", [&](const ARGS &args, cstring &result, void *pPrivate) {
#ifndef USE_DEBUG // prevent context menu
			CallJScript("document.body.setAttribute('oncontextmenu', 'return false;');");
#endif
		});
	}
	virtual ~WebGUI_imp(void) {}
};

int main(int argc, const char *argv[])
{
	WebGUI_imp gui;
	if (gui.Initialize()) {
		gui.SetRootPath("ui/");
		gui.SetTitle("GUI window");
		gui.SetSize(640, 480, WEBGUI_SIZE_MIN);
		gui.Run();
	}

	return 0;
}
