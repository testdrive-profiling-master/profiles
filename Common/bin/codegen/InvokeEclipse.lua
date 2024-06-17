RunScript("codegen_utils")

local	sTestDrivePath	= String(nil)
local	sProfilePath	= String(nil)

sTestDrivePath:GetEnvironment("TESTDRIVE_DIR")
sProfilePath:GetEnvironment("TESTDRIVE_PROFILE")

-- Argument parsing
local	Arg				= ArgTable("Invoke Eclipse for TestDrive Profiling Master.")

Arg:AddOptionFile	("path", nil, nil, nil, "path", "Root project path")

if (Arg:DoParse() == false) then
	return
end

-- check eclipse installation
if lfs.IsExist(sTestDrivePath.s .. "bin/eclipse") == false then
	LOGE("No Eclipse CDT is installed on this system. Need to run TestDrive Profiling Master first.")
	os.exit(1)
end

-- get project path
local	sProjectPath	= String(lfs.FullPath(Arg:GetOptionFile("path", 0)))

if sProjectPath:IsEmpty() then
	LOGE("Invalid path : " .. Arg:GetOptionFile("path", 0))
	os.exit(1)
end

-- find sub folder ".workspace"
local	sWorkPath	= String(sProjectPath.s)

while sWorkPath:find("\\", 0) > 0 do
	sWorkPath:CutBack("\\", false)
	
	if lfs.IsExist(sWorkPath.s .. "\\.workspace") then
		sWorkPath:Append("\\.workspace")
		break
	end
end

-- can't find .workspace then make it.
if sWorkPath:CompareBack("\\.workspace") == false then
	sWorkPath:Append("\\.workspace")

	if lfs.mkdir(sWorkPath.s) ~= true then
		LOGE("Can't create '.workspace' path : " .. sWorkPath.s)
		os.exit(1)
	end
end

-- create sub workspace path
do
	local	sExtraPath	= String(sProjectPath.s)
	sExtraPath:erase(0, sWorkPath:Length() - 11)
	sWorkPath:Append(sExtraPath.s)
end

