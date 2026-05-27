# Git class

Perform a git clone or pull operation.

* ex.) clone
```lua
local git    = Git()
git.User     = "user_name"
git.Password = "12345678"

if git:Clone("https://testdrive-profiling-master/gitsome.git") == false then
	LOGE("Git clone is failed.")
else
	LOGI("Git clone is succedded.")
end
```
 
* ex.) pull
```lua
local git    = Git()
git.User     = "user_name"
git.Password = "12345678"

if git:Pull("gitsome") == false then
	LOGE("Git pull is failed.")
else
	LOGI("Git pull is succedded.")
end
```
 

## Git.User
Specify the git user ID. Do not use if @<code>anonymous@</code>.
```lua
git.user
```

## Git.Password

## Git.Branch

## Git::Clone

## Git::Pull
