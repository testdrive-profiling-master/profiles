#-------------------------------------------------
# Project
#-------------------------------------------------
#TD_DESCRIPTION	= Dummy (NULL process)
#TD_TYPE		= profile
#TD_ERROR_TOKEN	= "*E: ", "*I: ":0, "*W: ":W
SRC_PATH		:= .
#TD_WORK_FOLDER :=
TARGETPATH		:= ../../../../Program
#TD_FILE_NAME	:= %%PROJECT%%Program\%s.exe
TARGETNAME		:= Testbench_Dummy
#USE_DEBUG		:= 1
#CPP_STANDARD	:= c++20
#MAIN_COMPILER	:= clang

SRCS			:= \
	$(wildcard $(SRC_PATH)/*.cpp) \
	$(SRC_PATH)/../framework/common.cpp \
	$(SRC_PATH)/../framework/Testbench.cpp
	
INC				:= -I../framework

CDEFS			:=

#-------------------------------------------------
# common document config
#-------------------------------------------------
include ../../../include/common.mk
