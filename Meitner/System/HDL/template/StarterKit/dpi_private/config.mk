SRCS	:= $(SRCS) \
	$(wildcard ../../../HDL/template/StarterKit/dpi_private/*.cpp)

CDEFS	:= $(CDEFS) -DSIMULATION_TOP_EX
