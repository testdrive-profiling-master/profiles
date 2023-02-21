all:
ifeq (,$(wildcard ../verilator/SimTop__Dpi.h))
	@echo // dummy > ../verilator/SimTop__Dpi.h
endif
	@rm -f SimHDL.o
	@mingw32-make -s -f Makefile.HDL dep
	@mingw32-make -s -f Makefile.HDL

clean:
	@mingw32-make -f Makefile.HDL clean

static:
	@mingw32-make -f Makefile.HDL static
