local sPath = String(os.getenv("TESTDRIVE_PROFILE"))

if sPath:IsEmpty() == false then
	sPath:Replace("/", "\\", true)
	sPath:Append("Common\\web")

	os.execute("cp -rf \"" .. sPath.s .. "\" .")
end
