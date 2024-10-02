#include <UtilFrameWork.h>
#include "webui.hpp"

void OutputText(webui::window::event* e, cstring sMsg) {
	cstring sJscript;
	sMsg.Replace("\"", "\x22", true);
	sJscript.Format("$('#cpp_output').text(\"%s\");", sMsg.c_str());	// jquery output
	e->run_client(sJscript.c_str());	// call jscript
}

void btn_events(webui::window::event* e) {
	cstring sMsg;
	sMsg.Format("btn[%d] = '%s'", e->get_int(0), e->get_string(1).c_str());
	OutputText(e, sMsg);
}

void radio_events(webui::window::event* e) {
	cstring sMsg;
	sMsg.Format("radio = '%s'", e->get_string(0).c_str());
	OutputText(e, sMsg);
}

void checkbox_events(webui::window::event* e) {
	cstring s;
	s.Format("checkbox : %d", e->get_bool(0));
	OutputText(e, s);
}

void inputbox_events(webui::window::event* e) {
	OutputText(e, e->get_string(0).c_str());
}

int main(int argc, const char *argv[])
{
	webui::window win;
    webui::set_default_root_folder("./ui");
    win.set_size(640, 480);
    win.set_minimum_size(640, 480);

    // bind javascript functions
    win.bind("PressBtn", btn_events);
    win.bind("RadioBtn", radio_events);
    win.bind("CheckBtn", checkbox_events);
    win.bind("InputBox", inputbox_events);

    // open window
    if(win.show("ui.html")) {
    	webui::wait();
    }
    webui_clean();
    return 0;
}
