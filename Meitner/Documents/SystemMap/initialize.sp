profile.tree "Document"{
	branch("System Map"){
		report	("SystemMap",				"SystemMap.vsd");
		report	("Processor",				"Processor.vsd");
		report	("Display",					"Display.vsd");
	}
	expand();
}

profile.call	"%TESTDRIVE_PROFILE%\\common\\doc\\initialize.sp"
