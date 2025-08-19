#lua	-- lua script declaration
-- Processing Unit TestDrive default verification environment
System.SetProfilePath("root", "..\\")										-- set root profile path
RunProfile("%TESTDRIVE_PROFILE%\\common\\initialize.sp")					-- common initialization


-- PROJECT processor environment
System.SetTitle("PROJECT Verification System")
--System.ShowSplash("Profiles\\media\\splash.png")

-- Create memory
CreateMemory((1024*1024) * 128,	"PROJECT")				-- System memory : 128MB
CreateMemory((1024*1024) * 16,	"PROJECT_Display")		-- Display configuration memory & etc..(register map) : 16MB

-- Common Profiles
RunProfile("%TESTDRIVE_PROFILE%\\common\\SystemManager\\initialize.sp")		-- System management
RunProfile("%TESTDRIVE_PROFILE%\\common\\SystemRemote\\initialize.sp")		-- system remote
RunProfile("%TESTDRIVE_PROFILE%\\common\\CodeAnalysis\\initialize.sp")		-- code analysis
RunProfile("%TESTDRIVE_PROFILE%\\common\\XilinxSynthesis\\initialize.sp")	-- xilinx synthesis
RunProfile("%TESTDRIVE_PROFILE%\\common\\SystemMap\\initialize.sp")			-- system map
RunProfile("%TESTDRIVE_PROFILE%\\common\\SystemChart\\initialize.sp")		-- system chart
RunProfile("%TESTDRIVE_PROFILE%\\common\\ScenarioTest\\initialize.sp")		-- scenario test
RunProfile("%TESTDRIVE_PROFILE%\\common\\VirtualDisplay\\initialize.sp")	-- virtual display
RunProfile("%TESTDRIVE_PROFILE%\\common\\DesignMap\\initialize.sp")			-- design map

-- Project Profiles
RunProfile("Application\\Example\\initialize.sp")							-- Examples
RunProfile("Documents\\initialize.sp")										-- documents
