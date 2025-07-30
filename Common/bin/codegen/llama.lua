local Arg = ArgTable("LLaMA C++")

Arg:AddOptionString		("model", "-", nil, nil, "model_path", "requested model path")
Arg:AddRemark(nil, "Refer : https://huggingface.co/models?apps=llama.cpp")
Arg:AddRemark(nil, " ")
Arg:AddRemark(nil, "*** Example ***")
Arg:AddRemark(nil, "> llama .")
if (Arg:DoParse() == false) then
	return
end

local sModel = Arg:GetOptionString("model")

if string.len(sModel) == 1 then
	sModel = "ggml-org/gemma-3-1b-it-GGUF"
end

os.require_library("llama.cpp")

local web = WebGUI()
web:Initialize("client")
web:SetTitle("LLaMA C++")

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

LOGI("LLAMA(" .. sModel .. ") Initializing... please wait...")
exec("llama-server -hf " .. sModel, check_url)
