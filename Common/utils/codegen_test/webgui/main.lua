local i = 0


local web = WebGUI()
web:Initialize()
web:SetRootPath("ui/")

function main_loop()
    i = i + 1
    print("Hello " .. i)
    return (i ~= 10)
end

function PressBtn(iVal, sVal)
    print("PressBtn (" .. tostring(iVal) .. ", \"" .. tostring(sVal) .. "\")")
    web:CallJScript("$('#cpp_output').text(\"PressBtn (" .. tostring(iVal) .. ", \"" .. tostring(sVal) .. ")\")")
end

function RadioBtn(bVal)
    print("RadioBtn (" .. tostring(bVal) .. ")")
    web:CallJScript("$('#cpp_output').text(\"RadioBtn (" .. tostring(bVal) .. ")\")")
end

function CheckBtn(bVal)
    print("CheckBtn (" .. tostring(bVal) .. ")")
    web:CallJScript("$('#cpp_output').text(\"CheckBtn (" .. tostring(bVal) .. ")\")")
end

function SetText(sVal)
    print("SetText (\"" .. sVal .. "\")")
    web:CallJScript("$('#cpp_output').text(\"" .. sVal .. "\")")
end

web:Bind("PressBtn")
web:Bind("RadioBtn")
web:Bind("CheckBtn")
web:Bind("SetText")
--web:Run("http://www.naver.com", fft)
web:Run(nil, main_loop)

print("Done!")
