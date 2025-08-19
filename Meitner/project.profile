#lua	-- lua script declaration
-- Processing Unit TestDrive default verification environment
System.SetProfilePath("root", "..\\")										-- set root profile path
RunProfile("%TESTDRIVE_PROFILE%\\common\\initialize.sp")					-- common initialization


-- Meitner processor environment
System.SetTitle("Meitner Processor Verification System")
--System.SetSubTitle("TestDrive Profiling Master")
System.ShowSplash("Profiles\\media\\splash.png")

-- Create memory
CreateMemory((1024*1024) * 128,	"Meitner")									-- System memory : 128MB
CreateMemory((1024*1024) * 16,	"Meitner_Display")							-- Display configuration memory & etc..(register map) : 16MB

-- Common Profiles
RunProfile("%TESTDRIVE_PROFILE%\\common\\SystemManager\\initialize.sp")		-- System management
--RunProfile("%TESTDRIVE_PROFILE%\\common\\SystemRemote\\initialize.sp")		-- system remote
RunProfile("%TESTDRIVE_PROFILE%\\common\\CodeAnalysis\\initialize.sp")		-- code analysis
RunProfile("%TESTDRIVE_PROFILE%\\common\\XilinxSynthesis\\initialize.sp")	-- xilinx synthesis
RunProfile("%TESTDRIVE_PROFILE%\\common\\SystemMap\\initialize.sp")			-- system map
RunProfile("%TESTDRIVE_PROFILE%\\common\\SystemChart\\initialize.sp")		-- system chart
RunProfile("%TESTDRIVE_PROFILE%\\common\\ScenarioTest\\initialize.sp")		-- scenario test
RunProfile("%TESTDRIVE_PROFILE%\\common\\VirtualDisplay\\initialize.sp")		-- virtual display
--RunProfile("%TESTDRIVE_PROFILE%\\common\\SerialConsole\\initialize.sp")	-- serial console
RunProfile("%TESTDRIVE_PROFILE%\\common\\DesignMap\\initialize.sp")			-- design map
RunProfile("%TESTDRIVE_PROFILE%\\common\\SerialConsole\\initialize.sp")		-- serial console

-- Project Profiles
RunProfile("Profiles\\ALUTest\\initialize.sp")								-- ALU test
RunProfile("Profiles\\RegisterMap\\initialize.sp")							-- register map
RunProfile("Profiles\\FPGA_StarterKit\\initialize.sp")						-- FPGA Starter kit
RunProfile("Application\\Example\\initialize.sp")							-- Examples
RunProfile("Documents\\initialize.sp")										-- documents
RunProfile("Driver\\initialize.sp")											-- driver
