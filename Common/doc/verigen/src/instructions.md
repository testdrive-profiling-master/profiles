# Classes and Methods

아래와 같이 총 3개의 class 가 존재합니다.

* clock
** 클럭을 생성하고, interface 에 할당할 수 있습니다. 해당 interface 가 사용될 때, 자동으로 할당된 클럭과 클럭에 매칭되는 reset 이 port 로 선언됩니다. 만약 reset 이 선언되어 있지 않는다면, 자동으로 기본 리셋 nRST 신호가 생성됩니다.
* interface
** module 에서 사용할 interface 를 생성합니다. interface 는 다른 interface 로 부터 상속을 받아 생성될 수 있습니다. 모듈에 add_interface 로 부탁된 instance 는 interface_i:* 함수만 호출 가능합니다.
* module
** 모듈을 생성하고, module:add_module 함수를 통해 다른 하위 모듈을 포함하거나, module:add_interface 를 호출하여 interface instance 를 생성할 수 있습니다.

;;;

## clock
interface 에 할당할 클럭을 생성하거나 관리합니다. \
할당된 클럭은 해당 interface 가 사용될 때 자동으로 module의 port에 따라 선언되게 됩니다.
또한 contraint 에 해당 클럭의 speed 가 정의되며, 이종 클럭간의 register 들과 클럭에 설정된 reset 에 대해서도 false_path 를 자동으로 지정하게 됩니다.

@<tbl:media/instruction_clock.xlsx;summary;clock 객체 요약>

;;;

### clock:new
@<tbl:media/instruction_clock.xlsx;new>
 
ex) 생성 예시
```lua
new_clock	= clock:new("CLK")			-- 기본 클럭으로 부터 생성
new_clock:set_reset("GRSTn")

new_clock2	= new_clock:new("ACLK")		-- new_clock을 복제한 클럭. reset, speed 를 상속 받는다.
```
;;;

### clock:set_reset
@<tbl:media/instruction_clock.xlsx;set_reset>
  
ex) 리셋 지정 예시
```lua
aclock	= clock:new("ACLK")
aclock:set_reset("ARSTn")				-- 리셋 ARSTn 지정
```
;;;

### clock:set_speed
@<tbl:media/instruction_clock.xlsx;set_speed>
  
ex) 동작 속도 지정 예시
```lua
aclock	= clock:new("ACLK")
aclock:set_speed(1000)					-- ACK에 1GHz 설정
```
;;;

### clock.find
@<tbl:media/instruction_clock.xlsx;find>
  
ex) 클럭 찾기 예시
```lua
aclock	= clock.find("ACLK")			-- ACLK 찾기

if aclock ~= nil then
	LOGI("ACLK is found.")				-- 찾았음.
end
```
;;;

### clock.is_valid
@<tbl:media/instruction_clock.xlsx;is_valid>
 
ex) 클럭 객체 확인 예시
```lua
aclock	= clock:new("ACLK")

if clock.is_valid(aclock) then
	LOGI("aclock is clock object.")		-- 클럭 객체가 맞음.
end
```
;;;

## interface

### interface:new

### interface.findport

### interface.is_valid

### interface:new_signal

### interface:set_prefix

### interface:set_bared

### interface:set_clock

### interface:get_clock

### interface:set_param

### interface:get_param

### interface:set_signal

### interface:set_modport

### interface:add_modport

### interface:get_modport

### interfae:full_name()

### interface_i:set_desc

### interface_i:set_prefix

### interface_i:get_prefix

### interface_i:set_port


;;;

## module

### module:new

### module:make_code

### module:set_title

### module:set_author

### module:set_inception

### module:get_inception

### module:get_interface

### module:get_port

### module:add_interface

### module:set_param

### module:get_param

### module:add_module

### module:get_module

### module_i.is_valid

### module_i:set_param

### module_i:get_param

### module_i:set_port

### module_i:get_port

