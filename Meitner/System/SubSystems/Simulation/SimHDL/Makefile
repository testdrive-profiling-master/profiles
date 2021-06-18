all:
ifeq (,$(wildcard ../verilator/SimTop__Dpi.h))
	@echo // dummy > ../verilator/SimTop__Dpi.h
endif
	@rm -f SimHDL.o
	@mingw32-make -j $(NUMBER_OF_PROCESSORS) -f Makefile.HDL

clean:
	@mingw32-make -f Makefile.HDL clean

static:
	@mingw32-make -f Makefile.HDL static
