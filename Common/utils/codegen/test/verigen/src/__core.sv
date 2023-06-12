:---------------------------------------------------------------------
:test_core

:---------------------------------------------------------------------
:core_if
$set_param("CORE_ID", "0")
$add_interface(core_i.inst, "if_inst", "m")
$add_interface(core_busy, nil, "m")

assign	core_busy	= 1'b0;


:---------------------------------------------------------------------
:core_ex
$add_interface(core_i.inst, "if_inst", "s")
$add_interface(core_i.inst, "ex_inst", "m")


:---------------------------------------------------------------------
:core_wb
$add_interface(core_i.inst, "ex_inst", "s")
