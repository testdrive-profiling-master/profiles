-- gobal call test
print(global_Function("Maria"))
print(global_Function("Todd", 16))

-- namespaced function call test
print(test.named_function(32, 3))

-- class useage
do
	local i_A	= ClassData("A")
	local i_B	= ClassData("B")
	
	i_A:Reset()
	print("get property of i_A : " .. i_A.v)
	
	print("Set A to 3")
	i_A:Set(3)
	print("get function of i_A : " .. i_A.v)
	print("get property of i_A : " .. i_A:Get())
	
	print("Set B to 5")
	i_B:Set(5)
	i_A:Add(i_B)
	print("get function of i_A + i_B : " .. i_A.v)
end

