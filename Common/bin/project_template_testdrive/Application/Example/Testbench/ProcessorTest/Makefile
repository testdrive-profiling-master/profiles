#-------------------------------------------------
# Project
#-------------------------------------------------
#TD_DESCRIPTION	= Processor test
#TD_TYPE		= profile
#TD_ERROR_TOKEN	= "*E: ", "*I: ":0, "*W: ":W, "Write :":0, "Read :":0
SRC_PATH		:= .
#TD_WORK_FOLDER :=
TARGETPATH		:= ../../../../Program
#TD_FILE_NAME	:= %%PROJECT%%Program\%s.exe
TARGETNAME		:= Testbench_ProcessorTest

SRCS			:= \
	$(wildcard $(SRC_PATH)/*.cpp) \
	$(SRC_PATH)/../framework/common.cpp \
	$(SRC_PATH)/../framework/Testbench.cpp \
	$(SRC_PATH)/../framework/hw/DUT.cpp \
	$(SRC_PATH)/../framework/hw/MMCM.cpp

INC				:= -I../framework

CDEFS			:=

#-------------------------------------------------
# common document config
#-------------------------------------------------
include ../../../include/common.mk
