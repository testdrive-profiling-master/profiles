#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= .
TARGETPATH		:= ../../../../Program
TARGETNAME		:= Testbench_Meitner_ScenarioTest

SRCS			:= \
	$(wildcard $(SRC_PATH)/*.cpp) \
	$(SRC_PATH)/../framework/common.cpp \
	$(SRC_PATH)/../framework/hw/I2C.cpp \
	$(SRC_PATH)/../framework/hw/MMCM.cpp \
	$(SRC_PATH)/../framework/hw/CVT.cpp \
	$(SRC_PATH)/../framework/hw/hdmi_controller.cpp \
	$(SRC_PATH)/../framework/hw/DUT.cpp \
	$(SRC_PATH)/../framework/hw/MTSP.cpp \
	$(SRC_PATH)/../framework/Testbench.cpp
	
INC				:= -I../framework

CDEFS			:= -DUSE_TESTDRIVE

#-------------------------------------------------
# common document config
#-------------------------------------------------
include ../../../include/common.mk
