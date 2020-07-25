// add to profile view
/*
if.locale "Korean"
	profile.tree "시스템" {
		screen("디스플레이",		"%CURRENT%\\main.sp");
		expand();
	}
if.else
*/
profile.tree "System" {
	screen("Display",		"%CURRENT%\\main.sp");
	expand();
}
//if.end

profile.call	"main.sp"		// run default
