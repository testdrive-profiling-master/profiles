# In-line block

In verilog code, you can use the Lua code in block(@<code>@{...}@</code>) expression.
The @<code>this@</code> directive indicates the current module.
 
Ex.)
```verilog
module test_module

@{
	for i=1,4 do
		this:add_module("SRAM_Single")
	end
}
```

;;;
