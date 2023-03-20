ifdef SIM_BUILD_JOBS
MAKEFLAGS += --jobs=$(SIM_BUILD_JOBS)
endif

all:
ifeq (,$(wildcard ../verilator/SimTop__Dpi.h))
	@echo // dummy > ../verilator/SimTop__Dpi.h
endif
	@rm -f $(TESTDRIVE_PROFILE)Common/System/SystemSim/SimHDL/SimHDL.o
	@mingw32-make -s -f $(TESTDRIVE_PROFILE)Common/System/SystemSim/SimHDL/Makefile.HDL dep
	@mingw32-make -s -f $(TESTDRIVE_PROFILE)Common/System/SystemSim/SimHDL/Makefile.HDL

clean:
	@mingw32-make -f $(TESTDRIVE_PROFILE)Common/System/SystemSim/SimHDL/Makefile.HDL clean

static:
	@mingw32-make -f $(TESTDRIVE_PROFILE)Common/System/SystemSim/SimHDL/Makefile.HDL static