-- first run. : make new workspace
if lfs.IsExist(sWorkPath.s) == false then
	-- build first project repo. folder
	LOGI("Preparing workspace for this at first time.")
	print("Please wait...")
	os.execute("%TESTDRIVE_DIR%bin/eclipse/eclipse.exe -nosplash -data \"" .. sWorkPath.s .. "\" -application org.eclipse.cdt.managedbuilder.core.headlessbuild -importAll \"" .. sProjectPath.s .. "\"")	
	
	-- apply default font configuration
	do
		local f = TextFile()
		f:Create(sWorkPath.s .. "/.metadata/.plugins/org.eclipse.core.runtime/.settings/org.eclipse.ui.workbench.prefs")
		f:Put(	"//org.eclipse.ui.commands/state/org.eclipse.ui.navigator.resources.nested.changeProjectPresentation/org.eclipse.ui.commands.radioState=false\
eclipse.preferences.version=1\
org.eclipse.ui.workbench.ACTIVE_NOFOCUS_TAB_BG_END=255,255,255\
org.eclipse.ui.workbench.ACTIVE_NOFOCUS_TAB_BG_START=255,255,255\
org.eclipse.ui.workbench.ACTIVE_NOFOCUS_TAB_TEXT_COLOR=16,16,16\
org.eclipse.ui.workbench.ACTIVE_TAB_BG_END=255,255,255\
org.eclipse.ui.workbench.ACTIVE_TAB_BG_START=255,255,255\
org.eclipse.ui.workbench.INACTIVE_TAB_BG_START=242,242,242\
org.eclipse.cdt.debug.ui.ModulesDetailPaneFont=1|Cascadia Mono|9.75|0|WINDOWS|1|-13|0|0|0|400|0|0|0|0|3|2|1|49|Cascadia Mono;\
org.eclipse.cdt.internal.ui.compare.AsmMergeViewer=1|Cascadia Mono|9.75|0|WINDOWS|1|-13|0|0|0|400|0|0|0|0|3|2|1|49|Cascadia Mono;\
org.eclipse.cdt.internal.ui.compare.CMergeViewer=1|Cascadia Mono|9.75|0|WINDOWS|1|-13|0|0|0|400|0|0|0|0|3|2|1|49|Cascadia Mono;\
org.eclipse.cdt.make.internal.ui.compare.MakefileMergeViewer=1|Cascadia Mono|9.75|0|WINDOWS|1|-13|0|0|0|400|0|0|0|0|3|2|1|49|Cascadia Mono;\
org.eclipse.cdt.ui.buildconsole.ConsoleFont=1|Cascadia Mono|9.75|0|WINDOWS|1|-13|0|0|0|400|0|0|0|0|3|2|1|49|Cascadia Mono;\
org.eclipse.cdt.ui.editors.textfont=1|Cascadia Mono|9.75|0|WINDOWS|1|-13|0|0|0|400|0|0|0|0|3|2|1|49|Cascadia Mono;\
org.eclipse.compare.contentmergeviewer.TextMergeViewer=1|Cascadia Mono|9.75|0|WINDOWS|1|-13|0|0|0|400|0|0|0|0|3|2|1|49|Cascadia Mono;\
org.eclipse.debug.ui.DetailPaneFont=1|Cascadia Mono|9.75|0|WINDOWS|1|-13|0|0|0|400|0|0|0|0|3|2|1|49|Cascadia Mono;\
org.eclipse.debug.ui.MemoryViewTableFont=1|Cascadia Mono|9.75|0|WINDOWS|1|-13|0|0|0|400|0|0|0|0|3|2|1|49|Cascadia Mono;\
org.eclipse.debug.ui.consoleFont=1|Cascadia Mono|9.75|0|WINDOWS|1|-13|0|0|0|400|0|0|0|0|3|2|1|49|Cascadia Mono;\
org.eclipse.egit.ui.CommitMessageEditorFont=1|Cascadia Mono|9.75|0|WINDOWS|1|-13|0|0|0|400|0|0|0|0|3|2|1|49|Cascadia Mono;\
org.eclipse.egit.ui.CommitMessageFont=1|Cascadia Mono|9.75|0|WINDOWS|1|-13|0|0|0|400|0|0|0|0|3|2|1|49|Cascadia Mono;\
org.eclipse.egit.ui.DiffHeadlineFont=1|Cascadia Mono|9.75|0|WINDOWS|1|-13|0|0|0|400|0|0|0|0|3|2|1|49|Cascadia Mono;\
org.eclipse.jface.textfont=1|Cascadia Mono|9.75|0|WINDOWS|1|-13|0|0|0|400|0|0|0|0|3|2|1|49|Cascadia Mono;\
org.eclipse.mylyn.wikitext.ui.presentation.textFont=1|Cascadia Mono|9.75|0|WINDOWS|1|-13|0|0|0|400|0|0|0|0|3|2|1|49|Cascadia Mono;\
org.eclipse.tracecompass.tmf.ui.font.eventraw=1|Cascadia Mono|9.75|0|WINDOWS|1|-13|0|0|0|400|0|0|0|0|3|2|1|49|Cascadia Mono;\
org.eclipse.wst.sse.ui.textfont=1|Cascadia Mono|9.75|0|WINDOWS|1|-13|0|0|0|400|0|0|0|0|3|2|1|49|Cascadia Mono;\
org.sveditor.ui.editor.svTextFont=1|Cascadia Mono|9.75|0|WINDOWS|1|-13|0|0|0|400|0|0|0|0|3|2|1|49|Cascadia Mono;\
terminal.views.view.font.definition=1|Cascadia Mono|9.75|0|WINDOWS|1|-13|0|0|0|400|0|0|0|0|3|2|1|49|Cascadia Mono;"
		);
		f:Close()
	end
end

-- run eclipse
os.execute("start %TESTDRIVE_DIR%bin/eclipse/eclipse.exe -data \"" .. sWorkPath.s .. "\"")