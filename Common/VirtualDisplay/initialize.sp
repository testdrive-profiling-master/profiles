// add to profile view
/*
if.locale "Korean"
	profile.tree "�ý���" {
		screen("���÷���",		"%CURRENT%\\main.sp");
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
