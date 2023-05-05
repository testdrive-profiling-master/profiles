# Classes and Methods

명령어에는 다음 4개의 

* module
* module_instance
* interface
* interface_instance
* clock

;;;

## module

### module:new(name)

### module:set_title(title)

### module:set_author(name)

### module:set_inception(filename)

### module:get_inception()

### module:get_interface(name)

### module:get_port(name)

### module:add_interface(i, name)

### module:set_param(name, value, is_local)

### module:get_param(name, is_local)

### module:add_module(name)

### module:get_module(m, name)

### module_i.is_valid(inst)

### module_i:set_param(name, val)

### module_i:get_param(name)

### module_i:set_port(name, val)

### module_i:get_port(name)

;;;

## interface

### interface.findport(name)

### interface.is_valid(inst)

### interface:new(name)

### interface:new_signal(name)

### interface:set_prefix(prefix)

### interface:set_bared(bared)

### interface:set_clock(clk)

### interface:get_clock()

### interface:set_param(name, default_value)

### interface:get_param(name)

### interface:set_signal(name, bitwidth)

### interface:set_modport(name, modport)

### interface:add_modport(name, modport)

### interface:get_modport(name)

### interfae:full_name()

### interface_i:set_desc(desc)

### interface_i:set_prefix()

### interface_i:get_prefix()

### interface_i:set_port(modport_name)



;;;

## clock

### clock.find(name)

### clock.is_valid(inst)

### clock:new(name, desc, base)

### clock:set_reset(name)

### clock:set_speed(mhz)
