LOGI("Download repository.")
exec("touch .TestDrive.nosearch")

os.execute("git clone https://github.com/testdrive-profiling-master/MTSP_Assembly.git MTSP_Assembler")

Sleep(2000)	-- to ignore file change autobuild
exec("rm -f .TestDrive.nosearch")
