local Arg = ArgTable("LLaMA C++")

Arg:AddOptionString		("model", "", nil, nil, "model_path", "Requested model path")
Arg:AddRemark(nil, "Refer : https://huggingface.co/models?apps=llama.cpp")
Arg:AddOption			("clear", "C", nil, "Clear local models")
Arg:AddOption           ("reset", "R", nil, "Reset default model(google Gemma-3-1B)")

if (Arg:DoParse() == false) then
	return
end

if Arg:GetOption("clear") then
	exec("del /Q %LOCALAPPDATA%\\llama.cpp\\*")
	LOGI("All models are deleted.")
	os.exit(0)
end

-- get model name from system configuration
local model_name = String()
model_name:GetEnvironment("MODEL@LLAMA")
if model_name:Length() == 0 then
	model_name.s = "ggml-org/gemma-3-1b-it-GGUF" -- default model
end

do	-- update from argument, if existed
	local sModel = Arg:GetOptionString("model")
	
	if Arg:GetOption("reset") then
		sModel = "ggml-org/gemma-3-1b-it-GGUF" -- default model
	end

	if string.len(sModel) > 0 then
		model_name.s = sModel
		model_name:SetEnvironment("MODEL@LLAMA")
	end
end

-- install library if required
os.require_library("llama.cpp")

local tool_path = String()
tool_path:GetEnvironment("TESTDRIVE_PROFILE")
tool_path:Append("common/bin/codegen/")

local web = WebGUI()
web:Initialize("client")
web:SetTitle("LLaMA C++ (" .. model_name.s .. ")")
web:SetIcon(tool_path.s .. "llama.ico")

function main_loop()
	web:SetSize(1024, 768)
end

local llama_url = String()
local llama_ready = false
function check_url(s)
	if llama_ready == false then
		llama_url:Append(s)
		if llama_url:CutFront("server is listening on") then
			llama_ready = true
			llama_url:CutBack("-")
			llama_url:Trim(" ")
			web:Run(llama_url.s, main_loop)
			exec("taskkill /F /im llama-server.exe")
			LOGI("Done!")
			os.exit(1)
		end
	end
end

LOGI("LLAMA(" .. model_name.s .. ") Initializing... please wait...")
exec("llama-server -hf " .. model_name.s, check_url)
