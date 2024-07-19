#-------------------------------------------------
# Project
#-------------------------------------------------
#TD_DESCRIPTION	= MTSP simulation test
#TD_TYPE		= profile
#TD_ERROR_TOKEN	= "*E: ", "*I: ":0, "*W: ":W
SRC_PATH		:= .
#TD_WORK_FOLDER :=
TARGETPATH		:= ../../../../Program
#TD_FILE_NAME	:= %%PROJECT%%Program\%s.exe
TARGETNAME		:= Testbench_MTSP_sim

SRCS			:= \
	$(SRC_PATH)/../framework/Testbench_MTSP.cpp \
	$(wildcard $(SRC_PATH)/*.cpp)
	
INC				:=
CDEFS			:=

#-------------------------------------------------
# common testbench config
#-------------------------------------------------
include ../include/Testbench.mk
