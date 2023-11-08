//================================================================================
// Copyright (c) 2013 ~ 2023. HyungKi Jeong(clonextop@gmail.com)
// Freely available under the terms of the 3-Clause BSD License
// (https://opensource.org/licenses/BSD-3-Clause)
//
// Redistribution and use in source and binary forms,
// with or without modification, are permitted provided
// that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
// BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
// OF SUCH DAMAGE.
//
// Title : Common profiles
// Rev.  : 11/8/2023 Wed (clonextop@gmail.com)
//================================================================================
//#---------------------------------------------------------------------
module test_wrapper
	wire	$RANGE(config.core_size)	core_busy_all;

	${	-- It's Lua codes
	   for i = 1, config.core_size, 1 do
   core.inst[i]:set_param("CORE_ID", i)
   core.inst[i]:set_port("core_busy", "core_busy_all[" .. (i-1) .. "]")
	   end

	   __m:get_module("slave_ctrl"):set_port("core_busy", "|core_busy_all")
	 }

	//#---------------------------------------------------------------------
	module slave_ctrl
		$set_param("BASE_ADDR", "32'h10000000")
		$add_interface(bus.apb, "s_apb", "m")
		$add_interface(bus.apb, "s_apb_0", "m")

		//#---------------------------------------------------------------------
		module mem_ctrl
			$add_interface(bus.maxi4, "maxi", "m")

			//#---------------------------------------------------------------------
			module reg_ctrl
				$add_interface(core_busy, nil, "s")
