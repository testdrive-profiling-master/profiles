// this profile will be not shown.
profile.tree "System" {
	//report ("Processor",					"%CURRENT%\\main.sp");
	profile ("Project folder"	, "%PROJECT%");
}

// run profile
profile.call	"main.sp"
