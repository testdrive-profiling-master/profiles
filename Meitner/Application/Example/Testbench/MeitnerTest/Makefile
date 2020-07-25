#-------------------------------------------------
# Project
#-------------------------------------------------
#TD_DESCRIPTION	= Meitner processor test
#TD_TYPE		= profile
#TD_ERROR_TOKEN	= "*E: ", "*I: ":0, "*W: ":0, "	Write":0, "	Read":0
SRC_PATH		:= .
#TD_WORK_FOLDER :=
TARGETPATH		:= ../../../../Program
#TD_FILE_NAME	:= %%PROJECT%%Program\%s.exe
TARGETNAME		:= Testbench_MeitnerTest

SRCS			:= \
	$(wildcard $(SRC_PATH)/*.cpp) \
	$(SRC_PATH)/../framework/hw/I2C.cpp \
	$(SRC_PATH)/../framework/hw/MMCM.cpp \
	$(SRC_PATH)/../framework/hw/DUT.cpp \
	$(SRC_PATH)/../framework/hw/CVT.cpp \
	$(SRC_PATH)/../framework/hw/hdmi_controller.cpp \
	$(SRC_PATH)/../framework/common.cpp \
	$(SRC_PATH)/../framework/Testbench.cpp

INC				:= -I../framework

CDEFS			:=

#-------------------------------------------------
# common document config
#-------------------------------------------------
include ../../../include/common.mk
