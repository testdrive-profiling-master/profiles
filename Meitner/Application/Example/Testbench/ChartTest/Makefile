#-------------------------------------------------
# Project
#-------------------------------------------------
#TD_DESCRIPTION	= Chart test
#TD_TYPE		= chart
#TD_ERROR_TOKEN	= "*E: ", "*I: ":0, "*W: ":0
SRC_PATH		:= .
#TD_WORK_FOLDER :=
TARGETPATH		:= ../../../../Program
#TD_FILE_NAME	:= %%PROJECT%%Program\%s.exe
TARGETNAME		:= Testbench_ChartTest

SRCS			:= \
	$(wildcard $(SRC_PATH)/*.cpp) \
	$(SRC_PATH)/../framework/common.cpp \
	$(SRC_PATH)/../framework/Chart.cpp
	
INC				:= -I../framework

CDEFS			:=

#-------------------------------------------------
# common document config
#-------------------------------------------------
include ../../../include/common.mk
