// initial run...
profile.tree "System" {
	report ("Serial console",				"main.lua");
}

system.document.add "Serial console" {
	SetProgram("SerialConsole.dll");
}
