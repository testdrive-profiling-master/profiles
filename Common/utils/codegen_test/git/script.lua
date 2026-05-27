local git = Git()

if git:Clone("https://github.com/zpqrtbnk/test-repo.git") == false then
	LOGE("git clone is failed.")
else
	LOGI("git clone is succedded.")
end

print("")
LOGI("Pull test...")
if git:Pull("test-repo") == false then
	LOGE("git pull is failed.")
else
	LOGI("git pull is succedded.")
end