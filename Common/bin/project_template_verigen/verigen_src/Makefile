#-------------------------------------------------
# Build sequence
#-------------------------------------------------
BUILD_TARGET	:= ../rtl/PROJECT_wrapper.sv
SRCS			:= $(wildcard ./*) .TestDrive.verilator

#-------------------------------------------------
# Build commands
#-------------------------------------------------

all: $(BUILD_TARGET)
	@echo *I: Verilog generation is done!

clean:
	@$(RM) -f ../rtl/*.sv
	@$(RM) -f ../rtl/*.svg
	@$(RM) -f ../rtl/*.vh
	@$(RM) -f ../rtl/*.f
	@$(RM) -f ../rtl/*.xdc
	@$(RM) -f ../rtl/*.config
	@echo Cleanup is done.

#-------------------------------------------------
# generic rules
#-------------------------------------------------
$(BUILD_TARGET):$(SRCS)
	@echo
	@echo '*** Build verigen ***'
	@verigen config.xlsx
