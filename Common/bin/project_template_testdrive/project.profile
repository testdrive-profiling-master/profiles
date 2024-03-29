// TestDrive verification environment
profile.path			"..\\"							// set common profile path
profile.set.initialize	""								// remove initialization profile
profile.set.cleanup		"Profiles\\Common\\cleanup.sp"	// set cleanup profile

system.clear

// PROJECT processor environment
system.title		"PROJECT Verification System"
system.splash		"Profiles\\media\\splash.png"

// Create memory
memory.create		0x8000000,	"PROJECT"				// System memory : 128MB
memory.create		0x1000000,	"PROJECT_Display"		// Display configuration memory & etc..(register map) : 16MB

// Common Profiles
profile.call		"%TESTDRIVE_PROFILE%\\common\\SystemManager\\initialize.sp"		// System management
profile.call		"%TESTDRIVE_PROFILE%\\common\\SystemRemote\\initialize.sp"		// system remote
profile.call		"%TESTDRIVE_PROFILE%\\common\\CodeAnalysis\\initialize.sp"		// code analysis
profile.call		"%TESTDRIVE_PROFILE%\\common\\XilinxSynthesis\\initialize.sp"	// xilinx synthesis
profile.call		"%TESTDRIVE_PROFILE%\\common\\SystemMap\\initialize.sp"			// system map
profile.call		"%TESTDRIVE_PROFILE%\\common\\SystemChart\\initialize.sp"		// system chart
profile.call		"%TESTDRIVE_PROFILE%\\common\\ScenarioTest\\initialize.sp"		// scenario test
profile.call		"%TESTDRIVE_PROFILE%\\common\\VirtualDisplay\\initialize.sp"	// virtual display
profile.call		"%TESTDRIVE_PROFILE%\\common\\DesignMap\\initialize.sp"			// design map

// Project Profiles
profile.call		"Profiles\\common\\initialize.sp"						// build archive
profile.call		"Application\\Example\\initialize.sp"					// Examples
profile.call		"Documents\\initialize.sp"								// documents

profile.call		"%TESTDRIVE_PROFILE%\\common\\Community\\initialize.sp"			// community