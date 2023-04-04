#-------------------------------------------------
# Project
#-------------------------------------------------
#TD_DESCRIPTION	= HDMI test
#TD_TYPE		= screen
#TD_ERROR_TOKEN	= "*E: ", "*I: ":0, "*W: ":W
SRC_PATH		:= .
#TD_WORK_FOLDER :=
TARGETPATH		:= ../../../../Program
#TD_FILE_NAME	:= %%PROJECT%%Program\%s.exe
TARGETNAME		:= Testbench_HDMITest

SRCS			:= \
	$(wildcard $(SRC_PATH)/*.cpp) \
	$(SRC_PATH)/../framework/hw/I2C.cpp \
	$(SRC_PATH)/../framework/hw/MMCM.cpp \
	$(SRC_PATH)/../framework/hw/CVT.cpp \
	$(SRC_PATH)/../framework/hw/hdmi_controller.cpp \
	$(SRC_PATH)/../framework/common.cpp \
	$(SRC_PATH)/../framework/Display.cpp \
	$(SRC_PATH)/../framework/Testbench.cpp

INC				:= -I../framework

CDEFS			:=

#-------------------------------------------------
# common document config
#-------------------------------------------------
include ../../../include/common.mk
