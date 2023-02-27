#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= .
TARGETPATH		:= ./release/
TARGETNAME		:= TestDrive_Installer
INSTALL_PATH	:= $(TARGETPATH)

SRCS			:= \
	$(wildcard $(SRC_PATH)/*.cpp) \
	$(wildcard $(SRC_PATH)/*.c)
	
INC				:= -I.
LIBDIR			:= -lssl -llibgit2 -lncurses
CDEFS			:=

#-------------------------------------------------
# common document config
#-------------------------------------------------
include ../include/util.mk
