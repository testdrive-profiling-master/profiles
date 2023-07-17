#-------------------------------------------------
# Project
#-------------------------------------------------
#TD_DESCRIPTION	= FPU test (Reciprocal)
#TD_TYPE		= profile
#TD_ERROR_TOKEN	= "*E: ", "*I: ":0, "*W: ":W
SRC_PATH		:= .
#TD_WORK_FOLDER :=
TARGETPATH		:= ../../../../Program
#TD_FILE_NAME	:= %%PROJECT%%Program\%s.exe
TARGETNAME		:= Testbench_FPU_Reciprocal

ifdef SIM_BUILD_JOBS
MAKEFLAGS += --jobs=$(SIM_BUILD_JOBS)
endif

all:
	@mingw32-make -s -f Makefile.HDL BUILD_TARGET=$(TARGETPATH)/$(TARGETNAME).exe
	@mingw32-make -s -f Makefile.SIM TARGETNAME=$(TARGETNAME)

clean:
	@mingw32-make -s -f Makefile.HDL clean
	@mingw32-make -s -f Makefile.SIM clean
