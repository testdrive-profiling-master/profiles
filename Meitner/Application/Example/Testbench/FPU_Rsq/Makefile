#-------------------------------------------------
# Project
#-------------------------------------------------
#TD_DESCRIPTION	= FPU test (Reciprocal Square Root)
#TD_TYPE		= profile
#TD_ERROR_TOKEN	= "*E: ", "*I: ":0, "*W: ":W
SRC_PATH		:= .
#TD_WORK_FOLDER :=
TARGETPATH		:= ../../../../Program
#TD_FILE_NAME	:= %%PROJECT%%Program\%s.exe
TARGETNAME		:= Testbench_FPU_RSQ

.PHONY: build_hw

build_hw:
	@mingw32-make -s -f Makefile.HDL BUILD_TARGET=$(TARGETPATH)/$(TARGETNAME).exe

SRCS			:= \
	$(wildcard $(SRC_PATH)/*.cpp) \
	$(wildcard $(SRC_PATH)/../framework/sim/*.cpp) \
	$(wildcard $(SRC_PATH)/.verilated_src/*.cpp) 
	
#-------------------------------------------------
# common document config
#-------------------------------------------------
LIBPATH			:= ../../../lib
INC				:= -I../../../include -I../framework/sim -I.verilated_src -I$(TESTDRIVE_DIR)include/verilator -I$(TESTDRIVE_DIR)bin/msys64/ucrt64/share/verilator/include -I$(TESTDRIVE_DIR)bin/msys64/ucrt64/share/verilator/include/vltstd
LIBDIR			:= -L$(LIBPATH) -lgomp
CDEFS			:= -DUSE_TESTDRIVE -DUSE_VERILATOR -DVL_THREADED -fopenmp

BUILD_TARGET	= $(TARGET_EXE)

include $(TESTDRIVE_PROFILE)/common/include/config.mk

